#ifndef DEFERRED_RENDERER_H_
#define DEFERRED_RENDERER_H_

#include <GL/glew.h>

#include "window.h"

class GBuffer : public WinResizeListener {
public:
    enum Textures {
        tex_position = 0,
        tex_diffuse,
        tex_normal,
        tex_depth,
        NUM_TEXTURES
    };

    ~GBuffer();

    bool Init(int width, int height);

    void Draw();
    void Read();
    void ReadOutput();

    virtual void WindowResized(int width, int height);
private:
    GLuint fbo = 0;
    GLuint textures[NUM_TEXTURES];
    GLuint tex_output;
};

#endif // DEFERRED_RENDERER_H_
