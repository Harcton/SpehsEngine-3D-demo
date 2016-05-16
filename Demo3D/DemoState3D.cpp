
#include "DemoState3D.h"
#include "ShaderContainer.h"

#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/Camera3D.h>
#include <SpehsEngine/BatchManager.h>
#include <SpehsEngine/Mesh.h>
#include <SpehsEngine/Time.h>
#include <SpehsEngine/Console.h>
#include <SpehsEngine/SkyBox.h>
#include <SpehsEngine/TextureManager.h>

#include <glm/vec3.hpp>
#include <glm/trigonometric.hpp>


DemoState3D::DemoState3D() : position(0.0f, 10.0f, 0.0f), rotation(0.0f)
{
	camera = new spehs::Camera3D();
	camera->setSmoothCamera(true);
	camera->setFar(50000.0f);
	camera->setNear(5.0f);
	camera->setFOV(40.0f);
	batchManager = new spehs::BatchManager(camera);
	spehs::setActiveBatchManager(batchManager);

	skyBox = new spehs::SkyBox("Textures/Simple/sky", ".png");
	skyBox->setShader((int)ShaderName::Sky);

	//meshes.push_back(batchManager->createMesh("Models/duck_ship.obj"));
	//meshes.back()->setColor(spehs::RED);
	//hero = meshes.back();

	meshes.push_back(batchManager->createMesh("Models/environment.obj"));
	meshes.back()->setTexture("Textures/stone_texture.jpg");
	meshes.back()->setScale(8.0f);
	meshes.back()->setPosition(0.0f, 0.5f, 0.0f);
	meshes.back()->setShader((int) ShaderName::Environment);

	meshes.push_back(batchManager->createMesh("Models/environment.obj"));
	meshes.back()->setTexture("Textures/stone_texture.jpg");
	meshes.back()->setScale(10.0f);
	meshes.back()->setRotation(0.0f, 2.0f, 0.0f);
	meshes.back()->setPosition(3550.0f, 0.5f, 2550.0f);
	meshes.back()->setShader((int) ShaderName::Environment);

	meshes.push_back(batchManager->createMesh("Models/environment.obj"));
	meshes.back()->setTexture("Textures/stone_texture.jpg");
	meshes.back()->setScale(30.0f);
	meshes.back()->setRotation(0.0f, 3.14f, 0.0f);
	meshes.back()->setPosition(0.0f, -1.0f, 0.0f);
	meshes.back()->setShader((int) ShaderName::Environment);

	meshes.push_back(batchManager->createMesh("Models/plane.obj"));
	meshes.back()->setScale(30000.0f);
	meshes.back()->setShader((int)ShaderName::Water);
	meshes.back()->setTexture("Textures/water-caustics.jpg");
}
DemoState3D::~DemoState3D()
{
	delete skyBox;
	for (unsigned i = 0; i < meshes.size(); i++)
	{
		meshes[i]->destroy();
	}

	delete camera;
	delete batchManager;
}


bool DemoState3D::update()
{
	shaderManager->getShader((int) ShaderName::Water)->getCustomUniforms<WaterUniforms>()->lightPosition = camera->getPosition();
	shaderManager->getShader((int) ShaderName::Environment)->getCustomUniforms<DemoUniforms>()->lightPosition = camera->getPosition();

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
	static float speed = 8.5f;
	static float lookSpeed = 0.8f;

	inputManager->update();
	if (inputManager->isKeyDown(KEYBOARD_ESCAPE) || inputManager->isQuitRequested())
		return false;

	if (inputManager->isKeyPressed(KEYBOARD_F5))
	{
		for (unsigned i = NEXT_SHADER_INDEX; i < NEXT_SHADER_INDEX + NUM_SHADERS; i++)
		{
			reloadShader((ShaderName)i);
		}
	}

	if (inputManager->isKeyDown(MOUSEBUTTON_RIGHT)) //Camera movement
	{
		inputManager->lockMouse(true);

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

		if (inputManager->isKeyDown(KEYBOARD_E))
		{
			camera->move(spehs::DOWN, speed * spehs::getDeltaTime().asSeconds);
		}
		if (inputManager->isKeyDown(KEYBOARD_Q))
		{
			camera->move(spehs::UP, speed * spehs::getDeltaTime().asSeconds);
		}

		camera->pitch(inputManager->getMouseMovementX() * spehs::getDeltaTime().asSeconds * lookSpeed);
		camera->yaw(inputManager->getMouseMovementY() * spehs::getDeltaTime().asSeconds * lookSpeed);
	}
	else if (inputManager->isKeyDown(MOUSEBUTTON_MIDDLE))
	{
		inputManager->lockMouse(true);

		camera->move(spehs::UP, inputManager->getMouseMovementY() * lookSpeed * spehs::getDeltaTime().asSeconds);
		camera->move(spehs::RIGHT, inputManager->getMouseMovementX() * lookSpeed * spehs::getDeltaTime().asSeconds);
	}
	else//Object movement
	{
		inputManager->lockMouse(false);
		//MOVE
		if (inputManager->isKeyDown(KEYBOARD_W))
		{
			position.z -= speed * spehs::getDeltaTime().asSeconds;
		}
		if (inputManager->isKeyDown(KEYBOARD_S))
		{
			position.z += speed * spehs::getDeltaTime().asSeconds;
		}
		if (inputManager->isKeyDown(KEYBOARD_A))
		{
			position.x -= speed * spehs::getDeltaTime().asSeconds;
		}
		if (inputManager->isKeyDown(KEYBOARD_D))
		{
			position.x += speed * spehs::getDeltaTime().asSeconds;
		}
		if (inputManager->isKeyDown(KEYBOARD_E))
		{
			position.y -= speed * spehs::getDeltaTime().asSeconds;
		}
		if (inputManager->isKeyDown(KEYBOARD_Q))
		{
			position.y += speed * spehs::getDeltaTime().asSeconds;
		}
		//ROTATE
		if (inputManager->isKeyDown(KEYBOARD_L))
		{
			rotation.y -= speed * spehs::getDeltaTime().asSeconds;
		}
		if (inputManager->isKeyDown(KEYBOARD_J))
		{
			rotation.y += speed * spehs::getDeltaTime().asSeconds;
		}
		if (inputManager->isKeyDown(KEYBOARD_O))
		{
			rotation.z -= speed * spehs::getDeltaTime().asSeconds;
		}
		if (inputManager->isKeyDown(KEYBOARD_U))
		{
			rotation.z += speed * spehs::getDeltaTime().asSeconds;
		}
		if (inputManager->isKeyDown(KEYBOARD_I))
		{
			rotation.x -= speed * spehs::getDeltaTime().asSeconds;
		}
		if (inputManager->isKeyDown(KEYBOARD_K))
		{
			rotation.x += speed * spehs::getDeltaTime().asSeconds;
		}
	}
	if (hero)
	{
		hero->setPosition(position);
		hero->setRotation(rotation);
	}
	
	if (camera->getPosition().y < 10.0f)
	{
		camera->setPosition(glm::vec3(camera->getPosition().x, 10.0f, camera->getPosition().z));
	}

	//camera->setTarget(position);
	//camera->setPosition(position + glm::vec3(0.0f, 5.0f, 6.0f));

	return true;
}
