#include <entityx/Entity.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <RakPeerInterface.h>
#include <BitStream.h>
#include <MessageIdentifiers.h>
#include <NetworkIDManager.h>
#include <GetTime.h>

#include "renderer/window.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "input/input.h"
#include "input/fp_camera_controller.h"
#include "components.h"
#include "transform.h"
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
#include "network/network_connection.h"
#include "network/replica_manager.h"
#include "network/network_defines.h"

using namespace RakNet;

int main(void) {

    NetworkConnection net_conn;
    StartupResult raknet_startup = net_conn.Startup();
    if (raknet_startup != RAKNET_STARTED) {
        Log(Error, "Failed to start RakNet, error: 0x%x", raknet_startup);
        exit(1);
    }

    entityx::EventManager events;
    entityx::EntityManager entities(events);

    uint32_t current_tick = 0;

    ReplicaManager replica_manager(&entities, current_tick);
    RakNet::NetworkIDManager network_id_manager;
    net_conn.peer->AttachPlugin(&replica_manager);
    replica_manager.SetNetworkIDManager(&network_id_manager);

    ConnectionAttemptResult conn_attempt = net_conn.Connect("localhost", 51851);
    if (conn_attempt != CONNECTION_ATTEMPT_STARTED) {
        Log(Error, "Failed to start connection attempt, error: 0x%x", conn_attempt);
        exit(1);
    }

    TimeMS server_start_time;
    bool received_time = false;
    while (!received_time) {
        for (Packet* packet = net_conn.peer->Receive(); packet; net_conn.peer->DeallocatePacket(packet), packet = net_conn.peer->Receive()) {
            if (packet->data[0] == ID_START_TIME) {
                if (packet->length == sizeof(MessageID) + sizeof(TimeMS))
                {
                    BitStream ts_bs(packet->data + sizeof(MessageID), packet->length - 1, false);
                    ts_bs.Read(server_start_time);

                    received_time = true;
                    break;
                }
            }
        }
    }

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
    point_light->radius = 20;

    MeshRenderer mesh_renderer;
    Shader shader;
    shader.Init("basic.glsl");

    Shader shadow_pass;
    shadow_pass.Init("shadow_pass.glsl");

    TextureLoader texture_loader;

    entityx::Entity ent = entities.create();
    Transform* monkey_transform = ent.assign<Transform>().get();
    ent.assign<MeshComponent>(mesh_loader.GetMesh("suzanne.obj"), Material(0.1f, 0.9f), &shader, texture_loader.GetTexture2d("suzanne.png"));
    mesh_renderer.RegisterEntity(ent);

    ent = entities.create();
    Transform* transform = ent.assign<Transform>().get();
    transform->pos = glm::vec3(0.0f, 10.0f, 0.0f);
    ent.assign<MeshComponent>(mesh_loader.GetMesh("torus.obj"), Material(0.4f, 0.2f), &shader, texture_loader.GetTexture2d("mona_lisa.png"));
    mesh_renderer.RegisterEntity(ent);

    point_light->transform = transform;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ent = entities.create();
            transform = ent.assign<Transform>().get();
            transform->pos = glm::vec3(i * 1.5f  - 6, 0, j * 1.5f - 6);
            ent.assign<MeshComponent>(mesh_loader.GetMesh("smooth_sphere.obj"), Material((i + 1) / 8.0f, j / 7.0f), &shader, texture_loader.GetTexture2d("yellow.png"));
            mesh_renderer.RegisterEntity(ent);
        }
    }

    ent = entities.create();
    transform = ent.assign<Transform>().get();
    transform->pos.y = -2.5f;
    ent.assign<MeshComponent>(mesh_loader.GetMesh("plane.obj"), Material(0.9f, 0.9f), &shader, texture_loader.GetTexture2d("suzanne.png"));
    mesh_renderer.RegisterEntity(ent);


    Camera camera;
    camera.InitPerspective(1280, 720, 60);
    camera.transform.pos= glm::vec3(0.f, 0.f, 5.f);
    window.resizeListeners.push_back(&camera);
    glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Camera shadow_camera;
    shadow_camera.InitOrtho(16, 16, -10, 10);
    shadow_camera.transform.orientation = glm::rotate(shadow_camera.transform.orientation, glm::radians(90.0f), glm::vec3(1, 0, 0));
    shadow_camera.UpdateMatrix(nullptr);


    ShadowMap shadow_map;
    shadow_map.Init(1024, 1024);

    ImGui_ImplGlfwGL3_Init(window.GetWindow(), true);

    FPCameraController camController(&camera);
    window.GetInput()->AddListener(&camController);
    ImGuiListener guiListener;
    window.GetInput()->AddListener(&guiListener);

    TimeMS game_time = GetTimeMS(), last_update_time = 0;
    float dt, accumulated_ticks = 0;

    Time clock_diff = net_conn.peer->GetClockDifferential(net_conn.peer->GetGUIDFromIndex(0));
    current_tick = (uint32_t)((GetTimeMS() - server_start_time - clock_diff) / TICK_LENGTH_MS);

    bool show_entity_editor = true;
    while (!window.ShouldClose()) {
        dt = (float) (GetTimeMS() - game_time);
        game_time = GetTimeMS();

        window.UpdateInput();

        accumulated_ticks += dt / TICK_LENGTH_MS;
        int ticks_processed = 0;
        while (accumulated_ticks >= 1 && ticks_processed++ < 10) {

            // sync ticks with server
            Time clock_diff = net_conn.peer->GetClockDifferential(net_conn.peer->GetGUIDFromIndex(0));
            uint32_t server_tick = (uint32_t)((GetTimeMS() - server_start_time - clock_diff) / TICK_LENGTH_MS);
            accumulated_ticks += ((float) server_tick - current_tick) / 10.0f;

            Packet* packet;
            for (packet = net_conn.peer->Receive(); packet; net_conn.peer->DeallocatePacket(packet), packet = net_conn.peer->Receive())
            {
            }

            accumulated_ticks--;
            current_tick++;
        }

        // just a hack until proper entity factories are made
        // sets the monkey's position to the last created entity which is the networked one
        int count = 0;
        for (entityx::Entity entity : entities.entities_for_debugging()) {
            count++;
            if (count == 68) {
                float integral_part;
                entity.component<TransformHistoryComponent>().get()->ReadState(current_tick + modf(accumulated_ticks, &integral_part) - 4, monkey_transform->pos, monkey_transform->orientation);
            }
        }

        camController.Update(dt);

        ImGui_ImplGlfwGL3_NewFrame();
        ShowEntityEditor(&show_entity_editor, &camera, &entities);

        camera.UpdateMatrix(nullptr);

        glViewport(0, 0, window.width, window.height);
        g_buffer.Draw();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mesh_renderer.Render(dt, camera, shadow_camera);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        g_buffer.Read();
        light_system.LightPass(&camera);

        ImGui::Render();
        window.SwapBuffers();
    }

    net_conn.peer->Shutdown(300);
    window.GetInput()->ClearListeners();
}
