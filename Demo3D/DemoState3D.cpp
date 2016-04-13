
#include "DemoState3D.h"

#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/Camera3D.h>
#include <SpehsEngine/BatchManager.h>
#include <SpehsEngine/Mesh.h>
#include <SpehsEngine/Time.h>
#include <SpehsEngine/Console.h>

#include <glm/vec3.hpp>
#include <glm/trigonometric.hpp>


DemoState3D::DemoState3D() : position(0.0f), rotation(0.0f)
{
	camera = new spehs::Camera3D();
	camera->setSmoothCamera(true);
	batchManager = new spehs::BatchManager(camera);
	spehs::setActiveBatchManager(batchManager);

	meshes.push_back(batchManager->createMesh("Models/duck_ship.obj"));
	meshes.back()->setColor(spehs::RED);
	hero = meshes.back();

	meshes.push_back(batchManager->createMesh("Models/plane.obj"));
	meshes.back()->setColor(spehs::CYAN);
}
DemoState3D::~DemoState3D()
{
	for (unsigned i = 0; i < meshes.size(); i++)
	{
		meshes[i]->destroy();
	}

	delete camera;
	delete batchManager;
}


bool DemoState3D::update()
{
	camera->update();

	if (!input())
		return false;

	spehs::console::update();	

	return true;
}

void DemoState3D::render()
{
	spehs::setActiveBatchManager(batchManager);
	batchManager->render();
}

bool DemoState3D::input()
{
	static float speed = 3.5f;
	static float lookSpeed = 0.8f;
	static float actualDeltaTime;
	actualDeltaTime = float(spehs::deltaTime) / 1000.0f;

	inputManager->update();
	if (inputManager->isKeyDown(KEYBOARD_ESCAPE) || inputManager->isQuitRequested())
		return false;

	if (inputManager->isKeyDown(MOUSEBUTTON_RIGHT)) //Camera movement
	{
		if (inputManager->isKeyDown(KEYBOARD_W))
		{
			camera->move(spehs::FORWARD, speed * actualDeltaTime);
		}
		if (inputManager->isKeyDown(KEYBOARD_S))
		{
			camera->move(spehs::BACKWARD, speed * actualDeltaTime);
		}
		if (inputManager->isKeyDown(KEYBOARD_A))
		{
			camera->move(spehs::LEFT, speed * actualDeltaTime);
		}
		if (inputManager->isKeyDown(KEYBOARD_D))
		{
			camera->move(spehs::RIGHT, speed * actualDeltaTime);
		}
		if (inputManager->isKeyDown(KEYBOARD_E))
		{
			camera->move(spehs::DOWN, speed * actualDeltaTime);
		}
		if (inputManager->isKeyDown(KEYBOARD_Q))
		{
			camera->move(spehs::UP, speed * actualDeltaTime);
		}
		camera->pitch(inputManager->getMouseMovementX() * actualDeltaTime * lookSpeed);
		camera->yaw(inputManager->getMouseMovementY() * actualDeltaTime * lookSpeed);
	}
	else//Object movement
	{
		//MOVE
		if (inputManager->isKeyDown(KEYBOARD_W))
		{
			position.z -= speed * actualDeltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_S))
		{
			position.z += speed * actualDeltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_A))
		{
			position.x -= speed * actualDeltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_D))
		{
			position.x += speed * actualDeltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_E))
		{
			position.y -= speed * actualDeltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_Q))
		{
			position.y += speed * actualDeltaTime;
		}
		//ROTATE
		if (inputManager->isKeyDown(KEYBOARD_I))
		{
			rotation.y -= speed * actualDeltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_K))
		{
			rotation.y += speed * actualDeltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_J))
		{
			rotation.x -= speed * actualDeltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_L))
		{
			rotation.x += speed * actualDeltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_O))
		{
			rotation.z -= speed * actualDeltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_U))
		{
			rotation.z += speed * actualDeltaTime;
		}
	}
	hero->setPosition(position);
	hero->setRotation(rotation);
	
	//camera->setTarget(position);

	return true;
}
