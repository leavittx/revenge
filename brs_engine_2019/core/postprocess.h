#pragma once


/*
 TODO: Implement a stack-based thing

*/
#include "../globals.h"

enum RenderTarget
{
    RENDERTARGET_0 = 0,
    RENDERTARGET_1,
    RENDERTARGET_2,
    RENDERTARGET_3,
};

class PostProcess
{
public:
    PostProcess();
    ~PostProcess();

    //set up render target
    void init(RenderTarget target, bool clear = true);
    void unInit();
    void clearTexture(RenderTarget target);

    //all the postproc filters
    void glow(RenderTarget source, int steps = 5, float xstep = 0.006f, float ystep = 0.006f, float bluralpha = 0.92f, float glowexp = 0.0f, bool combine = true);
    void blur(RenderTarget source, int steps = 5, float xstep = 0.015f, float ystep = 0.015f, float bluralpha = 0.90f, float mixalpha = 1.0f);
    void pixelize(RenderTarget source, Color4& color, int blendmode);
    void invert(RenderTarget source, float blendfactor = 1.0f);

    private:
        string getRenderTargetName(RenderTarget target);


};


