#ifndef LIGHT_SYSTEM_H_ 
#define LIGHT_SYSTEM_H_

#include <vector>

#include "window.h"
#include "renderer/camera.h"
#include "renderer/shader.h"
#include "renderer/lights.h"
#include "renderer/mesh.h"
#include "io/meshloader.h"

class LightSystem : public WinResizeListener {
public:
    bool Init(MeshLoader mesh_loader, int screen_width, int screen_height);

    void LightPass(Camera* camera);

    PointLight* CreatePointLight();
    DirectionalLight* CreateDirectionalLight();

    virtual void WindowResized(int width, int height);
private:
    std::vector<PointLight> point_lights;
    std::vector<DirectionalLight> directional_lights;

    Mesh* sphere_mesh;
    Shader light_shader;
};

#endif // LIGHT_SYSTEM_H_
