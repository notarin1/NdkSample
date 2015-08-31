//
// Created by ST20384 on 15/08/31.
//

#include <jni.h>
#include <stdlib.h>
#include <time.h>

#include "BasePopupBook.h"

bool checkGlError(const char* funcName) {
    GLint err = glGetError();
    if (err != GL_NO_ERROR) {
        ALOGE("GL error after %s(): 0x%08x\n", funcName, err);
        return true;
    }
    return false;
}

GLuint createShader(GLenum shaderType, const char* src) {
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        checkGlError("glCreateShader");
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);

    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen > 0) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
                ALOGE("Could not compile %s shader:\n%s\n",
                      shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
                      infoLog);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint createProgram(const char* vtxSrc, const char* fragSrc) {
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    vtxShader = createShader(GL_VERTEX_SHADER, vtxSrc);
    if (!vtxShader)
        goto exit;

    fragShader = createShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!fragShader)
        goto exit;

    program = glCreateProgram();
    if (!program) {
        checkGlError("glCreateProgram");
        goto exit;
    }
    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        ALOGE("Could not link program");
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
                ALOGE("Could not link program:\n%s\n", infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(program);
        program = 0;
    }

    exit:
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);
    return program;
}

static void printGlString(const char* name, GLenum s) {
    const char* v = (const char*)glGetString(s);
    ALOGV("GL %s: %s\n", name, v);
}

// ----------------------------------------------------------------------------
/**
 * Constructor
 */
BasePopupBook::BasePopupBook()
        :mNumInstances(0)
{
}

/**
 * Destructor
 */
BasePopupBook::~BasePopupBook() {
}


void BasePopupBook::resize(int w, int h) {
    glViewport(0, 0, w, h);
}

/**
 * レンダリング
 *
 *
 */
void BasePopupBook::render() {
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw(mNumInstances);
    checkGlError("Renderer::render");
}


// ----------------------------------------------------------------------------

static BasePopupBook *g_popupBook = NULL;

extern "C" {
JNIEXPORT void JNICALL Java_com_android_gles3jni_PopupBookLib_init(JNIEnv *env, jobject obj);
JNIEXPORT void JNICALL Java_com_android_gles3jni_PopupBookLib_resize(JNIEnv *env, jobject obj,
                                                                    jint width, jint height);
JNIEXPORT void JNICALL Java_com_android_gles3jni_PopupBookLib_step(JNIEnv *env, jobject obj);
};


#if !defined(DYNAMIC_ES3)

static GLboolean gl3stubInit() {
    return GL_TRUE;
}
#endif

JNIEXPORT void JNICALL
Java_com_android_gles3jni_PopupBookLib_init(JNIEnv *env, jobject obj) {
    if (g_popupBook) {
        delete g_popupBook;
        g_popupBook = NULL;
    }

    printGlString("Version", GL_VERSION);
    printGlString("Vendor", GL_VENDOR);
    printGlString("Renderer", GL_RENDERER);
    printGlString("Extensions", GL_EXTENSIONS);

    const char *versionStr = (const char *) glGetString(GL_VERSION);
    if (strstr(versionStr, "OpenGL ES 3.") && gl3stubInit()) {
        g_popupBook = createES3Popupbook();
    } else if (strstr(versionStr, "OpenGL ES 2.")) {
//        g_popupBook = createES2Popupbook();
    } else {
        ALOGE("Unsupported OpenGL ES version");
    }
}

JNIEXPORT void JNICALL
Java_com_android_gles3jni_PopupBookLib_resize(JNIEnv *env, jobject obj, jint width, jint height) {
    if (g_popupBook) {
        g_popupBook->resize(width, height);
    }
}

JNIEXPORT void JNICALL
Java_com_android_gles3jni_PopupBookLib_step(JNIEnv *env, jobject obj) {
    if (g_popupBook) {
        g_popupBook->render();
    }
}
