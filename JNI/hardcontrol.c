
#include <jni.h>  /* /usr/lib/jvm/java-1.7.0-openjdk-amd64/include/ */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <android/log.h>  /* liblog */

 
#if 0
typedef struct {
    char *name;          /* Java里调用的函数名 */
    char *signature;    /* JNI字段描述符, 用来表示Java里调用的函数的参数和返回值类型 */
    void *fnPtr;          /* C语言实现的本地函数 */
} JNINativeMethod;
#endif

static jint fd;

jint adcOpen(JNIEnv *env, jobject cls)
{
	fd = open("/dev/myAdc", O_RDWR);
	__android_log_print(ANDROID_LOG_DEBUG, "AdcDemo", "native adcOpen : %d", fd);
	if (fd >= 0)
		return 0;
	else
		return -1;
}


jstring adcRead(JNIEnv *env, jobject cls)
{
	int val1 = 0;
	char buf[4] = {0};
	__android_log_print(ANDROID_LOG_DEBUG, "AdcDemo", "enter read");
	int ret = read(fd, &val1, sizeof(val1));
	__android_log_print(ANDROID_LOG_DEBUG, "AdcDemo", "native adcCtrl : %d", val1);
	//返回数据给val
	sprintf(buf,"%d",val1);
	return (*env)->NewStringUTF(env, buf);
}


static const JNINativeMethod methods[] = {
	{"adcOpen", "()I", (void *)adcOpen},
	{"adcRead", "()Ljava/lang/String;", (void *)adcRead},
};




/* System.loadLibrary */
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *jvm, void *reserved)
{
	JNIEnv *env;
	jclass cls;

	if ((*jvm)->GetEnv(jvm, (void **)&env, JNI_VERSION_1_4)) {
		return JNI_ERR; /* JNI version not supported */
	}
	cls = (*env)->FindClass(env, "com/qu/myadc/HardControl");
	if (cls == NULL) {
		return JNI_ERR;
	}

	/* 2. map java hello <-->c c_hello */
	if ((*env)->RegisterNatives(env, cls, methods, sizeof(methods)/sizeof(methods[0])) < 0)
		return JNI_ERR;

	return JNI_VERSION_1_4;
}

