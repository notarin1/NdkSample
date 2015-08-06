//
// Created by notar_000 on 2015/08/05.
//

#include "hello.h"

CHello::CHello()  {
    g_vm = NULL;
    __android_log_print(ANDROID_LOG_INFO, "CHello", "constructor");
}

CHello::~CHello() {
    __android_log_print(ANDROID_LOG_INFO, "CHello", "destructor");
}

jstring CHello::createMessage(JNIEnv* env,jobject thiz) {
    return env->NewStringUTF("Hello from C++");
}