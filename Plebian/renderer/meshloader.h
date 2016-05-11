#ifndef MESHLOADER_H_
#define MESHLOADER_H_

#include <string>
#include <unordered_map>

#include <GL/glew.h>

#include "renderer/mesh.h"

class MeshLoader {
public:
	Mesh* GetMesh(std::string name);
private:
	std::unordered_map<std::string, Mesh*> meshes;
};


#endif // MESH_H_
