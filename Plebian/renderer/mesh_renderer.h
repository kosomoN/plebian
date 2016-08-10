#ifndef MESH_RENDERER_H_
#define MESH_RENDERER_H_

#include <entityx/entityx.h>
#include "components.h"

class MeshRenderer {
public:
    void Render(float delta);
    void RegisterEntity(entityx::Entity entity);
    void UnregisterEntity(entityx::Entity entity);
private:
    std::vector<entityx::Entity> entities;
    GLuint current_shader = 0,
           current_tex    = 0;
};

#endif MESH_RENDERER_H_
