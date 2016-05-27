#ifndef ISCENE_H_
#define ISCENE_H_

#include <renderer\camera_interface.h>

// This naming is retarded
struct Scene_interface {
	virtual ~Scene_interface() = 0;
	virtual void Render() = 0;
	virtual void Init() = 0;
	virtual Camera_interface* GetSceneCamera() = 0;
	virtual void SetSceneCamera(Camera_interface* p_camera) = 0;
};

#endif