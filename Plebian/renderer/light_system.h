#ifndef LIGHT_SYSTEM_H_ 
#define LIGHT_SYSTEM_H_

#include <vector>

#include "window.h"
#include "renderer/camera.h"
#include "renderer/shader.h"
#include "renderer/lights.h"
#include "renderer/mesh.h"
#include "io/meshloader.h"

class LightSystem {
public:
    bool Init(MeshLoader* mesh_loader);

    void LightPass(Camera* camera);

    PointLight* CreatePointLight();
    DirectionalLight* CreateDirectionalLight();
private:
    std::vector<PointLight> point_lights;
    std::vector<DirectionalLight> directional_lights;

    Mesh* sphere_mesh;
    Shader light_shader;
	Texture* cubemap;
};

#endif // LIGHT_SYSTEM_H_
