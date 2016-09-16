#ifndef SHADOW_MAP_H_
#define SHADOW_MAP_H_

#include <GL/glew.h>

class ShadowMap {
public:
    ~ShadowMap();
    bool Init(int width, int height);
    void BindDraw();
    void BindRead(GLenum texture_unit);
private:
    int width = 0, height = 0;
    GLuint fbo = 0;
    GLuint shadow_tex = 0;
};

#endif // SHADOW_MAP_H_
