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
#include "renderer/blur.h"
#include "log.h"
#include "network/network_connection.h"
#include "network/replica_manager.h"
#include "network/network_defines.h"
#include "plebian_game.h"

using namespace RakNet;

int main(void) {
    PlebianGame game;
    MeshLoader mesh_loader;
    TextureLoader texture_loader;
    MeshRenderer mesh_renderer;
    game.mesh_loader = &mesh_loader;
    game.texture_loader = &texture_loader;
    game.mesh_renderer = &mesh_renderer;

    Window window;
    if (!window.Init(1280, 720)) {
        Log(Error, "Failed to initialize window");
        exit(1);
    }

    game.Init();

    NetworkConnection net_conn;
    StartupResult raknet_startup = net_conn.Startup();
    if (raknet_startup != RAKNET_STARTED) {
        Log(Error, "Failed to start RakNet, error: 0x%x", raknet_startup);
        exit(1);
    }

    ReplicaManager replica_manager(&game);
    RakNet::NetworkIDManager network_id_manager;
    net_conn.peer->AttachPlugin(&replica_manager);
    replica_manager.SetNetworkIDManager(&network_id_manager);

    ConnectionAttemptResult conn_attempt = net_conn.Connect("localhost", 51851);
    if (conn_attempt != CONNECTION_ATTEMPT_STARTED) {
        Log(Error, "Failed to start connection attempt, error: 0x%x", conn_attempt);
        exit(1);
    }

    TimeMS server_start_time = 0;
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

    window.SetInput(new Input);
    window.GetInput()->Init(window.GetWindow());

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    GBuffer g_buffer;
    if (!g_buffer.Init(window.width, window.width)) {
        Log(Error, "Failed to initialize geometry buffer");
        exit(1);
    }
    window.resizeListeners.push_back(&g_buffer);

    LightSystem light_system;
    if (!light_system.Init(game.mesh_loader)) {
        Log(Error, "Failed to initialize light system");
        exit(1);
    }
    game.light_system = &light_system;

    PointLight* point_light = light_system.CreatePointLight();
    point_light->intensity = glm::vec3(10.0f, 10.0f, 10.f);
    point_light->radius = 20;

    Shader shadow_pass;
    shadow_pass.Init("shadow_pass.glsl");

    entityx::Entity ent = game.entity_manager.create();
    Transform* monkey_transform = ent.assign<Transform>().get();
    ent.assign<MeshComponent>(game.mesh_loader->GetMesh("suzanne.obj"), Material(0.1f, 0.9f), texture_loader.GetTexture2d("suzanne.png"));
    mesh_renderer.RegisterEntity(ent);

    ent = game.entity_manager.create();
    Transform* transform = ent.assign<Transform>().get();
    transform->pos = glm::vec3(0.0f, 10.0f, 0.0f);
    ent.assign<MeshComponent>(game.mesh_loader->GetMesh("torus.obj"), Material(0.4f, 0.2f), texture_loader.GetTexture2d("mona_lisa.png"));
    mesh_renderer.RegisterEntity(ent);

    point_light->transform = transform;

    ent = game.entity_manager.create();
    transform = ent.assign<Transform>().get();
    transform->pos.y = -2.5f;
    ent.assign<MeshComponent>(game.mesh_loader->GetMesh("plane.obj"), Material(0.9f, 0.9f), texture_loader.GetTexture2d("suzanne.png"));
    mesh_renderer.RegisterEntity(ent);

    Camera camera;
    camera.InitPerspective(window.width, window.height, 60.f);
    camera.transform.pos = glm::vec3(0.f, 3.f, 5.f);
    window.resizeListeners.push_back(&camera);
    glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    Blur blur;
    blur.Init(window.width, window.height);

    double time = glfwGetTime();
    float delta;

    bool show_entity_editor = true;
    while (!window.ShouldClose()) {
        dt = (float) (GetTimeMS() - game_time);
        game_time = GetTimeMS();

        window.UpdateInput();

        accumulated_ticks += dt / TICK_LENGTH_MS;
        int ticks_processed = 0;
        while (accumulated_ticks >= 1) {
            if (ticks_processed++ > 10) {
                Log(Warn, "Can't keep up with the tick rate!");
                // reset accumulated ticks to zero
                game.current_tick += (uint32_t)accumulated_ticks;
                accumulated_ticks = accumulated_ticks - floor(accumulated_ticks);
            }

            accumulated_ticks--;
            game.current_tick++;
        }
        game.current_tick_fraction = accumulated_ticks;

        Packet* packet;
        for (packet = net_conn.peer->Receive(); packet; net_conn.peer->DeallocatePacket(packet), packet = net_conn.peer->Receive())
        {
        }

        // update time for rendering
        game.UpdateServerTime();

        // interpolate transforms
        game.entity_manager.each<Transform, TransformHistoryComponent>([&game](entityx::Entity entity, Transform& transform, TransformHistoryComponent& hist) {
            hist.ReadState(game.server_time, transform.pos, transform.orientation);
        });

        camController.Update(dt);

        game.RenderFrame(camera, 0, &g_buffer);

        ImGui_ImplGlfwGL3_NewFrame();
        ShowEntityEditor(&show_entity_editor, &camera, &game.entity_manager);

        camera.UpdateMatrix();

        ImGui::Render();
        window.SwapBuffers();
    }

    net_conn.peer->Shutdown(300);
    window.GetInput()->ClearListeners();
}
