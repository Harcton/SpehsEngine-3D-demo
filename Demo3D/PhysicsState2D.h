
#pragma once

#include "State.h"

#include <vector>


namespace spehs{ class GameObject; class Camera2D; class BatchManager; class Polygon; struct CollisionPoint; }

class PhysicsState2D : public State
{
public:
	PhysicsState2D();
	~PhysicsState2D();

	bool update();
	void render();
	bool input();
	void collisionTesting();

private:
	spehs::Camera2D* camera;
	spehs::BatchManager* batchManager;
	std::vector<spehs::GameObject*> objects;
	std::vector<spehs::Polygon*> collisionPointVisuals;
	spehs::GameObject* userOBJ;

	spehs::CollisionPoint* collisionPoint;
};

