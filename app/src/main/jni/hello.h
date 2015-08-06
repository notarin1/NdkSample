//
// Created by notar_000 on 2015/08/05.
//

#ifndef NDKSAMPLE_HELLO_H
#define NDKSAMPLE_HELLO_H

#include <jni.h>
#include <string.h>
#include <android/log.h>

class CHello {
public:
    CHello();
    ~CHello();
    jstring createMessage(JNIEnv*, jobject);
    JavaVM *g_vm;
};

#ifdef __cplusplus
extern "C" {
#endif
CHello *g_hello;

JNIEXPORT void JNICALL Java_com_example_st20384_ndksample_MainActivity_init(JNIEnv* env, jobject thiz) {
    g_hello = new CHello();
    env->GetJavaVM(&g_hello->g_vm);
}

JNIEXPORT void JNICALL Java_com_example_st20384_ndksample_MainActivity_deinit(JNIEnv* env, jobject thiz) {
    delete g_hello;
}

JNIEXPORT jstring JNICALL Java_com_example_st20384_ndksample_MainActivity_createMessage(JNIEnv* env, jobject thiz) {
        jstring ret;

        ret = g_hello->createMessage(env, thiz);

        return ret;
    }

#ifdef __cplusplus
}
#endif

#endif //NDKSAMPLE_HELLO_H
