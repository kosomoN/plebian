#include "light_system.h"

#include <glm/gtc/type_ptr.hpp>

#include "log.h"
#include "renderer/g_buffer.h"
#include <io/textureloader.h>

bool LightSystem::Init(MeshLoader* mesh_loader)
{
	std::string** textures;
	textures = new std::string*[1];
	textures[0] = new std::string[6];
	textures[0][0] = "posx.png";
	textures[0][1] = "negx.png";
	textures[0][2] = "posy.png";
	textures[0][3] = "negy.png";
	textures[0][4] = "posz.png";
	textures[0][5] = "negz.png";
	//cubemap = TextureLoader().GetTexture3d(textures, 1);
    sphere_mesh = mesh_loader->GetMesh("sphere.obj");
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

    return true;
}

void LightSystem::LightPass(Camera* camera)
{
    glDisable(GL_CULL_FACE);

    glUseProgram(light_shader.shader_program);
    glUniform2f(glGetUniformLocation(light_shader.shader_program, "screen_size"), camera->GetWidth(), camera->GetHeight());
    glUniform3fv(glGetUniformLocation(light_shader.shader_program, "cam_pos"), 1, glm::value_ptr(camera->transform.pos));

    glBindVertexArray(sphere_mesh->vertex_array_object);
	glActiveTexture(GL_TEXTURE22);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->texture_id);
	glUniform1i(glGetUniformLocation(light_shader.shader_program, "cubemap"), 22);

    for (PointLight& l : point_lights) {
        glm::mat4 world_mat = l.transform->WorldSpace();
        glm::mat4 mvp = camera->combined * world_mat * glm::scale(glm::mat4(), glm::vec3(l.radius));
        glUniformMatrix4fv(glGetUniformLocation(light_shader.shader_program, "MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
        glUniform3fv(glGetUniformLocation(light_shader.shader_program, "light_pos"), 1, glm::value_ptr(world_mat * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
        glUniform3fv(glGetUniformLocation(light_shader.shader_program, "light_intensity"), 1, glm::value_ptr(l.intensity));
        glUniform1f(glGetUniformLocation(light_shader.shader_program, "inv_sqr_radius"), 1.0f / (l.radius * l.radius));

        glDrawElements(GL_TRIANGLES, sphere_mesh->num_indices, GL_UNSIGNED_INT, 0);
    }
    
    glBindVertexArray(0);

    glEnable(GL_CULL_FACE);
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
