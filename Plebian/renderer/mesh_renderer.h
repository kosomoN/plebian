#ifndef MESH_RENDERER_H_
#define MESH_RENDERER_H_

#include <entityx/entityx.h>
#include "components.h"
#include "transform.h"
#include "renderer/camera.h"

class MeshRenderer {
public:
    void Render(Camera& camera, GLuint shader);
    void RegisterEntity(entityx::Entity entity);
    void UnregisterEntity(entityx::Entity entity);
private:
    std::vector<entityx::Entity> entities;
    GLuint current_tex = 0;
};

#endif MESH_RENDERER_H_
