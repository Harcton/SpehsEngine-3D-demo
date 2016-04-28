#include <SpehsEngine/Console.h>
#include <SpehsEngine/Camera3D.h>
#include <SpehsEngine/BatchManager.h>
#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/ShaderManager.h>
#include <SpehsEngine/Mesh.h>
#include <SpehsEngine/Time.h>
#include "TeoState3D.h"


TeoState3D::TeoState3D()
{
	camera = new spehs::Camera3D();
	camera->setSmoothCamera(true);
	batchManager = new spehs::BatchManager(camera);
	spehs::setActiveBatchManager(batchManager);

	mesh = batchManager->createMesh("Models/duck_ship.obj");
	mesh->setColor(spehs::CYAN);
	mesh->setPosition(0.0f, 0.0f, 0.0f);
	mesh->setShader((int)spehs::ShaderName::DefaultTextureMesh);
}
TeoState3D::~TeoState3D()
{
	mesh->destroy();
	delete camera;
	delete batchManager;
}
bool TeoState3D::update()
{
	camera->update();	
	spehs::console::update();
	inputManager->update();
	if (inputManager->isKeyPressed(KEYBOARD_ESCAPE) || inputManager->isQuitRequested())
		return false;










	static float speed = 2.0f;
	static float lookSpeed = 0.5f;
	if (inputManager->isKeyDown(KEYBOARD_W))
	{
		camera->move(spehs::FORWARD, speed * spehs::getDeltaTime().asSeconds);
	}
	if (inputManager->isKeyDown(KEYBOARD_S))
	{
		camera->move(spehs::BACKWARD, speed * spehs::getDeltaTime().asSeconds);
	}
	if (inputManager->isKeyDown(KEYBOARD_A))
	{
		camera->move(spehs::LEFT, speed * spehs::getDeltaTime().asSeconds);
	}
	if (inputManager->isKeyDown(KEYBOARD_D))
	{
		camera->move(spehs::RIGHT, speed * spehs::getDeltaTime().asSeconds);
	}
	if (inputManager->isKeyDown(KEYBOARD_LSHIFT))
	{
		camera->move(spehs::DOWN, speed * spehs::getDeltaTime().asSeconds);
	}
	if (inputManager->isKeyDown(KEYBOARD_SPACE))
	{
		camera->move(spehs::UP, speed * spehs::getDeltaTime().asSeconds);
	}
	camera->pitch(inputManager->getMouseMovementX() * spehs::getDeltaTime().asSeconds * lookSpeed);
	camera->yaw(inputManager->getMouseMovementY() * spehs::getDeltaTime().asSeconds * lookSpeed);
	












	return true;
}
void TeoState3D::render()
{
	spehs::setActiveBatchManager(batchManager);
	batchManager->render();
}