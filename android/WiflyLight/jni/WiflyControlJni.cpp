/**
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

#include "BroadcastReceiver.h"
#include "ScriptManager.h"
#include "StartupManager.h"
#include "WiflyControl.h"
#include <sstream>
#include <unistd.h>
#include <jni.h>

namespace WyLight {
	static Control *g_pControl = NULL;

	void ThrowJniException(JNIEnv *env, const FatalError& e) {
		jclass javaException = env->FindClass(e.GetJavaClassType());
		env->ThrowNew(javaException, e.what());
	}

	jboolean TrySend(JNIEnv *env, Control *pCtrl, FwCommand&& cmd)
	{
		assert(pCtrl);
		try {
			*pCtrl << std::move(cmd);
			return true;
		} catch(FatalError& e) {
					ThrowJniException(env, e);
		}
		return false;
	}

	extern "C" {
		jlong Java_biz_bruenn_WyLight_BroadcastReceiver_create(JNIEnv *env, jobject ref, jstring path)
		{
			const char *myPath = env->GetStringUTFChars(path, 0);
			const jlong result = (jlong) new BroadcastReceiver(BroadcastReceiver::BROADCAST_PORT, myPath);
			env->ReleaseStringUTFChars(path, myPath);
			return result;
		}

		jlong Java_biz_bruenn_WyLight_BroadcastReceiver_getEndpoint(JNIEnv *env, jobject ref, jlong pNative, jlong index)
		{
			return ((BroadcastReceiver *)pNative)->GetEndpoint(index).AsUint64();
		}

		jlong Java_biz_bruenn_WyLight_BroadcastReceiver_getNextRemote(JNIEnv *env, jobject ref, jlong pNative, jlong timeoutNanos)
		{
			timeval timeout;
			timeout.tv_sec = timeoutNanos / 1000000000L;
			timeout.tv_usec = (timeoutNanos % 1000000000L) / 1000L;
			return ((BroadcastReceiver *)pNative)->GetNextRemote(&timeout).AsUint64();
		}

		void Java_biz_bruenn_WyLight_BroadcastReceiver_release(JNIEnv *env, jobject ref, jlong pNative)
		{
			delete (BroadcastReceiver *)pNative;
		}

		jlong Java_biz_bruenn_WyLight_Endpoint_connect(JNIEnv *env, jobject ref, jlong pBroadcastReceiver,  jlong fingerprint)
		{
			// TODO make this threadsafe
			if(NULL == g_pControl) {
				try {
					Endpoint& remote = ((BroadcastReceiver *)pBroadcastReceiver)->GetEndpointByFingerprint(fingerprint);
					++remote;
					g_pControl = new Control(remote.GetIp(), remote.GetPort());
					return reinterpret_cast<jlong>(g_pControl);
				} catch(FatalError& e) {
					g_pControl = NULL;
					ThrowJniException(env, e);
				}
			}
			return static_cast<jlong>(NULL);
		}

		jstring Java_biz_bruenn_WyLight_Endpoint_getEndpointName(JNIEnv *env, jobject ref, jlong pBroadcastReceiver,  jlong fingerprint)
		{
			try {
				const Endpoint& remote = ((BroadcastReceiver *)pBroadcastReceiver)->GetEndpointByFingerprint(fingerprint);

				const std::string myDeviceId = remote.GetDeviceId();
				return env->NewStringUTF(myDeviceId.data());
			} catch(FatalError& e) {
					ThrowJniException(env, e);
			}
			return env->NewStringUTF("");
		}

		void Java_biz_bruenn_WyLight_Endpoint_setEndpointName(JNIEnv *env, jobject ref, jlong pBroadcastReceiver,  jlong fingerprint, jstring deviceId)
		{
			try {
				const char *const myDeviceId = env->GetStringUTFChars(deviceId, 0);
				Endpoint& remote = ((BroadcastReceiver *)pBroadcastReceiver)->GetEndpointByFingerprint(fingerprint);
				remote.SetDeviceId(myDeviceId);
				env->ReleaseStringUTFChars(deviceId, myDeviceId);
			} catch(FatalError& e) {
					ThrowJniException(env, e);
			}
		}

		jstring Java_biz_bruenn_WyLight_WiflyControl_ConfGetDeviceId(JNIEnv *env, jobject ref, jlong pNative)
		{
			std::string myDeviceId = reinterpret_cast<Control *>(pNative)->ConfGetDeviceId();
			return env->NewStringUTF(myDeviceId.data());
		}

		jstring Java_biz_bruenn_WyLight_WiflyControl_ConfGetPassphrase(JNIEnv *env, jobject ref, jlong pNative)
		{
			std::string myPassphrase = reinterpret_cast<Control *>(pNative)->ConfGetPassphrase();
			return env->NewStringUTF(myPassphrase.data());
		}

		jboolean Java_biz_bruenn_WyLight_WiflyControl_ConfGetSoftAp(JNIEnv *env, jobject ref, jlong pNative)
		{
			return reinterpret_cast<Control *>(pNative)->ConfGetSoftAp();
		}

		jstring Java_biz_bruenn_WyLight_WiflyControl_ConfGetSsid(JNIEnv *env, jobject ref, jlong pNative)
		{
			std::string mySsid = reinterpret_cast<Control *>(pNative)->ConfGetSsid();
			return env->NewStringUTF(mySsid.data());
		}

		jboolean Java_biz_bruenn_WyLight_WiflyControl_ConfSetWlan(JNIEnv *env, jobject ref, jlong pNative, jstring passphrase, jstring ssid, jstring deviceId, jboolean softAp)
		{
			const char *const myDeviceId = env->GetStringUTFChars(deviceId, 0);
			const char *const myPassphrase = env->GetStringUTFChars(passphrase, 0);
			const char *const mySsid = env->GetStringUTFChars(ssid, 0);
			jboolean result;
			if(softAp) {
				result = reinterpret_cast<Control *>(pNative)->ConfModuleAsSoftAP(mySsid);
			} else {
				result = reinterpret_cast<Control *>(pNative)->ConfModuleForWlan(myPassphrase, mySsid, myDeviceId);
			}
			env->ReleaseStringUTFChars(deviceId, myDeviceId);
			env->ReleaseStringUTFChars(passphrase, myPassphrase);
			env->ReleaseStringUTFChars(ssid, mySsid);
			return result;
		}

		jboolean Java_biz_bruenn_WyLight_WiflyControl_FwClearScript(JNIEnv *env, jobject ref, jlong pNative)
		{
			return TrySend(env, reinterpret_cast<Control *>(pNative), FwCmdClearScript {}
				       );
		}

		jboolean Java_biz_bruenn_WyLight_WiflyControl_FwLoopOff(JNIEnv *env, jobject ref, jlong pNative, jbyte numLoops)
		{
			return TrySend(env, reinterpret_cast<Control *>(pNative), FwCmdLoopOff {(uint8_t)numLoops}
				       );
		}

		jboolean Java_biz_bruenn_WyLight_WiflyControl_FwLoopOn(JNIEnv *env, jobject ref, jlong pNative)
		{
			return TrySend(env, reinterpret_cast<Control *>(pNative), FwCmdLoopOn {}
				       );
		}

		jboolean Java_biz_bruenn_WyLight_WiflyControl_FwSendScript(JNIEnv *env, jobject ref, jlong pNative, jlong pNativeScript)
		{
			try {
				Control *pControl = reinterpret_cast<Control *>(pNative);
				Script *pScript = reinterpret_cast<Script *>(pNativeScript);
				*pControl << *pScript;
			} catch(FatalError& e) {
					ThrowJniException(env, e);
			}
			return true;
		}

		jboolean Java_biz_bruenn_WyLight_WiflyControl_FwSetColor(JNIEnv *env, jobject ref, jlong pNative, jint argb, jint addr)
		{
			return TrySend(env, reinterpret_cast<Control *>(pNative), FwCmdSetColorDirect {(uint32_t)argb, (uint32_t)addr}
				       );
		}

		jboolean Java_biz_bruenn_WyLight_WiflyControl_FwSetFade(JNIEnv *env, jobject ref, jlong pNative, jint argb, jint addr, jshort fadeTime)
		{
			return TrySend(env, reinterpret_cast<Control *>(pNative), FwCmdSetFade {(uint32_t)argb, (uint16_t)fadeTime, (uint32_t)addr, false}
				       );
		}

		jboolean Java_biz_bruenn_WyLight_WiflyControl_FwSetGradient(JNIEnv *env, jobject ref, jlong pNative, jint argb_1, jint argb_2, jint length, jint offset, jshort fadeTime)
		{
			return TrySend(env, reinterpret_cast<Control *>(pNative), FwCmdSetGradient(argb_1, argb_2, fadeTime, false, length, offset));
		}

		void Java_biz_bruenn_WyLight_WiflyControl_release(JNIEnv *env, jobject ref, jlong pNative)
		{
			if((Control *)pNative == g_pControl) {
				delete g_pControl;
				g_pControl = NULL;
			}
		}

		void Java_biz_bruenn_WyLight_WiflyControl_Startup(JNIEnv *env, jobject ref, jlong pNative, jstring path)
		{
			const char *const myPath = env->GetStringUTFChars(path, 0);
			Control& myControl = *reinterpret_cast<Control*>(pNative);
			StartupManager manager;
			try {
				manager.startup(myControl, myPath);
			} catch (FatalError& e) {
				ThrowJniException(env, e);
			}
			env->ReleaseStringUTFChars(path, myPath);
		}

		jint Java_biz_bruenn_WyLight_library_FwCmdScriptAdapter_getFadeColor(JNIEnv *env, jobject ref, jlong pNative)
		{
			auto fadeCommand = reinterpret_cast<const FwCmdSetFade *>(pNative);
			return fadeCommand->argb();
		}

		void Java_biz_bruenn_WyLight_library_FwCmdScriptAdapter_setFadeColor(JNIEnv *env, jobject ref, jlong pNative, jint argb)
		{
			auto fadeCommand = reinterpret_cast<FwCmdSetFade *>(pNative);
			fadeCommand->argb(argb);
		}

		void Java_biz_bruenn_WyLight_library_ScriptAdapter_addFade(JNIEnv *env, jobject ref, jlong pNative, jint argb, jint addr, jshort fadeTime)
		{
			reinterpret_cast<Script *>(pNative)->push_back(new FwCmdSetFade(argb, fadeTime, addr));
		}

		void Java_biz_bruenn_WyLight_library_ScriptAdapter_clear(JNIEnv *env, jobject ref, jlong pNative)
		{
			reinterpret_cast<Script *>(pNative)->clear();
		}

		jlong Java_biz_bruenn_WyLight_library_ScriptAdapter_getItem(JNIEnv *env, jobject ref, jlong pNative, jint position)
		{
			auto fwCmdScript = reinterpret_cast<Script *>(pNative)->begin();
			std::advance(fwCmdScript, position);
			return reinterpret_cast<jlong>(*fwCmdScript);
		}

		jstring Java_biz_bruenn_WyLight_library_ScriptAdapter_name(JNIEnv *env, jobject ref, jlong pNative)
		{
			const std::string& myName = reinterpret_cast<Script *>(pNative)->getName();
			return env->NewStringUTF(myName.data());
		}

		jint Java_biz_bruenn_WyLight_library_ScriptAdapter_numCommands(JNIEnv *env, jobject ref, jlong pNative)
		{
			return reinterpret_cast<const Script *>(pNative)->size();
		}

		jlong Java_biz_bruenn_WyLight_library_ScriptAdapter_create(JNIEnv *env, jobject ref, jstring path)
		{
			Script *result = NULL;
			const char *const myPath = env->GetStringUTFChars(path, 0);
			try {
				result = new Script(myPath);
			} catch(FatalError& e) {
					ThrowJniException(env, e);
			}
			env->ReleaseStringUTFChars(path, myPath);
			return reinterpret_cast<jlong>(result);
		}

		jstring Java_biz_bruenn_WyLight_library_ScriptManagerAdapter_getScriptName(JNIEnv *env, jobject ref, jstring path, jint index)
		{
			jstring result = NULL;
			const char *const myPath = env->GetStringUTFChars(path, 0);
			try {
				ScriptManager manager {myPath};
				result = env->NewStringUTF(manager.getScriptName(index).data());
			} catch(FatalError& e) {
					ThrowJniException(env, e);
			}
			env->ReleaseStringUTFChars(path, myPath);
			return result;
		}

		void jni_serialize_Script(JNIEnv *env, jstring path, const Script& script)
		{
			const char *const myPath = env->GetStringUTFChars(path, 0);
			try {
				Script::serialize(myPath, script);
			} catch(FatalError& e) {
					ThrowJniException(env, e);
			}
			env->ReleaseStringUTFChars(path, myPath);
		}

		void Java_biz_bruenn_WyLight_library_ScriptManagerAdapter_newScript(JNIEnv *env, jobject ref, jstring path, jstring scriptName)
		{
			// Script constructor can throw, too. So we need an additional try/catch outside of jni_serialize_Script
			try {
				jni_serialize_Script(env, path, Script {}
						     );
			} catch(FatalError& e) {
				ThrowJniException(env, e);
			}
		}

		jint Java_biz_bruenn_WyLight_library_ScriptManagerAdapter_numScripts(JNIEnv *env, jobject ref, jstring path)
		{
			const char *const myPath = env->GetStringUTFChars(path, 0);
			jint numScripts = 0;
			try {
				ScriptManager manager {myPath};
				numScripts = manager.numScripts();
			} catch(FatalError& e) {
				ThrowJniException(env, e);
			}
			env->ReleaseStringUTFChars(path, myPath);
			return numScripts;
		}

		void Java_biz_bruenn_WyLight_library_ScriptManagerAdapter_saveScript(JNIEnv *env, jobject ref, jstring path, jlong pNativeScript)
		{
			jni_serialize_Script(env, path, *(reinterpret_cast<Script *>(pNativeScript)));
		}

	} /* extern "C" */
} /* namespace WyLight */

