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
};

#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT jstring JNICALL Java_com_example_st20384_ndksample_MainActivity_createMessage(JNIEnv* env, jobject thiz) {
        CHello* p_hello;
        jstring ret;

        p_hello = new CHello();
        ret = p_hello->createMessage(env, thiz);
        delete p_hello;

        return ret;
    }

#ifdef __cplusplus
}
#endif

#endif //NDKSAMPLE_HELLO_H
