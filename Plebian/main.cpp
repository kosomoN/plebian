#include "renderer/window.h"
#include "input/input.h"

#include <entityx/Entity.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "components.h"
#include "io/meshloader.h"
#include "io/textureloader.h"
#include "renderer/mesh_renderer.h"
#include "renderer/shader.h"
#include "renderer/camera.h"

int main(void) {
    Window window;
    window.Init(1280, 720);

    window.SetInput(new Input);
    window.GetInput()->Init(window.GetWindow());

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    MeshRenderer mesh_renderer;
    Shader shader;
    shader.Init("basic.glsl");

    MeshLoader mesh_loader;
    TextureLoader texture_loader;

    entityx::EventManager events;
    entityx::EntityManager entities(events);
    entityx::Entity ent = entities.create();
    Transform* transform = ent.assign<Transform>().get();
    transform->pos.x += 0;
    ent.assign<MeshComponent>(mesh_loader.GetMesh("suzanne.obj"), &shader, texture_loader.GetTexture("suzanne.png"));
    mesh_renderer.RegisterEntity(ent);

    ent = entities.create();
    ent.assign<Transform>();
    ent.assign<MeshComponent>(mesh_loader.GetMesh("torus.obj"), &shader, texture_loader.GetTexture("mona_lisa.png"));
    mesh_renderer.RegisterEntity(ent);

    Camera camera(1280, 720, 60);
    window.resizeListeners.push_back(&camera);

    double time;

    while (!window.ShouldClose()) {
        time = glfwGetTime();
        window.UpdateInput();
		camera.position = glm::vec3(sin(time), 0, cos(time)) * 5.f;
		camera.rotation = glm::rotate(glm::quat(), (float)-time, glm::vec3(0, 1, 0));
        camera.UpdateMatrix();
        transform->orientation = glm::rotate(glm::quat(), (float) time, glm::vec3(0, 1, 1));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mesh_renderer.Render(0.0f, camera);
        window.SwapBuffers();
    }

    window.GetInput()->ClearListeners();
}
