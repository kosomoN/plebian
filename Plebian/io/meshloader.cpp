#include "io/meshloader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobj/tiny_obj_loader.h>

#include "log.h"

#define MESH_PATH "assets/meshes/"

Mesh* MeshLoader::GetMesh(std::string name) {
	if (!meshes.count(name)) {
		Log(Info, "Loading mesh: %s", name.c_str());

		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;
		if (!tinyobj::LoadObj(shapes, materials, err, std::string(MESH_PATH + name).c_str())) {
			Log(Error, "Failed loading obj: %s", err.c_str());
			return nullptr;
		}

		tinyobj::mesh_t shape = shapes[0].mesh;
		if (shape.positions.empty() || shape.normals.empty() || shape.texcoords.empty()) {
			Log(Error, "One or more of the requried geometry properties are missing in %s", std::string(MESH_PATH + name).c_str());
			return nullptr;
		}

		Mesh* mesh = new Mesh;
		mesh->num_indices = shape.indices.size();

		glGenVertexArrays(1, &(mesh->vertex_array_object));
		glBindVertexArray(mesh->vertex_array_object);

		glEnableVertexAttribArray(0);
		glGenBuffers(1, &(mesh->vertex_buffer));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * shape.positions.size(), &shape.positions[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GL_FALSE, nullptr);

		glEnableVertexAttribArray(1);
		glGenBuffers(1, &(mesh->normal_buffer));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->normal_buffer);
		glBufferData(GL_ARRAY_BUFFER, shape.normals.size() * sizeof(float), &shape.normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, GL_FALSE, nullptr);

		glEnableVertexAttribArray(2);
		glGenBuffers(1, &(mesh->tex_coord_buffer));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->tex_coord_buffer);
		glBufferData(GL_ARRAY_BUFFER, shape.texcoords.size() * sizeof(float), &shape.texcoords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glGenBuffers(1, &(mesh->index_buffer));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indices.size() * sizeof(unsigned int), &shape.indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);

		meshes[name] = mesh;
	}

	return meshes[name];
}
