#pragma once
#include <glm/vec3.hpp>
namespace SpehsEngine { class Camera3D; }

class Camera3DController
{
public:
	Camera3DController(spehs::Camera3D& camera);
	~Camera3DController();

	void update();
private:
	spehs::Camera3D& camera;
};