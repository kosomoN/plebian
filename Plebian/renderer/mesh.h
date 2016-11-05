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
    Material(float roughness, float metallic)
            : roughness(roughness), metallic(metallic) {}
    float roughness = 0, metallic = 0;
};


#endif // MESH_H_
