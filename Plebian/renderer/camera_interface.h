#ifndef ICAMERA_H_
#define ICAMERA_H_

#include <glm\mat4x4.hpp>
#include <glm\vec3.hpp>

struct CameraProperties {
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 worldPosition;
};

struct Camera_interface {
	virtual ~Camera_interface() = 0;
	virtual void Update() = 0;
	virtual void Init() = 0;
	virtual CameraProperties& GetCameraProperties() = 0;
};

#endif