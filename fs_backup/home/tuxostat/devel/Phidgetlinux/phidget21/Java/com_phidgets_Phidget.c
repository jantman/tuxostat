#include "../stdafx.h"
#include <stdlib.h>
#include "com_phidgets_Phidget.h"
#include "../cphidget.h"
#include "../csocket.h"
#include "phidget_jni.h"

static int CCONV attach_handler(CPhidgetHandle h, void *arg);
static int CCONV detach_handler(CPhidgetHandle h, void *arg);
static int CCONV serverConnect_handler(CPhidgetHandle h, void *arg);
static int CCONV serverDisconnect_handler(CPhidgetHandle h, void *arg);
static int CCONV error_handler(CPhidgetHandle h, void *, int l, const char *arg);

jfieldID handle_fid;
static jfieldID nativeAttachHandler_fid;
static jfieldID nativeDetachHandler_fid;
static jfieldID nativeErrorHandler_fid;
static jfieldID nativeServerConnectHandler_fid;
static jfieldID nativeServerDisconnectHandler_fid;
static jmethodID attachEvent_cons;
static jmethodID detachEvent_cons;
static jmethodID errorEvent_cons;
static jmethodID serverConnectEvent_cons;
static jmethodID serverDisconnectEvent_cons;
static jmethodID fireAttach_mid;
static jmethodID fireDetach_mid;
static jmethodID fireError_mid;
static jmethodID fireServerConnect_mid;
static jmethodID fireServerDisconnect_mid;
static jclass attachEvent_class;
static jclass detachEvent_class;
static jclass errorEvent_class;
static jclass serverConnectEvent_class;
static jclass serverDisconnectEvent_class;
static jclass phidget_class;

void
com_phidgets_Phidget_OnLoad(JNIEnv *env)
{
	if (!(phidget_class = (*env)->FindClass(env, "com/phidgets/Phidget")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(attachEvent_class = (*env)->FindClass(env, 
	  "com/phidgets/event/AttachEvent")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(attachEvent_class = (jclass)(*env)->NewGlobalRef(env, attachEvent_class)))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(detachEvent_class = (*env)->FindClass(env, 
	  "com/phidgets/event/DetachEvent")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(detachEvent_class = (jclass)(*env)->NewGlobalRef(env, detachEvent_class)))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(errorEvent_class = (*env)->FindClass(env, 
	  "com/phidgets/event/ErrorEvent")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(errorEvent_class = (jclass)(*env)->NewGlobalRef(env, errorEvent_class)))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(serverConnectEvent_class = (*env)->FindClass(env, 
	  "com/phidgets/event/ServerConnectEvent")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(serverConnectEvent_class = (jclass)(*env)->NewGlobalRef(env, serverConnectEvent_class)))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(serverDisconnectEvent_class = (*env)->FindClass(env, 
	  "com/phidgets/event/ServerDisconnectEvent")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(serverDisconnectEvent_class = (jclass)(*env)->NewGlobalRef(env, serverDisconnectEvent_class)))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(fireAttach_mid = (*env)->GetMethodID(env, phidget_class,
	  "fireAttach", "(Lcom/phidgets/event/AttachEvent;)V")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(fireDetach_mid = (*env)->GetMethodID(env, phidget_class,
	  "fireDetach", "(Lcom/phidgets/event/DetachEvent;)V")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(fireError_mid = (*env)->GetMethodID(env, phidget_class,
	  "fireError", "(Lcom/phidgets/event/ErrorEvent;)V")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(fireServerConnect_mid = (*env)->GetMethodID(env, phidget_class,
	  "fireServerConnect", "(Lcom/phidgets/event/ServerConnectEvent;)V")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(fireServerDisconnect_mid = (*env)->GetMethodID(env, phidget_class,
	  "fireServerDisconnect", "(Lcom/phidgets/event/ServerDisconnectEvent;)V")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(attachEvent_cons = (*env)->GetMethodID(env, attachEvent_class,
	  "<init>", "(Lcom/phidgets/Phidget;)V")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(detachEvent_cons = (*env)->GetMethodID(env, detachEvent_class,
	  "<init>", "(Lcom/phidgets/Phidget;)V")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(errorEvent_cons = (*env)->GetMethodID(env, errorEvent_class,
	  "<init>", "(Lcom/phidgets/Phidget;)V")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(serverConnectEvent_cons = (*env)->GetMethodID(env, serverConnectEvent_class,
	  "<init>", "(Ljava/lang/Object;)V")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(serverDisconnectEvent_cons = (*env)->GetMethodID(env, serverDisconnectEvent_class,
	  "<init>", "(Ljava/lang/Object;)V")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(handle_fid = (*env)->GetFieldID(env, phidget_class,
	  "handle", "J")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(nativeAttachHandler_fid = (*env)->GetFieldID(env, phidget_class,
	  "nativeAttachHandler", "J")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(nativeDetachHandler_fid = (*env)->GetFieldID(env, phidget_class,
	  "nativeDetachHandler", "J")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(nativeErrorHandler_fid = (*env)->GetFieldID(env, phidget_class,
	  "nativeErrorHandler", "J")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(nativeServerConnectHandler_fid = (*env)->GetFieldID(env, phidget_class,
	  "nativeServerConnectHandler", "J")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
	if (!(nativeServerDisconnectHandler_fid = (*env)->GetFieldID(env, phidget_class,
	  "nativeServerDisconnectHandler", "J")))
	{
		(*env)->ExceptionDescribe(env);
		(*env)->ExceptionClear(env);
		abort();
	}
}

JNIEXPORT void JNICALL
Java_com_phidgets_Phidget_enableAttachEvents(JNIEnv *env, jobject obj,
  jboolean b)
{
	jlong gr = updateGlobalRef(env, obj, nativeAttachHandler_fid, b);
	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)(*env)->GetLongField(env,
	    obj, handle_fid);

	CPhidget_set_OnAttach_Handler(h, b ? attach_handler : 0,
	    (void *)(uintptr_t)gr);
}

static int CCONV
attach_handler(CPhidgetHandle h, void *arg)
{
	JNIEnv *env;
	jobject obj;
	jobject attachEvent;
	jint result;

	result = (*ph_vm)->GetEnv(ph_vm, (void **)&env, JNI_VERSION_1_4);
	
	if(result == JNI_EDETACHED)
	{
	if ((*ph_vm)->AttachCurrentThread(ph_vm, (void **)&env, NULL))
		abort();
	}

	obj = (jobject)arg;

	if (!(attachEvent = (*env)->NewObject(env, attachEvent_class, attachEvent_cons,
	  obj)))
		return -1;
	(*env)->CallVoidMethod(env, obj, fireAttach_mid, attachEvent);
	(*env)->DeleteLocalRef(env, attachEvent);
	(*ph_vm)->DetachCurrentThread(ph_vm);

	return 0;
}

JNIEXPORT void JNICALL
Java_com_phidgets_Phidget_enableDetachEvents(JNIEnv *env, jobject obj,
  jboolean b)
{
	jlong gr = updateGlobalRef(env, obj, nativeDetachHandler_fid, b);
	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)(*env)->GetLongField(env,
	    obj, handle_fid);

	CPhidget_set_OnDetach_Handler(h, b ? detach_handler : 0, (void *)(uintptr_t)gr);
}

static int CCONV
detach_handler(CPhidgetHandle h, void *arg)
{
	JNIEnv *env;
	jobject obj;
	jobject detachEvent;

	if ((*ph_vm)->AttachCurrentThread(ph_vm, (void **)&env, NULL))
		abort();

	obj = (jobject)arg;

	if (!(detachEvent = (*env)->NewObject(env, detachEvent_class, detachEvent_cons,
	  obj)))
		return -1;
	(*env)->CallVoidMethod(env, obj, fireDetach_mid, detachEvent);
	(*env)->DeleteLocalRef(env, detachEvent);
	(*ph_vm)->DetachCurrentThread(ph_vm);

	return 0;
}

JNIEXPORT void JNICALL
Java_com_phidgets_Phidget_enableErrorEvents(JNIEnv *env, jobject obj,
  jboolean b)
{
	jlong gr = updateGlobalRef(env, obj, nativeErrorHandler_fid, b);
	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)(*env)->GetLongField(env,
	    obj, handle_fid);

	CPhidget_set_OnError_Handler(h, b ? error_handler : 0, (void *)(uintptr_t)gr);
}

static int CCONV
error_handler(CPhidgetHandle h, void *arg, int l, const char *v)
{
	JNIEnv *env;
	jobject obj;
	jobject errorEvent;

	if ((*ph_vm)->AttachCurrentThread(ph_vm, (void **)&env, NULL))
		abort();

	obj = (jobject)arg;

	if (!(errorEvent = (*env)->NewObject(env, errorEvent_class, errorEvent_cons,
	  obj)))
		return -1;
	(*env)->CallVoidMethod(env, obj, fireError_mid, errorEvent);
	(*env)->DeleteLocalRef(env, errorEvent);
	(*ph_vm)->DetachCurrentThread(ph_vm);

	return 0;
}

JNIEXPORT void JNICALL
Java_com_phidgets_Phidget_enableServerConnectEvents(JNIEnv *env, jobject obj,
  jboolean b)
{
	jlong gr = updateGlobalRef(env, obj, nativeServerConnectHandler_fid, b);
	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)(*env)->GetLongField(env,
	    obj, handle_fid);

	CPhidget_set_OnServerConnect_Handler(h, b ? serverConnect_handler : 0,
	    (void *)(uintptr_t)gr);
}

static int CCONV
serverConnect_handler(CPhidgetHandle h, void *arg)
{
	JNIEnv *env;
	jobject obj;
	jobject serverConnectEvent;
	jint result;

	result = (*ph_vm)->GetEnv(ph_vm, (void **)&env, JNI_VERSION_1_4);
	
	if(result == JNI_EDETACHED)
	{
	if ((*ph_vm)->AttachCurrentThread(ph_vm, (void **)&env, NULL))
		abort();
	}

	obj = (jobject)arg;

	if (!(serverConnectEvent = (*env)->NewObject(env, serverConnectEvent_class, serverConnectEvent_cons,
	  obj)))
		return -1;
	(*env)->CallVoidMethod(env, obj, fireServerConnect_mid, serverConnectEvent);
	(*env)->DeleteLocalRef(env, serverConnectEvent);
	(*ph_vm)->DetachCurrentThread(ph_vm);

	return 0;
}

JNIEXPORT void JNICALL
Java_com_phidgets_Phidget_enableServerDisconnectEvents(JNIEnv *env, jobject obj,
  jboolean b)
{
	jlong gr = updateGlobalRef(env, obj, nativeServerDisconnectHandler_fid, b);
	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)(*env)->GetLongField(env,
	    obj, handle_fid);

	CPhidget_set_OnServerDisconnect_Handler(h, b ? serverDisconnect_handler : 0,
	    (void *)(uintptr_t)gr);
}

static int CCONV
serverDisconnect_handler(CPhidgetHandle h, void *arg)
{
	JNIEnv *env;
	jobject obj;
	jobject serverDisconnectEvent;
	jint result;

	result = (*ph_vm)->GetEnv(ph_vm, (void **)&env, JNI_VERSION_1_4);
	
	if(result == JNI_EDETACHED)
	{
	if ((*ph_vm)->AttachCurrentThread(ph_vm, (void **)&env, NULL))
		abort();
	}

	obj = (jobject)arg;

	if (!(serverDisconnectEvent = (*env)->NewObject(env, serverDisconnectEvent_class, serverDisconnectEvent_cons,
	  obj)))
		return -1;
	(*env)->CallVoidMethod(env, obj, fireServerDisconnect_mid, serverDisconnectEvent);
	(*env)->DeleteLocalRef(env, serverDisconnectEvent);
	(*ph_vm)->DetachCurrentThread(ph_vm);

	return 0;
}

JNIEXPORT void JNICALL
Java_com_phidgets_Phidget_nativeOpen(JNIEnv *env, jobject obj, jint ser)
{
	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)(*env)->GetLongField(env,
	    obj, handle_fid);
	int error;
	
	if ((error = CPhidget_open(h, ser)))
		PH_THROW(error);
}

JNIEXPORT void JNICALL
Java_com_phidgets_Phidget_nativeOpenRemote(JNIEnv *env, jobject obj, jint ser, jstring serverID, jstring pass)
{
	int error;
    jboolean iscopy;
    const char *serverIDcopy = serverID?(*env)->GetStringUTFChars(
				env, serverID, &iscopy):0;
    const char *passcopy = (*env)->GetStringUTFChars(
                env, pass, &iscopy);

	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)
	    (*env)->GetLongField(env, obj, handle_fid);

	if ((error = CPhidget_openRemote(h, ser, serverIDcopy, passcopy)))
		PH_THROW(error);
}

JNIEXPORT void JNICALL
Java_com_phidgets_Phidget_nativeOpenRemoteIP(JNIEnv *env, jobject obj, jint ser, jstring ipAddr, jint port, jstring pass)
{
	int error;
    jboolean iscopy;
    const char *ipAddrcopy = (*env)->GetStringUTFChars(
                env, ipAddr, &iscopy);
    const char *passcopy = (*env)->GetStringUTFChars(
                env, pass, &iscopy);

	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)
	    (*env)->GetLongField(env, obj, handle_fid);

	if ((error = CPhidget_openRemoteIP(h, ser, ipAddrcopy, port, passcopy)))
		PH_THROW(error);
}

JNIEXPORT void JNICALL
Java_com_phidgets_Phidget_nativeClose(JNIEnv *env, jobject obj)
{
	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)(*env)->GetLongField(env,
	    obj, handle_fid);
	int error;
	
	if ((error = CPhidget_close(h)))
		PH_THROW(error);
}

JNIEXPORT void JNICALL
Java_com_phidgets_Phidget_nativeDelete(JNIEnv *env, jobject obj)
{
	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)(*env)->GetLongField(env,
	    obj, handle_fid);
	int error;
	
	if ((error = CPhidget_delete(h)))
		PH_THROW(error);
}

JNIEXPORT jint JNICALL
Java_com_phidgets_Phidget_getSerialNumber(JNIEnv *env, jobject obj)
{
	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)(*env)->GetLongField(env,
	    obj, handle_fid);
	int error;
	int ser;
	
	if ((error = CPhidget_getSerialNumber(h, &ser)))
		PH_THROW(error);

	return ser;
}

JNIEXPORT jint JNICALL
Java_com_phidgets_Phidget_getServerPort(JNIEnv *env, jobject obj)
{
	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)(*env)->GetLongField(env,
	    obj, handle_fid);
	int error;
	int port;
	const char *addr;

	if ((error = CPhidget_getServerAddress(h, &addr, &port)))
		PH_THROW(error);

	return port;
}

JNIEXPORT jstring JNICALL
Java_com_phidgets_Phidget_getServerAddress(JNIEnv *env, jobject obj)
{
	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)(*env)->GetLongField(env,
	    obj, handle_fid);
	int error;
	int port;
	const char *addr;

	if ((error = CPhidget_getServerAddress(h, &addr, &port)))
		PH_THROW(error);

	return (*env)->NewStringUTF(env, addr);
}

JNIEXPORT jstring JNICALL
Java_com_phidgets_Phidget_getServerID(JNIEnv *env, jobject obj)
{
	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)(*env)->GetLongField(env,
	    obj, handle_fid);
	int error;
	const char *id;

	if ((error = CPhidget_getServerID(h, &id)))
		PH_THROW(error);

	return (*env)->NewStringUTF(env, id);
}

JNIEXPORT jint JNICALL
Java_com_phidgets_Phidget_getDeviceVersion(JNIEnv *env, jobject obj)
{
	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)(*env)->GetLongField(env,
	    obj, handle_fid);
	int error;
	int ver;
	
	if ((error = CPhidget_getDeviceVersion(h, &ver)))
		PH_THROW(error);

	return ver;
}

JNIEXPORT jstring JNICALL
Java_com_phidgets_Phidget_getDeviceType(JNIEnv *env, jobject obj)
{
	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)(*env)->GetLongField(env,
	    obj, handle_fid);
	const char *type;
	int error;
	
	if ((error = CPhidget_getDeviceType(h, &type)))
		PH_THROW(error);

	return (*env)->NewStringUTF(env, type);
}

JNIEXPORT jstring JNICALL
Java_com_phidgets_Phidget_getDeviceName(JNIEnv *env, jobject obj)
{
	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)(*env)->GetLongField(env,
	    obj, handle_fid);
	const char *type;
	int error;
	
	if ((error = CPhidget_getDeviceName(h, &type)))
		PH_THROW(error);

	return (*env)->NewStringUTF(env, type);
}

JNIEXPORT jstring JNICALL
Java_com_phidgets_Phidget_getDeviceLabel(JNIEnv *env, jobject obj)
{
	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)(*env)->GetLongField(env,
	    obj, handle_fid);
	const char *label;
	int error;
	
	if ((error = CPhidget_getDeviceLabel(h, &label)))
		PH_THROW(error);

	return (*env)->NewStringUTF(env, label);
}

JNIEXPORT void JNICALL
Java_com_phidgets_Phidget_setDeviceLabel(JNIEnv *env, jobject obj, jstring v)
{
	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)
	    (*env)->GetLongField(env, obj, handle_fid);
	int error;
    jboolean iscopy;
    const char *textString = (*env)->GetStringUTFChars(
                env, v, &iscopy);
	if ((error = CPhidget_setDeviceLabel(h, (char *)textString)))
		PH_THROW(error);
	(*env)->ReleaseStringUTFChars(env, v, textString);
}

JNIEXPORT void JNICALL
Java_com_phidgets_Phidget_nativeEnableLogging(JNIEnv *env, jclass cls, jint level, jstring file)
{
	int error;
    jboolean iscopy;
    const char *textString = (*env)->GetStringUTFChars(
                env, file, &iscopy);
	if ((error = CPhidget_enableLogging(level, (char *)textString)))
		PH_THROW(error);
	(*env)->ReleaseStringUTFChars(env, file, textString);
}

JNIEXPORT void JNICALL
Java_com_phidgets_Phidget_disableLogging(JNIEnv *env, jclass cls)
{
	int error;
	if ((error = CPhidget_disableLogging()))
		PH_THROW(error);
}

JNIEXPORT void JNICALL
Java_com_phidgets_Phidget_nativeLog(JNIEnv *env, jclass cls, jint level, jstring id, jstring log)
{
	int error;
    jboolean iscopy1;
    const char *textString1 = (*env)->GetStringUTFChars(
                env, id, &iscopy1);
    jboolean iscopy2;
    const char *textString2 = (*env)->GetStringUTFChars(
                env, log, &iscopy2);
	if ((error = CPhidget_log(level, (char *)textString1, (char *)textString2)))
		PH_THROW(error);
	(*env)->ReleaseStringUTFChars(env, id, textString1);
	(*env)->ReleaseStringUTFChars(env, log, textString2);
}

JNIEXPORT jstring JNICALL
Java_com_phidgets_Phidget_getLibraryVersion(JNIEnv *env, jclass cls)
{
	const char *ver;

	CPhidget_getLibraryVersion(&ver);

	return (*env)->NewStringUTF(env, ver);
}

JNIEXPORT jboolean JNICALL
Java_com_phidgets_Phidget_isAttached(JNIEnv *env, jobject obj)
{
	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)(*env)->GetLongField(env,
	    obj, handle_fid);
	int error;
	int s;
	
	if ((error = CPhidget_getDeviceStatus(h, &s)))
		PH_THROW(error);

	return s == PHIDGET_ATTACHED;
}

JNIEXPORT jboolean JNICALL
Java_com_phidgets_Phidget_isAttachedToServer(JNIEnv *env, jobject obj)
{
	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)(*env)->GetLongField(env,
	    obj, handle_fid);
	int error;
	int s;
	
	if ((error = CPhidget_getServerStatus(h, &s)))
		PH_THROW(error);

	return s == PHIDGET_ATTACHED;
}

JNIEXPORT void JNICALL 
Java_com_phidgets_Phidget_waitForAttachment(JNIEnv *env, jobject obj, jint timeout)
{
	CPhidgetHandle h = (CPhidgetHandle)(uintptr_t)(*env)->GetLongField(env,
	    obj, handle_fid);
	int error;
	
	if ((error = CPhidget_waitForAttachment(h, timeout)))
		PH_THROW(error);

	return;
}
