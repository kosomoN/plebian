#include "mesh_renderer.h"

#include <algorithm>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

// TODO implement proper light system
void MeshRenderer::Render(Camera& cam, GLuint shader) {
    // matrix to transform range [-1,1] to [0,1]
     glm::mat4 biasMatrix(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
            );

    current_tex = 0;

    glUseProgram(shader);

    for (entityx::Entity e : entities) {
        Transform* transform = e.component<Transform>().get();
        MeshComponent* mesh_component = e.component<MeshComponent>().get();
        Mesh* mesh = mesh_component->mesh;
        Texture* texture = mesh_component->texture;

        if (!mesh) continue;

        if (texture && current_tex != texture->texture_id) {
            glActiveTexture(GL_TEXTURE0);
            current_tex = texture->texture_id;
            glBindTexture(GL_TEXTURE_2D, current_tex);
        }

        glm::mat4 worldMat = transform->WorldSpace();
        glm::mat4 mvp = cam.combined * worldMat;

        glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
        glUniformMatrix4fv(glGetUniformLocation(shader, "worldMat"), 1, GL_FALSE, glm::value_ptr(worldMat));
        glUniform2f(glGetUniformLocation(shader, "material"), mesh_component->material.roughness,
                                                                      mesh_component->material.metallic);

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

