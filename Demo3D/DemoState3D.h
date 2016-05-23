
#pragma once

#include "State.h"

#include <glm/vec3.hpp>

#include <vector>


namespace spehs{ class Mesh; class Camera3D; class BatchManager; class SkyBox; }


class DemoState3D : public State
{
public:
	DemoState3D();
	~DemoState3D();

	bool update();
	void render();
	bool input();

private:
	void load();

	spehs::Camera3D* camera;
	spehs::BatchManager* batchManager;

	spehs::SkyBox* skyBox;
	std::vector<spehs::Mesh*> meshes;
	std::vector<spehs::Mesh*> particles;
	spehs::Mesh* land;
};

