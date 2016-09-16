#include "renderer/window.h"

#include <entityx/Entity.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "input/input.h"
#include "input/fp_camera_controller.h"
#include "components.h"
#include "io/meshloader.h"
#include "io/textureloader.h"
#include "renderer/mesh_renderer.h"
#include "renderer/shader.h"
#include "renderer/camera.h"
#include "renderer/shadow_map.h"
#include "entity_gui.h"

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

    Shader shadow_pass;
    shadow_pass.Init("shadow_pass.glsl");

    MeshLoader mesh_loader;
    TextureLoader texture_loader;

    entityx::EventManager events;
    entityx::EntityManager entities(events);
    entityx::Entity ent = entities.create();
    Transform* monkey_transform = ent.assign<Transform>().get();
    ent.assign<MeshComponent>(mesh_loader.GetMesh("suzanne.obj"), &shader, texture_loader.GetTexture("suzanne.png"));
    mesh_renderer.RegisterEntity(ent);

    ent = entities.create();
    Transform* transform = ent.assign<Transform>().get();
    transform->parent = monkey_transform;
    ent.assign<MeshComponent>(mesh_loader.GetMesh("torus.obj"), &shader, texture_loader.GetTexture("mona_lisa.png"));
    mesh_renderer.RegisterEntity(ent);

    ent = entities.create();
    transform = ent.assign<Transform>().get();
    transform->parent = monkey_transform;
    transform->pos.y = -2.5f;
    ent.assign<MeshComponent>(mesh_loader.GetMesh("plane.obj"), &shader, texture_loader.GetTexture("suzanne.png"));
    mesh_renderer.RegisterEntity(ent);


    Camera camera(1280, 720, 60);
    camera.position = glm::vec3(0.f, 0.f, 5.f);
    window.resizeListeners.push_back(&camera);

    Camera shadow_camera(16, 16, 90);
    shadow_camera.orientation = glm::rotate(shadow_camera.orientation, glm::radians(90.0f), glm::vec3(1, 0, 0));
    shadow_camera.UpdateMatrix();

    glUseProgram(shader.m_shader_program);
    glUniform1i(glGetUniformLocation(shader.m_shader_program, "shadowMap"), 1);
    glUniformMatrix4fv(glGetUniformLocation(shader.m_shader_program, "lightMVP"), 1,
                       GL_FALSE, glm::value_ptr(shadow_camera.combined));
    glUseProgram(0);

    ShadowMap shadow_map;
    shadow_map.Init(1024, 1024);

    ImGui_ImplGlfwGL3_Init(window.GetWindow(), true);

    ImGuiListener guiListener;
    window.GetInput()->AddListener(&guiListener);
    FPCameraController camController(&camera);
    window.GetInput()->AddListener(&camController);

    double time = glfwGetTime();
    float delta;

    bool show_entity_editor = true;
    while (!window.ShouldClose()) {
        delta = (float) (glfwGetTime() - time);
        time = glfwGetTime();

        window.UpdateInput();
        ImGui_ImplGlfwGL3_NewFrame();

        ShowEntityEditor(&show_entity_editor, &camera, &entities);

        camController.Update(delta);
        camera.UpdateMatrix();

        shadow_map.BindDraw();
        glClear(GL_DEPTH_BUFFER_BIT);
        mesh_renderer.Render(delta, shadow_camera, shadow_camera, shadow_pass.m_shader_program);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, window.width, window.height);
        shadow_map.BindRead(GL_TEXTURE1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mesh_renderer.Render(delta, camera, shadow_camera);
        ImGui::Render();
        window.SwapBuffers();
    }

    window.GetInput()->ClearListeners();
}
