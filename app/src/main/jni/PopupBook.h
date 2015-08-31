//
// Created by ST20384 on 15/08/31.
//

#ifndef NDKSAMPLE_POPUPBOOK_H
#define NDKSAMPLE_POPUPBOOK_H

#include "BasePopupBook.h"
#include <EGL/egl.h>

class PopupBook : public BasePopupBook {

public:
    PopupBook();
    virtual ~PopupBook();
    bool init();

private:
    enum {VB_INSTANCE, VB_SCALEROT, VB_OFFSET, VB_COUNT};

    virtual float* mapOffsetBuf();
    virtual void unmapOffsetBuf();
    virtual float* mapTransformBuf();
    virtual void unmapTransformBuf();
    virtual void draw(unsigned int numInstances);

    const EGLContext mEglContext;
    GLuint mProgram;
    GLuint mVB[VB_COUNT];
    GLuint mVBState;

};

#endif //NDKSAMPLE_POPUPBOOK_H
