#include <SpehsEngine/Time.h>
#include <SpehsEngine/SpehsEngine.h>
#include <SpehsEngine/Console.h>
#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/Window.h>
#include <SpehsEngine/ApplicationData.h>

#include <SpehsEngine/Camera2D.h>
#include <SpehsEngine/BatchManager.h>
#include <SpehsEngine/Polygon.h>

#include <vector>


void update();
void render();
void input();

static bool run = true;
spehs::BatchManager* batchManager = new spehs::BatchManager(&camera);

void main()
{

	std::vector<spehs::Polygon*> polygons;

	polygons.push_back(batchManager->createPolygon());

	spehs::initialize("spehs_engine_testing_zone");
	mainWindow->clearColor(0, 0, 0, 1.0f);
	spehs::console::addVariable("fps", applicationData->showFps);
	spehs::console::addVariable("maxfps", applicationData->maxFps);
	
	while (run)
	{
		spehs::beginFPS();

		update();
		render();

		spehs::endFPS();
	}

	polygons.clear();

	spehs::uninitialize();
}

void update()
{
	spehs::console::update();
	inputManager->update();
	input();
}

void render()
{
	mainWindow->clearBuffer();

	spehs::console::render();
	spehs::drawFPS();
	batchManager->render();

	mainWindow->swapBuffers();
}

void input()
{
	if (inputManager->isKeyDown(KEYBOARD_ESCAPE))
		run = false;
}