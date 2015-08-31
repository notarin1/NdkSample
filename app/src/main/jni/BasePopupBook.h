//
// Created by ST20384 on 15/08/31.
//

#ifndef NDKSAMPLE_BASEPOPUPBOOK_H
#define NDKSAMPLE_BASEPOPUPBOOK_H

#include <android/log.h>
#include <math.h>

#if DYNAMIC_ES3
#include "gl3stub.h"
#else
#include <GLES3/gl3.h>
#endif

#define DEBUG 1

#define LOG_TAG "POPUPBOOK"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

#define MAX_INSTANCES_PER_SIDE 16
#define MAX_INSTANCES   (MAX_INSTANCES_PER_SIDE * MAX_INSTANCES_PER_SIDE)
#define TWO_PI          (2.0 * M_PI)
#define MAX_ROT_SPEED   (0.3 * TWO_PI)

struct Vertex {
    GLfloat pos[2];
    GLubyte rgba[4];
};
extern const Vertex QUAD[4];

// returns true if a GL error occurred
extern bool checkGlError(const char* funcName);
extern GLuint createShader(GLenum shaderType, const char* src);
extern GLuint createProgram(const char* vtxSrc, const char* fragSrc);

// ----------------------------------------------------------------------------
// Interface to the ES2 and ES3 renderers, used by JNI code.

class BasePopupBook {
public:
    virtual ~BasePopupBook();
    void resize(int w, int h);
    void render();

protected:
    BasePopupBook();

    // return a pointer to a buffer of MAX_INSTANCES * sizeof(vec2).
    // the buffer is filled with per-instance offsets, then unmapped.
    virtual float* mapOffsetBuf() = 0;
    virtual void unmapOffsetBuf() = 0;
    // return a pointer to a buffer of MAX_INSTANCES * sizeof(vec4).
    // the buffer is filled with per-instance scale and rotation transforms.
    virtual float* mapTransformBuf() = 0;
    virtual void unmapTransformBuf() = 0;

    virtual void draw(unsigned int numInstances) = 0;

private:
    unsigned int mNumInstances;

    void step();
};

//extern BasePopupBook* createES2Renderer();
extern BasePopupBook* createES3Popupbook();

#endif //NDKSAMPLE_BASEPOPUPBOOK_H
