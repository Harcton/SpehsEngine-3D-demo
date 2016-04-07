
#include "DemoState3D.h"

#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/Camera3D.h>
#include <SpehsEngine/BatchManager.h>
#include <SpehsEngine/Mesh.h>
#include <SpehsEngine/Time.h>

#include <glm/vec3.hpp>


DemoState3D::DemoState3D()
{
	camera = new spehs::Camera3D();
	batchManager = new spehs::BatchManager(camera);
	spehs::setActiveBatchManager(batchManager);

	meshes.push_back(batchManager->createMesh("Models/case.obj"));
	hero = meshes.back();
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
	static glm::vec3 rotation(0.0f);
	static glm::vec3 position(0.0f);
	static glm::vec3 cameraPosition(0.0f, 0.0f, -3.0f);
	static glm::vec2 cameraLook(0.0f);
	static float speed = 0.01f;;

	inputManager->update();
	if (inputManager->isKeyDown(KEYBOARD_ESCAPE) || inputManager->isQuitRequested())
		return false;

	if (inputManager->isKeyDown(MOUSEBUTTON_RIGHT)) //Camera movement
	{
		if (inputManager->isKeyDown(KEYBOARD_W))
		{
			cameraPosition.z += speed * spehs::deltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_S))
		{
			cameraPosition.z -= speed * spehs::deltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_A))
		{
			cameraPosition.x += speed * spehs::deltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_D))
		{
			cameraPosition.x -= speed * spehs::deltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_E))
		{
			cameraPosition.y += speed * spehs::deltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_Q))
		{
			cameraPosition.y -= speed * spehs::deltaTime;
		}
		cameraLook.x += inputManager->getMouseMovementX() * spehs::deltaTime * speed;
		cameraLook.y += inputManager->getMouseMovementY() * spehs::deltaTime * speed;
		//Restrict mouse look
		if (cameraLook.x > 89.0f)
			cameraLook.x = 89.0f;
		if (cameraLook.x < -89.0f)
			cameraLook.x = -89.0f;
		if (cameraLook.y > 89.0f)
			cameraLook.y = 89.0f;
		if (cameraLook.y > -89.0f)
			cameraLook.y = -89.0f;
	}
	else //Object movement
	{
		//MOVE
		if (inputManager->isKeyDown(KEYBOARD_W))
		{
			position.z -= speed * spehs::deltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_S))
		{
			position.z += speed * spehs::deltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_A))
		{
			position.x -= speed * spehs::deltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_D))
		{
			position.x += speed * spehs::deltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_E))
		{
			position.y -= speed * spehs::deltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_Q))
		{
			position.y += speed * spehs::deltaTime;
		}
		//ROTATE
		if (inputManager->isKeyDown(KEYBOARD_I))
		{
			rotation.y -= speed * spehs::deltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_K))
		{
			rotation.y += speed * spehs::deltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_J))
		{
			rotation.x -= speed * spehs::deltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_L))
		{
			rotation.x += speed * spehs::deltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_O))
		{
			rotation.z -= speed * spehs::deltaTime;
		}
		if (inputManager->isKeyDown(KEYBOARD_U))
		{
			rotation.z += speed * spehs::deltaTime;
		}
	}
	camera->setPosition(cameraPosition);
	camera->setRotation(cameraLook);
	hero->setPosition(position);
	hero->setRotation(rotation);
	
	return true;
}
