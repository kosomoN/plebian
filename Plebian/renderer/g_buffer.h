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

    virtual void WindowResized(int width, int height);

    int width = 0, height = 0;
private:
    GLuint fbo = 0;
    GLuint textures[NUM_TEXTURES];
};

#endif // DEFERRED_RENDERER_H_
