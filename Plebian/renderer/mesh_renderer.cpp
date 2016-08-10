#include "mesh_renderer.h"

#include <algorithm>
#include <GL/glew.h>
#include "components.h"

void MeshRenderer::Render(float delta) {
    for (entityx::Entity e : entities) {
        auto transform = e.component<Transform>();
        auto meshComponent = e.component<MeshComponent>();
        Mesh* mesh = meshComponent.get()->mesh;

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

