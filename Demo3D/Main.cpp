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

#include <vector>
#include <glm/vec2.hpp>


void update();
void render();
void input();

static bool run = true;
spehs::Camera2D* camera;
spehs::BatchManager* batchManager;

std::vector<spehs::Polygon*> polygons;
std::vector<spehs::Point*> points;
std::vector<spehs::Line*> lines;

void main()
{
	spehs::initialize("spehs_engine_testing_zone");
	mainWindow->clearColor(0.2f, 0.2f, 0.2f, 1.0f);
	spehs::console::addVariable("fps", applicationData->showFps);
	spehs::console::addVariable("maxfps", applicationData->maxFps);

	camera = new spehs::Camera2D();
	batchManager = new spehs::BatchManager(camera);

	spehs::setActiveBatchManager(batchManager);
	textureManager->setDefaultTexture("test_texture.png");
#define distrib 20.0f
	for (unsigned i = 0; i < 400; i++)
	{
		polygons.push_back(batchManager->createPolygon(4, 0, 1.0f, 1.0f));
		polygons.back()->setColor(glm::vec4(1.0f));
		polygons.back()->setPosition(rng->frandom(-distrib, distrib), rng->frandom(-distrib, distrib));
		polygons.back()->setScale(0.1f);
		polygons.back()->setCameraMatrixState(true);
		//polygons.back()->setDrawMode(spehs::OUTLINE);
	}

	for (unsigned i = 0; i < 0; i++)
	{
		points.push_back(batchManager->createPoint());
		points.back()->setColor(1.0f, 1.0f, 1.0f, 1.0f);
		points.back()->setPosition(rng->frandom(-distrib, distrib), rng->frandom(-distrib, distrib));
		points.back()->setCameraMatrixState(true);
	}
	
	while (run)
	{
		spehs::beginFPS();

		update();
		render();

		spehs::endFPS();
	}

	polygons.clear(); //BatchManager cleans up the polygons

	spehs::uninitialize();
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

	if (inputManager->isKeyDown(KEYBOARD_W))
	{
		pos.y += 0.1f;
	}
	if (inputManager->isKeyDown(KEYBOARD_S))
	{
		pos.y -= 0.1f;
	}
	if (inputManager->isKeyDown(KEYBOARD_A))
	{
		pos.x -= 0.1f;
	}
	if (inputManager->isKeyDown(KEYBOARD_D))
	{
		pos.x += 0.1f;
	}
	if (inputManager->isKeyDown(KEYBOARD_Q))
	{
		rotation += 0.1f;
	}
	if (inputManager->isKeyDown(KEYBOARD_E))
	{
		rotation -= 0.1f;
	}
	for (unsigned i = 0; i < polygons.size(); i++)
	{
		polygons[i]->setPosition(pos);
		polygons[i]->setRotation(rotation);
	}

	//CAMERA:
	if (inputManager->isKeyDown(KEYBOARD_LEFT))
	{
		camera->translate(glm::vec2(0.1f, 0.0f));
	}
	if (inputManager->isKeyDown(KEYBOARD_RIGHT))
	{
		camera->translate(glm::vec2(-0.1f, 0.0f));
	}
	if (inputManager->isKeyDown(KEYBOARD_UP))
	{
		camera->translate(glm::vec2(0.0f, -0.1f));
	}
	if (inputManager->isKeyDown(KEYBOARD_DOWN))
	{
		camera->translate(glm::vec2(0.0f, 0.1f));
	}
	if (inputManager->isKeyDown(KEYBOARD_PAGEDOWN))
	{
		cameraScale -= 0.01f;
		camera->scale = cameraScale;
	}
	if (inputManager->isKeyDown(KEYBOARD_PAGEUP))
	{
		cameraScale += 0.01f;
		camera->scale = cameraScale;
	}
}