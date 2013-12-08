/*
 Copyright (C) 2013 Nils Weiss, Patrick Bruenn.

 This file is part of Wifly_Light.

 Wifly_Light is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Wifly_Light is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef __WiflyControlCli__FwCommand__
#define __WiflyControlCli__FwCommand__

#include "FwResponse.h"
#include "WiflyColor.h"

#include <iostream>

namespace WyLight {

	class FwCommand
	{
	const size_t mSize;
	const bool requiresResponse;
	protected:
		struct led_cmd mReqFrame;
		FwCommand(uint8_t cmd, size_t size = 0, bool withResponse = true) : mSize(1 + size), requiresResponse(withResponse) { memset(&mReqFrame, 0, sizeof(mReqFrame)); mReqFrame.cmd = cmd; };
		virtual ~FwCommand(void) = default;

	public:
		const uint8_t *GetData(void) const { return reinterpret_cast<const uint8_t *>(&mReqFrame);       };
		size_t GetSize(void) const { return mSize; };
		const bool IsResponseRequired(void) const { return requiresResponse; };
		virtual FwResponse& GetResponse(void) = 0;
		virtual bool operator == (const FwCommand& ref) const
		{
			if(this->GetSize() != ref.GetSize()) return false;
			return memcmp(this->GetData(), ref.GetData(), std::min(this->GetSize(), ref.GetSize())) == 0;
		}
		virtual bool operator != (const FwCommand& ref) const
		{
			return !(*this == ref);
		}
	};

	struct FwCmdGet : public FwCommand
	{
		FwCmdGet(uint8_t cmd) : FwCommand(cmd) {};
	};

	class FwCmdSimple : public FwCommand
	{
	FwResponse mResponse;
	protected:
		FwCmdSimple(uint8_t cmd, size_t size = 0, bool withResponse = true) : FwCommand(cmd, size, withResponse), mResponse(cmd) {};
	public:
		FwResponse& GetResponse(void) { return mResponse;       };
	};

	class FwCmdScript : public FwCmdSimple
	{
	protected:
		FwCmdScript(uint8_t cmd, size_t size = 0) : FwCmdSimple(cmd, size) {};

	public:
		static const size_t INDENTATION_MAX = 10;
		static const char INDENTATION_CHARACTER = ' ';

		virtual std::ostream& Write(std::ostream& out, size_t& indentation) const {
			const size_t numCharacters = std::min(INDENTATION_MAX, 2 * indentation);
			return out << std::string(numCharacters, INDENTATION_CHARACTER);
		};
	};

	class FwCmdWait : public FwCmdScript
	{
	public:
		static const std::string TOKEN;
		FwCmdWait(std::istream& is) : FwCmdScript(WAIT, sizeof(cmd_wait)) {
			uint16_t waitTime;
			is >> waitTime;
			mReqFrame.data.wait.waitTmms = htons(std::max((uint16_t)1, waitTime));
		};

		/**
		 * Injects a wait command into the wifly script controller.
		 * This causes the script processing to wait before executing the next command for the specified duration
		 * @param waitTime in hundreths of a second
		 */
		FwCmdWait(uint16_t waitTime) : FwCmdScript(WAIT, sizeof(cmd_wait)) {
			mReqFrame.data.wait.waitTmms = htons(std::max((uint16_t)1, waitTime));
		};

		std::ostream& Write(std::ostream& out, size_t& indentation) const override {
			return FwCmdScript::Write(out, indentation) << TOKEN << ' ' << std::dec << ntohs(mReqFrame.data.wait.waitTmms);
		};
	};

/**
 * Wipe all commands from the WyLight script controller
 */
	struct FwCmdClearScript : public FwCmdSimple
	{
		FwCmdClearScript(void) : FwCmdSimple(CLEAR_SCRIPT) {};
	};

	struct FwCmdGetCycletime : public FwCmdGet
	{
		CycletimeResponse mResponse;
		FwCmdGetCycletime(void) : FwCmdGet(GET_CYCLETIME) {};
		FwResponse& GetResponse(void) { return mResponse;       };
	};

	struct FwCmdGetRtc : public FwCmdGet
	{
		RtcResponse mResponse;
		FwCmdGetRtc(void) : FwCmdGet(GET_RTC) {};
		FwResponse& GetResponse(void) { return mResponse;       };
	};

	struct FwCmdGetTracebuffer : public FwCmdGet
	{
		TracebufferResponse mResponse;
		FwCmdGetTracebuffer(void) : FwCmdGet(GET_TRACE) {};
		FwResponse& GetResponse(void) { return mResponse;       };
	};

	struct FwCmdGetVersion : public FwCmdGet
	{
		FirmwareVersionResponse mResponse;
		FwCmdGetVersion(void) : FwCmdGet(GET_FW_VERSION) {};
		FwResponse& GetResponse(void) { return mResponse;       };
	};

	struct FwCmdLoopOff : public FwCmdScript
	{
		static const std::string TOKEN;
		FwCmdLoopOff(std::istream& is) : FwCmdScript(LOOP_OFF, sizeof(cmd_loop_end)) {
			unsigned int numLoops;
			is >> numLoops;
			mReqFrame.data.loopEnd.numLoops = (uint8_t)numLoops;
		};

		/**
		 * Injects a LoopOff command into the wifly script controller
		 * @param numLoops number of rounds before termination of the loop, use 0 for infinite loops. To terminate an infinite loop you have to send a \<FwCmdClearScript\>
		 */
		FwCmdLoopOff(uint8_t numLoops = 0) : FwCmdScript(LOOP_OFF, sizeof(cmd_loop_end)) {
			mReqFrame.data.loopEnd.numLoops = numLoops;
		};
		
		std::ostream& Write(std::ostream& out, size_t& indentation) const override {
			return FwCmdScript::Write(out, --indentation) << TOKEN << ' ' << std::dec << (int)mReqFrame.data.loopEnd.numLoops;
		};
	};

/**
 * Injects a LoopOn command into the wifly script controller
 */
	struct FwCmdLoopOn : public FwCmdScript
	{
		static const std::string TOKEN;
		FwCmdLoopOn(void) : FwCmdScript(LOOP_ON) {};

		std::ostream& Write(std::ostream& out, size_t& indentation) const override {
			FwCmdScript::Write(out, indentation) << TOKEN;
			++indentation;
			return out;
		};
	};

	struct FwCmdSetColorDirect : public FwCmdSimple
	{
		/**
		 * Sets all leds with same color directly. This doesn't affect the WyLight script controller
		 * @param argb is a 32 bit rgb value with unused alpha channel (set alpha always to 0xff) f.e.
		 *        black(  0,  0,  0) as argb is 0xff000000
		 *        green(  0,255,  0) as argb is 0xff00ff00
		 *        white(255,255,255) as argb is 0xffffffff
		 * @param addr bitmask of leds which should be effected by this command, set bit to 1 to affect the led, default 0xffffffff
		 */
		FwCmdSetColorDirect(uint32_t argb, uint32_t addr) : FwCmdSimple(SET_COLOR_DIRECT, sizeof(cmd_set_color_direct), false)
		{
			const uint8_t red = (uint8_t)(argb >> 16);
			const uint8_t green = (uint8_t)(argb >> 8);
			const uint8_t blue = (uint8_t)argb;
			mReqFrame.data.set_color_direct.Set(red, green, blue, addr);
		};

		/**
		 * Sets all leds with different colors directly. This doesn't affect the WyLight script controller
		 * Example: to set the first led to yellow and the second to blue and all others to off use a \<pBuffer\> like this:
		 * pBuffer[] = {0xff, 0xff, 0x00, 0x00, 0x00, 0xff}; bufferLength = 6;
		 * @param pBuffer containing continouse rgb values r1g1b1r2g2b2...r32g32b32
		 * @param bufferLength number of bytes in \<pBuffer\> usally 32 * 3 bytes
		 */
		FwCmdSetColorDirect(const uint8_t *pBuffer, size_t bufferLength) : FwCmdSimple(SET_COLOR_DIRECT, sizeof(cmd_set_color_direct), false)
		{
			mReqFrame.data.set_color_direct.Set(pBuffer, bufferLength);
		};
	};

	struct FwCmdSetFade : public FwCmdScript
	{
		static const std::string TOKEN;
		FwCmdSetFade(std::istream& is) : FwCmdScript(SET_FADE, sizeof(cmd_set_fade)) {
			WiflyColor addr, argb;
			uint16_t fadeTime;
			bool parallelFade;
			is >> addr >> argb >> fadeTime >> parallelFade;
			mReqFrame.data.set_fade.Set(addr.argb(), argb.argb(), (uint8_t)parallelFade, fadeTime);
		};

		/**
		 * Injects a fade command into the wifly script controller
		 * @param argb is a 32 bit rgb value with unused alpha channel (set alpha always to 0xff) f.e.
		 *        black(  0,  0,  0) as argb is 0xff000000
		 *        green(  0,255,  0) as argb is 0xff00ff00
		 *        white(255,255,255) as argb is 0xffffffff
		 * @param fadeTime in hundreths of a second. Use 0 to set color immediately, default = 0
		 * @param addr bitmask of leds which should be effected by this command, set bit to 1 to affect the led, default 0xffffffff
		 * @param parallelFade if true other fades are allowed in parallel with this fade
		 */
		FwCmdSetFade(uint32_t argb, uint16_t fadeTime = 0, uint32_t addr = 0xffffffff, bool parallelFade = false)
			: FwCmdScript(SET_FADE, sizeof(cmd_set_fade)) {
			mReqFrame.data.set_fade.Set(addr, argb, (uint8_t)parallelFade, fadeTime);
		};

		uint32_t argb(void) const {
			return 0xff000000 | (uint32_t)mReqFrame.data.set_fade.red << 16 | (uint32_t)mReqFrame.data.set_fade.green << 8 | mReqFrame.data.set_fade.blue;
		};

		void argb(uint32_t argb) {
			mReqFrame.data.set_fade.red = (uint8_t)(argb >> 16);
			mReqFrame.data.set_fade.green = (uint8_t)(argb >> 8);
			mReqFrame.data.set_fade.blue = (uint8_t)argb;
		};

		std::ostream& Write(std::ostream& out, size_t& indentation) const override {
			FwCmdScript::Write(out, indentation) << TOKEN << ' ';
			return mReqFrame.data.set_fade.Write(out, indentation);
		};
	};

	struct FwCmdSetGradient : public FwCmdScript
	{
		static const std::string TOKEN;
		FwCmdSetGradient(std::istream& is) : FwCmdScript(SET_GRADIENT, sizeof(cmd_set_gradient)) {
			WiflyColor argb_1, argb_2;
			uint16_t fadeTime;
			int parallel, length, offset;
			is >> argb_1 >> argb_2 >> fadeTime >> offset >> length >> parallel;
			mReqFrame.data.set_gradient.Set(argb_1.argb(), argb_2.argb(), parallel, (uint8_t)offset, (uint8_t)length, fadeTime);
		};

		/**
		       * Injects a gradient command into the wifly script controller
		       * @param argb_1 is a 32 bit rgb value with unused alpha channel (set alpha always to 0xff). This is the start color for the gradient.
		       * @param argb_2 is a 32 bit rgb value with unused alpha channel (set alpha always to 0xff). This is the end color for the gradient.
		       * @param fadeTime in hundreths of a second. Use 0 to set color immediately, default = 0
		       * @param parallelFade if true other fades are allowed in parallel with this fade
		       * @param length is the number of led's from startposition to endposition
		       * @param offset can be used to move the startposition of the gradient on the ledstrip
		       */
		FwCmdSetGradient(uint32_t argb_1, uint32_t argb_2, uint16_t fadeTime = 0, bool parallelFade = false, uint8_t length = NUM_OF_LED, uint8_t offset = 0) : FwCmdScript(SET_GRADIENT, sizeof(cmd_set_gradient)) {

			mReqFrame.data.set_gradient.Set(argb_1, argb_2, parallelFade, offset, length, fadeTime);
		};

		std::ostream& Write(std::ostream& out, size_t& indentation) const override {
			FwCmdScript::Write(out, indentation) << TOKEN << ' ';
			return mReqFrame.data.set_gradient.Write(out, indentation);
		};
	};

	struct FwCmdSetRtc : public FwCmdSimple
	{
		/**
		 * Sets the rtc clock of the wifly device to the current time.
		 * The wifly device has to be in firmware mode for this command.
		 */
		FwCmdSetRtc(void) : FwCmdSimple(SET_RTC, sizeof(rtc_time))
		{
			tm timeinfo;
			time_t rawtime;
			rawtime = time(NULL);
			localtime_r(&rawtime, &timeinfo);
			SetTime(timeinfo);
		}

		/**
		 * Sets the rtc clock of the wifly device to the specified time.
		 * The wifly device has to be in firmware mode for this command.
		 * @param timeValue pointer to a posix tm struct containing the new time
		 */
		FwCmdSetRtc(const tm& timeValue) : FwCmdSimple(SET_RTC, sizeof(rtc_time))
		{
			SetTime(timeValue);
		};
		private:
			void SetTime(const tm& timeValue)
			{
				mReqFrame.data.set_rtc.tm_sec = (uns8) timeValue.tm_sec;
				mReqFrame.data.set_rtc.tm_min = (uns8) timeValue.tm_min;
				mReqFrame.data.set_rtc.tm_hour = (uns8) timeValue.tm_hour;
				mReqFrame.data.set_rtc.tm_mday = (uns8) timeValue.tm_mday;
				mReqFrame.data.set_rtc.tm_mon = (uns8) timeValue.tm_mon;
				mReqFrame.data.set_rtc.tm_year = (uns8) timeValue.tm_year;
				mReqFrame.data.set_rtc.tm_wday = (uns8) timeValue.tm_wday;
			};
	};

/**
 * Stops firmware and script controller execution and start the bootloader of the wifly device
 */
	struct FwCmdStartBl : public FwCmdSimple
	{
		FwCmdStartBl(void) : FwCmdSimple(START_BL) {};
	};

}

#endif /* defined(__WiflyControlCli__FwCommand__) */
