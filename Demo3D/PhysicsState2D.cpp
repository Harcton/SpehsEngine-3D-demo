
#include "PhysicsState2D.h"
#include "CreateObject.h"
#include "SpehsEngine/Camera2D.h"
#include "SpehsEngine/BatchManager.h"
#include "SpehsEngine/GameObject.h"
#include "SpehsEngine/InputManager.h"
#include "SpehsEngine/RNG.h"


PhysicsState2D::PhysicsState2D()
{
	camera = new spehs::Camera2D();
	batchManager = new spehs::BatchManager(camera);
}
PhysicsState2D::~PhysicsState2D()
{
	for (unsigned i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
	delete batchManager;
	delete camera;
}


bool PhysicsState2D::update()
{
	if (!input())
	{
		return false;
	}
	for (unsigned i = 0; i < objects.size(); i++)
	{
		objects[i]->update();
	}
	return true;
}
void PhysicsState2D::render()
{
	batchManager->render();
}
bool PhysicsState2D::input()
{
	inputManager->update();
	if (inputManager->isQuitRequested() || inputManager->isKeyPressed(KEYBOARD_ESCAPE))
	{
		return false;
	}
	if (inputManager->isKeyPressed(KEYBOARD_SPACE))
	{
		objects.push_back(createPhysicsObject(rng->frandom(1.0f, 3.0f), rng->frandom(1.0f, 3.0f), 4));
	}
	return true;
}