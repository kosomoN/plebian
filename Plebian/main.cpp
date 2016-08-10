#include "renderer/window.h"

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
    ent.assign<Transform>();
    ent.assign<MeshComponent>(mesh_loader.GetMesh("suzanne.obj"), &shader, texture_loader.GetTexture("suzanne.png"));
    mesh_renderer.RegisterEntity(ent);

    ent = entities.create();
    ent.assign<Transform>();
    ent.assign<MeshComponent>(mesh_loader.GetMesh("torus.obj"), &shader, texture_loader.GetTexture("mona_lisa.png"));
    mesh_renderer.RegisterEntity(ent);


    Camera camera(1280, 720, 60);
    window.resizeListeners.push_back(&camera);
    camera.viewMatrix = glm::lookAt(glm::vec3(0, 1, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    camera.UpdateMatrix();
    glUseProgram(shader.m_shader_program);
    glUniformMatrix4fv(glGetUniformLocation(shader.m_shader_program, "MVP"), 1, GL_FALSE, glm::value_ptr(camera.GetCombined()));

    double time;
    while (!window.ShouldClose()) {
        time = glfwGetTime();
        window.UpdateInput();
        camera.viewMatrix = glm::lookAt(glm::vec3(cos(time) * 5, cos(time * 0.3) * 3, sin(time) * 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        camera.UpdateMatrix();
        glUseProgram(shader.m_shader_program);
        glUniformMatrix4fv(glGetUniformLocation(shader.m_shader_program, "MVP"), 1, GL_FALSE, glm::value_ptr(camera.GetCombined()));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mesh_renderer.Render(0.0f);
        window.SwapBuffers();
    }
}
