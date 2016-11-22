#ifndef BLUR_H_
#define BLUR_H_

#include <GL/glew.h>

#include "window.h"
#include "shader.h"

class Blur : public WinResizeListener {
public:
    bool Init(int width, int height);
    void Draw();
    void DrawQuad();

    virtual void WindowResized(int width, int height);
private:
    GLuint vao, vbo;
    GLuint tex_buffer;
    GLuint pingpong_fbo[2];
    GLuint pingpong_textures[2];
    Shader blur_pass;
    Shader simple;

};
#endif // BLUR_H_
