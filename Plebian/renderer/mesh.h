#ifndef MESH_H_
#define MESH_H_

#include <GL/glew.h>

struct Mesh {
	GLuint vertex_buffer;
	GLuint normal_buffer;
	GLuint tex_coord_buffer;
	GLuint index_buffer;
	GLuint vertex_array_object;
	GLsizei num_indices;
};

struct Material {
    Material() {}
    Material(float roughness, float metallic)
            : roughness(roughness), metallic(metallic) {}
    float roughness = 1, metallic = 1;
};


#endif // MESH_H_
