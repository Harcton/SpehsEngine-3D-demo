
#include "DemoState3D.h"

#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/Camera3D.h>
#include <SpehsEngine/BatchManager.h>
#include <SpehsEngine/Mesh.h>
#include <SpehsEngine/Time.h>
#include <SpehsEngine/Console.h>

#include <glm/vec3.hpp>
#include <glm/trigonometric.hpp>


DemoState3D::DemoState3D() : position(0.0f, -1.0f, 0.0f), rotation(0.0f)
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
	meshes.back()->setPosition(0.0f, -10.0f, 0.0f);
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

	inputManager->update();
	if (inputManager->isKeyDown(KEYBOARD_ESCAPE) || inputManager->isQuitRequested())
		return false;

	if (inputManager->isKeyDown(MOUSEBUTTON_RIGHT)) //Camera movement
	{
		if (inputManager->isKeyDown(KEYBOARD_W))
		{
			camera->move(spehs::FORWARD, speed * spehs::deltaTime.asSeconds);
		}
		if (inputManager->isKeyDown(KEYBOARD_S))
		{
			camera->move(spehs::BACKWARD, speed * spehs::deltaTime.asSeconds);
		}
		if (inputManager->isKeyDown(KEYBOARD_A))
		{
			camera->move(spehs::LEFT, speed * spehs::deltaTime.asSeconds);
		}
		if (inputManager->isKeyDown(KEYBOARD_D))
		{
			camera->move(spehs::RIGHT, speed * spehs::deltaTime.asSeconds);
		}
		if (inputManager->isKeyDown(KEYBOARD_E))
		{
			camera->move(spehs::DOWN, speed * spehs::deltaTime.asSeconds);
		}
		if (inputManager->isKeyDown(KEYBOARD_Q))
		{
			camera->move(spehs::UP, speed * spehs::deltaTime.asSeconds);
		}
		camera->pitch(inputManager->getMouseMovementX() * spehs::deltaTime.asSeconds * lookSpeed);
		camera->yaw(inputManager->getMouseMovementY() * spehs::deltaTime.asSeconds * lookSpeed);
	}
	else//Object movement
	{
		if (inputManager->isKeyDown(MOUSEBUTTON_MIDDLE))
		{
			camera->move(spehs::UP, inputManager->getMouseMovementY() * lookSpeed * spehs::deltaTime.asSeconds);
			camera->move(spehs::RIGHT, inputManager->getMouseMovementX() * lookSpeed * spehs::deltaTime.asSeconds);
		}
		//MOVE
		if (inputManager->isKeyDown(KEYBOARD_W))
		{
			position.z -= speed * spehs::deltaTime.asSeconds;
		}
		if (inputManager->isKeyDown(KEYBOARD_S))
		{
			position.z += speed * spehs::deltaTime.asSeconds;
		}
		if (inputManager->isKeyDown(KEYBOARD_A))
		{
			position.x -= speed * spehs::deltaTime.asSeconds;
		}
		if (inputManager->isKeyDown(KEYBOARD_D))
		{
			position.x += speed * spehs::deltaTime.asSeconds;
		}
		if (inputManager->isKeyDown(KEYBOARD_E))
		{
			position.y -= speed * spehs::deltaTime.asSeconds;
		}
		if (inputManager->isKeyDown(KEYBOARD_Q))
		{
			position.y += speed * spehs::deltaTime.asSeconds;
		}
		//ROTATE
		if (inputManager->isKeyDown(KEYBOARD_I))
		{
			rotation.y -= speed * spehs::deltaTime.asSeconds;
		}
		if (inputManager->isKeyDown(KEYBOARD_K))
		{
			rotation.y += speed * spehs::deltaTime.asSeconds;
		}
		if (inputManager->isKeyDown(KEYBOARD_J))
		{
			rotation.x -= speed * spehs::deltaTime.asSeconds;
		}
		if (inputManager->isKeyDown(KEYBOARD_L))
		{
			rotation.x += speed * spehs::deltaTime.asSeconds;
		}
		if (inputManager->isKeyDown(KEYBOARD_O))
		{
			rotation.z -= speed * spehs::deltaTime.asSeconds;
		}
		if (inputManager->isKeyDown(KEYBOARD_U))
		{
			rotation.z += speed * spehs::deltaTime.asSeconds;
		}
	}
	hero->setPosition(position);
	hero->setRotation(rotation);
	
	//camera->setTarget(position);

	return true;
}
