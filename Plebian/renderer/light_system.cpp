#include "light_system.h"

#include <glm/gtc/type_ptr.hpp>

#include "log.h"
#include "renderer/g_buffer.h"

bool LightSystem::Init(MeshLoader mesh_loader, int screen_width, int screen_height)
{
    sphere_mesh = mesh_loader.GetMesh("sphere.obj");
    if (sphere_mesh == nullptr) {
        Log(Error, "Failed to load sphere mesh for light system");
        return false;
    }

    if (!light_shader.Init("light_pass.glsl")) {
        Log(Error, "Failed to load light pass shader");
        return false;
    }

    glUseProgram(light_shader.shader_program);
    glUniform1i(glGetUniformLocation(light_shader.shader_program, "position_tex"), GBuffer::tex_position);
    glUniform1i(glGetUniformLocation(light_shader.shader_program, "diffuse_tex"), GBuffer::tex_diffuse);
    glUniform1i(glGetUniformLocation(light_shader.shader_program, "normal_tex"), GBuffer::tex_normal);

    glUniform2f(glGetUniformLocation(light_shader.shader_program, "screen_size"), (float) screen_width, (float) screen_height);

    return true;
}

void LightSystem::LightPass(Camera* camera)
{
    glUseProgram(light_shader.shader_program);
    glUniform3fv(glGetUniformLocation(light_shader.shader_program, "cam_pos"), 1, glm::value_ptr(camera->position));

    glBindVertexArray(sphere_mesh->vertex_array_object);

    for (PointLight& l : point_lights) {
        glm::mat4 world_mat = l.transform->WorldSpace();
        glm::mat4 mvp = camera->combined * world_mat * glm::scale(glm::mat4(), glm::vec3(l.GetRadius()));
        glUniformMatrix4fv(glGetUniformLocation(light_shader.shader_program, "MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
        glUniform3fv(glGetUniformLocation(light_shader.shader_program, "light_pos"), 1, glm::value_ptr(world_mat * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
        glUniform3fv(glGetUniformLocation(light_shader.shader_program, "light_intensity"), 1, glm::value_ptr(l.intensity));
        glUniform3fv(glGetUniformLocation(light_shader.shader_program, "attenuation"), 1, glm::value_ptr(l.GetAttenuation()));

        glDrawElements(GL_TRIANGLES, sphere_mesh->num_indices, GL_UNSIGNED_INT, 0);
    }
    
    glBindVertexArray(0);
}
    

PointLight* LightSystem::CreatePointLight()
{
    point_lights.push_back(PointLight());
    return &point_lights.back();
}

DirectionalLight* LightSystem::CreateDirectionalLight()
{
    directional_lights.push_back(DirectionalLight());
    return &directional_lights.back();
}

void LightSystem::WindowResized(int width, int height) {
    glUseProgram(light_shader.shader_program);
    glUniform2f(glGetUniformLocation(light_shader.shader_program, "screen_size"), (float) width, (float) height);
}

