#include "mesh_renderer.h"

#include <algorithm>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

void MeshRenderer::Render(float delta, Camera& cam) {
    for (entityx::Entity e : entities) {
        Transform* transform = e.component<Transform>().get();
        MeshComponent* meshComponent = e.component<MeshComponent>().get();
        Mesh* mesh = meshComponent->mesh;

        if (current_shader != meshComponent->shader->m_shader_program) {
            current_shader = meshComponent->shader->m_shader_program;
            glUseProgram(current_shader);
        }

        if (meshComponent->texture &&
                current_tex != meshComponent->texture->texture_id) {

            glActiveTexture(GL_TEXTURE0);
            current_tex = meshComponent->texture->texture_id;
            glBindTexture(GL_TEXTURE_2D, current_tex);
        }

        glm::mat4 worldMat =  glm::translate(glm::mat4(), transform->pos) * glm::mat4_cast(transform->orientation);
        glm::mat4 mvp = cam.GetCombined() * worldMat;
        glUniformMatrix4fv(glGetUniformLocation(current_shader, "MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
        glUniformMatrix4fv(glGetUniformLocation(current_shader, "worldMat"), 1, GL_FALSE, glm::value_ptr(worldMat));

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

