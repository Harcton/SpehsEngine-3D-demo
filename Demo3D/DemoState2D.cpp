
#include "DemoState2D.h"

#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/ApplicationData.h>
#include <SpehsEngine/Console.h>
#include <SpehsEngine/Camera2D.h>
#include <SpehsEngine/TextureManager.h>
#include <SpehsEngine/BatchManager.h>
#include <SpehsEngine/Polygon.h>
#include <SpehsEngine/Point.h>
#include <SpehsEngine/Line.h>
#include <SpehsEngine/RNG.h>
#include <SpehsEngine/Time.h>
#include <SpehsEngine/Vertex.h>

#include <glm/vec2.hpp>

#define TARGET_POLYGON_COUNT 2000



DemoState2D::DemoState2D() : distrib(2000.0f)
{
	camera = new spehs::Camera2D();
	batchManager = new spehs::BatchManager(camera);

	spehs::setActiveBatchManager(batchManager);
	textureManager->setDefaultTexture("Textures/test_texture.png");

	for (unsigned i = 0; i < TARGET_POLYGON_COUNT; i++)
	{
		polygons.push_back(batchManager->createPolygon(6, 10, 100.0f, 100.0f));
		polygons.back()->setPosition(rng->frandom(-distrib, distrib), rng->frandom(-distrib, distrib));
		polygons.back()->setTexture("Textures/test_texture.png");
	}

	for (unsigned i = 0; i < 8; i++)
	{
		lines.push_back(batchManager->createLine(glm::vec2(rng->frandom(-distrib, distrib), rng->frandom(-distrib, distrib)), glm::vec2(rng->frandom(-distrib, distrib), rng->frandom(-distrib, distrib)), 5));
		lines.back()->setColor(glm::vec4(1.0f));
	}

	for (unsigned i = 0; i < TARGET_POLYGON_COUNT; i++)
	{
		points.push_back(batchManager->createPoint(2));
		points.back()->setColor(glm::vec4(0.9f));
		points.back()->setPosition(rng->frandom(-distrib, distrib), rng->frandom(-distrib, distrib));
	}
}
DemoState2D::~DemoState2D()
{
	delete camera;
	delete batchManager;
}


bool DemoState2D::update()
{
#ifdef TARGET_POLYGON_COUNT
	static unsigned int counter = 0;
	static unsigned int count = 500;
	static unsigned int temp;
	counter += spehs::deltaTime;
	if (counter > 16)
	{
		counter -= 16;
		//REMOVE
		for (unsigned i = 0; i < count; i++)
		{
			temp = rng->irandom(0, polygons.size() - 1);
			polygons[temp]->destroy();
			if (polygons[temp] != polygons.back())
				polygons[temp] = polygons.back();
			polygons.pop_back();
		}
		//REPLACE
		for (unsigned i = 0; i < count; i++)
		{
			polygons.push_back(batchManager->createPolygon(6, 10, 100.0f, 100.0f));
			polygons.back()->setPosition(rng->frandom(-distrib, distrib), rng->frandom(-distrib, distrib));
			polygons.back()->setTexture("Textures/test_texture.png");
		}
	}
#endif

	spehs::console::update();

	if (!input())
		return false;

	camera->update();

	return true;
}

void DemoState2D::render()
{
	batchManager->render();
}

bool DemoState2D::input()
{
	inputManager->update();
	if (inputManager->isKeyDown(KEYBOARD_ESCAPE) || inputManager->isQuitRequested())
		return false;

	static glm::vec2 pos(0.0f, 0.0f);
	static float cameraScale = 1.0f;
	static float rotation = 0.0f;
	static float speed;
	static float minus = 1;
	static int minus_timer = 0;
	static float R = 0.5f, G = 0.5f, B = 0.5f, A = 1.0f;

	speed = 1.0f * spehs::deltaTime;
	
	minus_timer += spehs::deltaTime;
	if (minus_timer > 2000)
	{
		minus_timer = 0;
		minus = -minus;
	}

	if (inputManager->isKeyDown(KEYBOARD_W))
	{
		pos.y += speed;
	}
	if (inputManager->isKeyDown(KEYBOARD_S))
	{
		pos.y -= speed;
	}
	if (inputManager->isKeyDown(KEYBOARD_A))
	{
		pos.x -= speed;
	}
	if (inputManager->isKeyDown(KEYBOARD_D))
	{
		pos.x += speed;
	}
	if (inputManager->isKeyDown(KEYBOARD_Q))
	{
		rotation += speed / 100.0f;
	}
	if (inputManager->isKeyDown(KEYBOARD_E))
	{
		rotation -= speed / 100.0f;
	}
	if (inputManager->isKeyDown(KEYBOARD_1))
	{
		R += minus*0.02f;
	}
	if (inputManager->isKeyDown(KEYBOARD_2))
	{
		G += minus*0.02f;
	}
	if (inputManager->isKeyDown(KEYBOARD_3))
	{
		B += minus*0.02f;
	}
	if (inputManager->isKeyDown(KEYBOARD_4))
	{
		A -= minus*0.02f;
	}

	for (unsigned i = 0; i < polygons.size(); i++)
	{
		//polygons[i]->setPosition(pos / float(i + 1));
		polygons[i]->setRotation(rotation);
		for (unsigned w = 0; w < polygons[i]->numVertices; w++)
		{
			polygons[i]->worldVertexArray[w].color = { R, G, B, A };
		}
	}
	for (unsigned i = 0; i < lines.size(); i++)
	{
		//lines[i]->setPosition(pos / float(i + 1));
		lines[i]->setRotation(rotation);
	}
	for (unsigned i = 0; i < points.size(); i++)
	{
		//points[i]->setPosition(pos / float(i + 1));
	}

	//CAMERA:
	if (inputManager->isKeyDown(KEYBOARD_LEFT))
	{
		camera->translate(glm::vec2(speed, 0.0f));
	}
	if (inputManager->isKeyDown(KEYBOARD_RIGHT))
	{
		camera->translate(glm::vec2(-speed, 0.0f));
	}
	if (inputManager->isKeyDown(KEYBOARD_UP))
	{
		camera->translate(glm::vec2(0.0f, -speed));
	}
	if (inputManager->isKeyDown(KEYBOARD_DOWN))
	{
		camera->translate(glm::vec2(0.0f, speed));
	}
	if (inputManager->isKeyDown(KEYBOARD_PAGEDOWN))
	{
		cameraScale -= 0.03f * camera->scale;
		camera->scale = cameraScale;
	}
	if (inputManager->isKeyDown(KEYBOARD_PAGEUP))
	{
		cameraScale += 0.03f * camera->scale;
		camera->scale = cameraScale;
	}
	if (inputManager->isKeyDown(KEYBOARD_END))
	{
		cameraScale = 1.0f;
		camera->scale = cameraScale;
	}
	camera->setRotation(rotation);

	return true;
}