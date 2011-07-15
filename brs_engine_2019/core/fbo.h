#pragma once

#include "../globals.h"

class FBOManager
{
public:

    FBOManager();
    ~FBOManager();

    void init();
    void release();

    void bindDepthFBO();
    void bindTextureFBO2048(GLuint textureID);
    void bindTextureFBO1024(GLuint textureID);
    void bindTextureFBO512(GLuint textureID);
    void bindTextureFBO256(GLuint textureID);
    void bindTextureFBO128(GLuint textureID);
    void bindTextureFBO64(GLuint textureID);
    void bindTextureFBO32(GLuint textureID);
    void unbindFBO();

private:
    bool logFBOErrors();
    bool logFBOErrors(string kohta);

    GLuint m_frameBufferTexture2048;
    GLuint m_frameBufferTexture1024;
    GLuint m_frameBufferTexture512;
    GLuint m_frameBufferTexture256;
    GLuint m_frameBufferTexture128;
    GLuint m_frameBufferTexture64;
    GLuint m_frameBufferTexture32;

    GLuint m_depthRenderBufferTexture2048;
    GLuint m_depthRenderBufferTexture1024;
    GLuint m_depthRenderBufferTexture512;
    GLuint m_depthRenderBufferTexture256;
    GLuint m_depthRenderBufferTexture128;
    GLuint m_depthRenderBufferTexture64;
    GLuint m_depthRenderBufferTexture32;

    GLuint m_depthFBO;
    GLuint m_depthColTex;
    GLuint m_depthTex;
};

