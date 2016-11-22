#include "blur.h"

#include "log.h"

bool Blur::Init(int width, int height)
{
    // Create VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create VBO
    GLfloat vertices[] = {
        -1.0f, -1.0f, // Lower left
         1.0f, -1.0f, // Lower right
        -1.0f,  1.0f, // Upper left
         1.0f,  1.0f, // Upper right
    };

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load shaders
    simple.Init("simple.glsl");
    blur_pass.Init("blur_pass.glsl");

    // Create FBO's and textures
    glGenFramebuffers(2, pingpong_fbo);
    glGenTextures(2, pingpong_textures);

    for (int i = 0; i < 2; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpong_fbo[i]);
        glBindTexture(GL_TEXTURE_2D, pingpong_textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpong_textures[i], 0);
    }

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        Log(Error, "Framebuffer error: 0x%x", status);
        return false;
    }

    Log(Debug, "Framebuffer created");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

void Blur::Draw() {
    GLboolean horizontal = true;
    glUseProgram(blur_pass.shader_program);

    for (int i = 0; i < 2; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpong_fbo[horizontal]);
        glUniform1i(glGetUniformLocation(blur_pass.shader_program, "horizontal"), 0);

        if (i > 0) {
            glBindTexture(GL_TEXTURE_2D, pingpong_textures[horizontal]);
        }

        DrawQuad();
        horizontal = !horizontal;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(simple.shader_program);

    glBindTexture(GL_TEXTURE_2D, pingpong_textures[horizontal]);
    DrawQuad();
}

void Blur::DrawQuad() {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
};

void Blur::WindowResized(int width, int height)
{
    for (int i = 0; i < 2; i++) {
        if (pingpong_textures[i] != 0)
            glDeleteTextures(1, &pingpong_textures[i]);
        if (pingpong_fbo[i] != 0)
            glDeleteFramebuffers(1, &pingpong_fbo[i]);
    }

    Init(width, height);
}