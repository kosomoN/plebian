#include "shadow_map.h"

#include "log.h"

bool ShadowMap::Init(int width_, int height_)
{
    width = width_;
    height = height_;

    glGenFramebuffers(1, &fbo);
    glGenTextures(1, &shadow_tex);
    glBindTexture(GL_TEXTURE_2D, shadow_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_tex, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        Log(Error, "Framebuffer error: 0x%x", status);
        return false;
    }

    return true;
}

void ShadowMap::BindDraw()
{
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void ShadowMap::BindRead(GLenum texture_unit)
{
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, shadow_tex);
}

ShadowMap::~ShadowMap()
{
    if (shadow_tex)
        glDeleteTextures(1, &shadow_tex);
    if (fbo)
        glDeleteFramebuffers(1, &fbo);
}
