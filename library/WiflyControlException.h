/*
 Copyright (C) 2012, 2013 Nils Weiss, Patrick Bruenn.

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

#ifndef ____WiflyControlException__
#define ____WiflyControlException__

#include <stdio.h>
#include <exception>
#include <string>
#include <typeinfo>
#include "wifly_cmd.h"
#include "BlRequest.h"

namespace WyLight {

/******************************************************************************/
/*!\file WiflyControlException.h
 * \author Nils Weiss, Patrick Bruenn
 *
 * \cond
 * enum - WiflyError
 * \endcond
 *
 * \brief Returnvalues of WiflyControlNoThrow
 *
 *
 *******************************************************************************/

	enum WiflyError {
		NO_ERROR = 0,           /**< is returned if no error occurred */
		FATAL_ERROR,            /**< if command code of the response doesn't match the code of the request, or too many retries failed */
		CONNECTION_LOST,
		CONNECTION_TIMEOUT,     /**< if response timed out */
		INVALID_PARAMETER,      /**< if a parameter is out of bound */
		SCRIPT_FULL
	};

	class FatalError : public std::exception
	{
	public:
		FatalError(const std::string& description, uint32_t errorCode = FATAL_ERROR) throw () : mDescription(description), mErrorCode(errorCode) {};
		virtual ~FatalError(void) throw () {};

		uint32_t AsErrorCode(void) const {
			return mErrorCode;
		};

		virtual const char *GetJavaClassType(void) const {
			return "biz/bruenn/WyLight/exception/FatalError";
		};

		const char *what(void) const throw() {
			return mDescription.c_str();
		};

		friend std::ostream& operator<< (std::ostream& out, const FatalError& ref)
		{
			return out << ref.mDescription;
		};

	protected:
		const std::string mDescription;
		const uint32_t mErrorCode;
	};

	class ConnectionLost : public FatalError
	{
	public:
		ConnectionLost(const std::string& description, uint32_t addr, uint16_t port)
			: FatalError(description, CONNECTION_LOST), mAddress(addr), mPort(port)
		{};

		virtual const char *GetJavaClassType(void) const {
			return "biz/bruenn/WyLight/exception/ConnectionLost";
		};

		friend std::ostream& operator<< (std::ostream& out, const ConnectionLost& ref)
		{
			return out << "ConnectionLost to: " << ref.mAddress << ':' << ref.mPort
				   << " due: '" << ref.mDescription << '\'';
		};

	protected:
		uint32_t mAddress;
		uint16_t mPort;
	};

	class ConnectionTimeout : public FatalError
	{
	public:
		ConnectionTimeout(const std::string& description) : FatalError(description, CONNECTION_TIMEOUT) {};

		virtual const char *GetJavaClassType(void) const {
			return "biz/bruenn/WyLight/exception/ConnectionTimeout";
		};
	};

	class InvalidParameter : public FatalError
	{
	public:
		InvalidParameter(const std::string& description) : FatalError(description, INVALID_PARAMETER) {};

		virtual const char *GetJavaClassType(void) const {
			return "biz/bruenn/WyLight/exception/InvalidParameter";
		};
	};

	class ScriptBufferFull : public FatalError
	{
	public:
		ScriptBufferFull(void) : FatalError("ScriptBuffer in PIC is full, clear it or wait", SCRIPT_FULL) {};

		virtual const char *GetJavaClassType(void) const {
			return "biz/bruenn/WyLight/exception/ScriptBufferFull";
		};
	};
}
#endif /* defined(____WiflyControlException__) */
