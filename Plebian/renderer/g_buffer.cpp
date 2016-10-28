#include "g_buffer.h"

#include "log.h"

GBuffer::~GBuffer()
{
    for (int i = 0; i < NUM_TEXTURES; i++) {
        if (textures[i] != 0)
            glDeleteTextures(1, &textures[i]);
    }

    if (fbo)
        glDeleteFramebuffers(1, &fbo);
}

bool GBuffer::Init(int width_, int height_)
{
    width = width_;
    height = height_;

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(NUM_TEXTURES, textures);

    for (int i = 0; i < NUM_TEXTURES; i++) {
        if (i == tex_depth) continue;

        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);
    }

    glGenTextures(1, &textures[tex_depth]);
    glBindTexture(GL_TEXTURE_2D, textures[tex_depth]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[tex_depth], 0);

    GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(NUM_TEXTURES, draw_buffers);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        Log(Error, "Framebuffer error: 0x%x", status);
        return false;
    }

    Log(Debug, "Framebuffer created");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

void GBuffer::Draw()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void GBuffer::Read()
{
    for (unsigned int i = 0; i < NUM_TEXTURES; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
}

void GBuffer::WindowResized(int width, int height)
{
    for (int i = 0; i < NUM_TEXTURES; i++) {
        if (textures[i] != 0)
            glDeleteTextures(1, &textures[i]);
    }

    if (fbo)
        glDeleteFramebuffers(1, &fbo);

    Init(width, height);
}
