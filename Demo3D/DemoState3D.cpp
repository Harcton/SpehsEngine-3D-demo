
#include "DemoState3D.h"

#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/Camera3D.h>
#include <SpehsEngine/BatchManager.h>
#include <SpehsEngine/Mesh.h>


DemoState3D::DemoState3D()
{
	camera = new spehs::Camera3D();
	batchManager = new spehs::BatchManager(camera);
	spehs::setActiveBatchManager(batchManager);

	meshes.push_back(batchManager->createMesh("Models/rock.obj"));
}
DemoState3D::~DemoState3D()
{
	delete camera;
	delete batchManager;
}


bool DemoState3D::update()
{
	if (!input())
		return false;

	camera->update();

	return true;
}

void DemoState3D::render()
{
	spehs::setActiveBatchManager(batchManager);
	batchManager->render();
}

bool DemoState3D::input()
{
	static float rotation = 0.0f;

	inputManager->update();
	if (inputManager->isKeyDown(KEYBOARD_ESCAPE) || inputManager->isQuitRequested())
		return false;

	if (inputManager->isKeyDown(KEYBOARD_W))
	{
		camera->translate(glm::vec3(-0.03f));
	}
	if (inputManager->isKeyDown(KEYBOARD_S))
	{
		camera->translate(glm::vec3(0.03f));
	}
	if (inputManager->isKeyDown(KEYBOARD_Q))
	{
		rotation += 0.01f;
	}
	//camera->setRotation(glm::vec2(rotation));

	return true;
}
