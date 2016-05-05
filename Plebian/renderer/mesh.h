#ifndef MESH_H_
#define MESH_H_

#include <gl/glew.h>

struct Mesh {
	GLuint vertex_buffer;
	GLuint normal_buffer;
	GLuint tex_coord_buffer;
	GLuint index_buffer;
	GLuint vertex_array_object;
	GLsizei num_indices;
};


#endif // MESH_H_
