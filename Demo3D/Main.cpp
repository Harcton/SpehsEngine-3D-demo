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


void update();
void render();
void input();

static bool run = true;
spehs::Camera2D* camera = new spehs::Camera2D();
spehs::BatchManager* batchManager = new spehs::BatchManager(camera);

void main()
{
	spehs::initialize("spehs_engine_testing_zone");
	mainWindow->clearColor(0.2f, 0.2f, 0.2f, 1.0f);
	spehs::console::addVariable("fps", applicationData->showFps);
	spehs::console::addVariable("maxfps", applicationData->maxFps);

	spehs::setActiveBatchManager(batchManager);
	textureManager->setDefaultTexture("test_texture.png");
	camera->enableCameraMatrix();

	std::vector<spehs::Polygon*> polygons;
	std::vector<spehs::Point*> points;
	std::vector<spehs::Line*> lines;

	for (unsigned i = 0; i < 500; i++)
	{
		polygons.push_back(batchManager->createPolygon(4, 0, 100.0f, 100.0f));
		polygons.back()->setColor(glm::vec4(1.0f));
		polygons.back()->setPosition(0.0f, 0.0f);
	}

	for (unsigned i = 0; i < 1; i++)
	{
		points.push_back(batchManager->createPoint());
		points.back()->setColor(1.0f, 1.0f, 1.0f, 1.0f);
		points.back()->setPosition(rng->frandom(-10.0f, 10.0f), rng->frandom(-10.0f, 10.0f));
		points.back()->setCameraMatrixState(false);
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

	batchManager->render();

	spehs::console::render();
	spehs::drawFPS();

	mainWindow->renderEnd();
}

void input()
{
	if (inputManager->isKeyDown(KEYBOARD_ESCAPE))
		run = false;
}