#include <SpehsEngine/Time.h>
#include <SpehsEngine/SpehsEngine.h>
#include <SpehsEngine/Console.h>
#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/Window.h>
#include <SpehsEngine/ApplicationData.h>

#include <SpehsEngine/Camera2D.h>
#include <SpehsEngine/TextureManager.h>
#include <SpehsEngine/BatchManager.h>
#include <SpehsEngine/Polygon.h>
#include <SpehsEngine/Point.h>
#include <SpehsEngine/Line.h>
#include <SpehsEngine/RNG.h>
#include <SpehsEngine/Time.h>
#include <SpehsEngine/Vertex.h>

#include <vector>
#include <glm/vec2.hpp>

#include <Windows.h>


void update();
void render();
void input();

static bool run = true;
spehs::Camera2D* camera;
spehs::BatchManager* batchManager;
spehs::Vertex* vertices;

std::vector<spehs::Polygon*> polygons;
std::vector<spehs::Point*> points;
std::vector<spehs::Line*> lines;

void main()
{
	spehs::initialize("spehs_engine_testing_zone");
	mainWindow->clearColor(0.2f, 0.3f, 0.3f, 1.0f);
	spehs::console::addVariable("fps", applicationData->showFps);
	spehs::console::addVariable("maxfps", applicationData->maxFps);

	camera = new spehs::Camera2D();
	batchManager = new spehs::BatchManager(camera);

	spehs::setActiveBatchManager(batchManager);
	textureManager->setDefaultTexture("test_texture.png");
	static float distrib = 500.0f;

	vertices = new spehs::Vertex [4];
	vertices[0].position.setPosition(-1.0f, 1.0f);
	vertices[1].position.setPosition(-1.0f, -1.0f);
	vertices[2].position.setPosition(1.0f, -1.0f);
	vertices[3].position.setPosition(1.0f, 1.0f);

	for (unsigned i = 0; i < 2; i++)
	{
		polygons.push_back(batchManager->createPolygon(vertices, 4, 0, 100.0f + rng->frandom(0.2f, 5.0f) * i, 100.0f + rng->frandom(0.2f, 5.0f) * i));
		polygons.back()->setColor(glm::vec4(glm::vec3(1.0f - i / 100.0f), 1.0f));
		//polygons.back()->setPosition(rng->frandom(0, distrib), rng->frandom(0, distrib));
	}

	for (unsigned i = 0; i < 0; i++)
	{
		lines.push_back(batchManager->createLine(glm::vec2(rng->frandom(-distrib, distrib), rng->frandom(-distrib, distrib)), glm::vec2(rng->frandom(-distrib, distrib), rng->frandom(-distrib, distrib))));
		lines.back()->setColor(glm::vec4(1.0f));
	}

	for (unsigned i = 0; i < 0; i++)
	{
		points.push_back(batchManager->createPoint());
		points.back()->setColor(1.0f, 1.0f, 1.0f, 1.0f);
		//points.back()->setPosition(rng->frandom(-distrib, distrib), rng->frandom(-distrib, distrib));
	}
	
	while (run)
	{
		spehs::beginFPS();

		update();
		render();

		spehs::endFPS();
	}

	//BatchManager cleans up the primitives
	polygons.clear(); 
	lines.clear();
	points.clear();

	delete camera;
	delete batchManager;
	
	spehs::uninitialize();
	Sleep(500);
}

void update()
{
	spehs::console::update();

	inputManager->update();
	run = !inputManager->isQuitRequested();
	input();

	camera->update();
}

void render()
{
	mainWindow->renderBegin();

	spehs::setActiveBatchManager(batchManager);
	batchManager->render();

	spehs::console::render();
	spehs::drawFPS();

	mainWindow->renderEnd();
}

void input()
{
	if (inputManager->isKeyDown(KEYBOARD_ESCAPE))
		run = false;

	static glm::vec2 pos(0.0f, 0.0f);
	static float cameraScale = 1.0f;
	static float rotation = 0.0f;
	static float speed;

	static float R = 0.0f, G = 0.0f, B = 0.0f, A = 0.0f;

	speed = 1.0f * spehs::deltaTime;

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
		R += 0.02f;
	}
	if (inputManager->isKeyDown(KEYBOARD_2))
	{
		G += 0.02f;
	}
	if (inputManager->isKeyDown(KEYBOARD_3))
	{
		B += 0.02f;
	}
	if (inputManager->isKeyDown(KEYBOARD_4))
	{
		A += 0.02f;
	}

	for (unsigned i = 0; i < polygons.size(); i++)
	{
		polygons[i]->setPosition(pos / float(i + 1));
		polygons[i]->setRotation(rotation);
		polygons[i]->worldVertexArray[0].color = { R, 0.0f, 0.0f, 1.0f };
		polygons[i]->worldVertexArray[1].color = { 0.0f, G, 0.0f, 1.0f };
		polygons[i]->worldVertexArray[2].color = { 0.0f, 0.0f, B, 1.0f };
		polygons[i]->worldVertexArray[3].color = { 0.0f, 0.0f, 0.0f, A };
	}
	for (unsigned i = 0; i < lines.size(); i++)
	{
		//lines[i]->setPosition(pos);
		lines[i]->setRotation(rotation);
	}
	for (unsigned i = 0; i < points.size(); i++)
	{
		//points[i]->setPosition(pos);
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
		cameraScale -= 0.01f * camera->scale;
		camera->scale = cameraScale;
	}
	if (inputManager->isKeyDown(KEYBOARD_PAGEUP))
	{
		cameraScale += 0.01f * camera->scale;
		camera->scale = cameraScale;
	}
}