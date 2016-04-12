
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
	batchManager = new spehs::BatchManager(camera);
	spehs::setActiveBatchManager(batchManager);

	meshes.push_back(batchManager->createMesh("Models/plane.obj"));
	meshes.back()->setPosition(0.0f, -1.0f, 3.0f);
	meshes.back()->setColor(spehs::RED);
	hero = meshes.back();

	hero->worldVertexArray[0].color.setColor(spehs::YELLOW);
	hero->worldVertexArray[1].color.setColor(spehs::ORANGE);
	hero->worldVertexArray[2].color.setColor(spehs::GREEN);
	hero->worldVertexArray[3].color.setColor(spehs::CYAN);

	meshes.push_back(batchManager->createMesh("Models/case.obj"));
	meshes.back()->setColor(spehs::CYAN);
	meshes.back()->setRenderState(false);
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
	static float speed = 0.005f;

	inputManager->update();
	if (inputManager->isKeyDown(KEYBOARD_ESCAPE) || inputManager->isQuitRequested())
		return false;

	if (inputManager->isKeyDown(MOUSEBUTTON_RIGHT)) //Camera movement
	{
		if (inputManager->isKeyDown(KEYBOARD_W))
		{
			camera->moveForward(speed * spehs::deltaTime);
		}
		if (inputManager->isKeyDown(KEYBOARD_S))
		{
			camera->moveBackward(speed * spehs::deltaTime);
		}
		if (inputManager->isKeyDown(KEYBOARD_A))
		{
			camera->moveLeft(speed * spehs::deltaTime);
		}
		if (inputManager->isKeyDown(KEYBOARD_D))
		{
			camera->moveRight(speed * spehs::deltaTime);
		}
		if (inputManager->isKeyDown(KEYBOARD_E))
		{
			camera->moveDown(speed * spehs::deltaTime);
		}
		if (inputManager->isKeyDown(KEYBOARD_Q))
		{
			camera->moveUp(speed * spehs::deltaTime);
		}
		camera->pitch(inputManager->getMouseMovementX() * spehs::deltaTime * speed); //pitch
		camera->yaw(inputManager->getMouseMovementY() * spehs::deltaTime * speed); //yaw
	}
	else//Object movement
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
	hero->setPosition(position);
	hero->setRotation(rotation);
	
	camera->setTarget(position);

	return true;
}
