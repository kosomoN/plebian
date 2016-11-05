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
#include "renderer/g_buffer.h"
#include "renderer/light_system.h"
#include "log.h"

int main(void) {
    Window window;
    if (!window.Init(1280, 720)) {
        Log(Error, "Failed to initialize window");
        exit(1);
    }
    window.SetInput(new Input);
    window.GetInput()->Init(window.GetWindow());

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GBuffer g_buffer;
    if (!g_buffer.Init(1280, 720)) {
        Log(Error, "Failed to initialize geometry buffer");
        exit(1);
    }
    window.resizeListeners.push_back(&g_buffer);

    MeshLoader mesh_loader;

    LightSystem light_system;
    if (!light_system.Init(mesh_loader, window.width, window.height)) {
        Log(Error, "Failed to initialize light system");
        exit(1);
    }
    window.resizeListeners.push_back(&light_system);

    PointLight* point_light = light_system.CreatePointLight();
    point_light->intensity = glm::vec3(10.0f, 10.0f, 10.f);
    point_light->SetAttenuation(glm::vec3(0.1f, 0.3f, 0.6f));

    MeshRenderer mesh_renderer;
    Shader shader;
    shader.Init("basic.glsl");

    Shader shadow_pass;
    shadow_pass.Init("shadow_pass.glsl");

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
    transform->pos = glm::vec3(-2.0f, 1.0f, 2.5f);
    ent.assign<MeshComponent>(mesh_loader.GetMesh("torus.obj"), &shader, texture_loader.GetTexture("mona_lisa.png"));
    mesh_renderer.RegisterEntity(ent);

    point_light->transform = transform;

    ent = entities.create();
    transform = ent.assign<Transform>().get();
    transform->parent = monkey_transform;
    transform->pos.y = -2.5f;
    ent.assign<MeshComponent>(mesh_loader.GetMesh("plane.obj"), &shader, texture_loader.GetTexture("suzanne.png"));
    mesh_renderer.RegisterEntity(ent);


    Camera camera;
    camera.InitPerspective(1280, 720, 60);
    camera.transform.pos= glm::vec3(0.f, 0.f, 5.f);
    window.resizeListeners.push_back(&camera);
    glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Camera shadow_camera;
    shadow_camera.InitOrtho(16, 16, -10, 10);
    shadow_camera.transform.orientation = glm::rotate(shadow_camera.transform.orientation, glm::radians(90.0f), glm::vec3(1, 0, 0));
    shadow_camera.UpdateMatrix();

    ShadowMap shadow_map;
    shadow_map.Init(1024, 1024);

    ImGui_ImplGlfwGL3_Init(window.GetWindow(), true);

    FPCameraController camController(&camera);
    window.GetInput()->AddListener(&camController);
    ImGuiListener guiListener;
    window.GetInput()->AddListener(&guiListener);

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

        glViewport(0, 0, window.width, window.height);
        g_buffer.Draw();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mesh_renderer.Render(delta, camera, shadow_camera);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        g_buffer.Read();
        light_system.LightPass(&camera);

        ImGui::Render();
        window.SwapBuffers();
    }

    window.GetInput()->ClearListeners();
}
