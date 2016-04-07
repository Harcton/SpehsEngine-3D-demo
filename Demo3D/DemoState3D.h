
#pragma once

#include "State.h"

#include <vector>


namespace spehs{ class Mesh; class Camera3D; class BatchManager; }

class DemoState3D : public State
{
public:
	DemoState3D();
	~DemoState3D();

	bool update();
	void render();
	bool input();

private:
	spehs::Camera3D* camera;
	spehs::BatchManager* batchManager;

	std::vector<spehs::Mesh*> meshes;
	spehs::Mesh* hero;
};

