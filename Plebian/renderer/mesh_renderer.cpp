#include "mesh_renderer.h"

#include <algorithm>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

// TODO implement proper light system
void MeshRenderer::Render(float delta, Camera& cam, Camera& light_camera, GLuint shader_override) {
    // matrix to transform range [-1,1] to [0,1]
     glm::mat4 biasMatrix(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
            );

    current_tex = 0;
    current_shader = 0;

    if (shader_override) {
        glUseProgram(shader_override);
        current_shader = shader_override;
    }

    for (entityx::Entity e : entities) {
        Transform* transform = e.component<Transform>().get();
        MeshComponent* meshComponent = e.component<MeshComponent>().get();
        Mesh* mesh = meshComponent->mesh;

        if (!mesh) continue;

        if (!shader_override && current_shader != meshComponent->shader->m_shader_program) {
            current_shader = meshComponent->shader->m_shader_program;
            glUseProgram(current_shader);
        }

        if (meshComponent->texture &&
                current_tex != meshComponent->texture->texture_id) {

            glActiveTexture(GL_TEXTURE0);
            current_tex = meshComponent->texture->texture_id;
            glBindTexture(GL_TEXTURE_2D, current_tex);
        }

        glm::mat4 worldMat = transform->WorldSpace();
        glm::mat4 mvp = cam.combined * worldMat;
        glm::mat4 lightMVP = biasMatrix * light_camera.combined * worldMat;

        glUniformMatrix4fv(glGetUniformLocation(current_shader, "MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
        glUniformMatrix4fv(glGetUniformLocation(current_shader, "worldMat"), 1, GL_FALSE, glm::value_ptr(worldMat));
        glUniformMatrix4fv(glGetUniformLocation(current_shader, "lightMVP"), 1, GL_FALSE, glm::value_ptr(lightMVP));
		glUniform3fv(glGetUniformLocation(current_shader, "camPos"), 1, glm::value_ptr(cam.position));

        glBindVertexArray(mesh->vertex_array_object);
        glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}

void MeshRenderer::RegisterEntity(entityx::Entity entity) {
    entities.push_back(entity);
}

void MeshRenderer::UnregisterEntity(entityx::Entity entity) {
    entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
}

