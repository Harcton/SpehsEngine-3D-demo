
#include "State.h"
#include "DemoState2D.h"
#include "DemoState3D.h"

#include <SpehsEngine/SpehsEngine.h>
#include <SpehsEngine/Window.h>
#include <SpehsEngine/Console.h>
#include <SpehsEngine/ApplicationData.h>
#include <SpehsEngine/Time.h>

#include <Windows.h>

static bool run = true;


void main()
{
	spehs::initialize("spehs_engine_testing_zone");
	mainWindow->clearColor(0.2f, 0.3f, 0.3f, 1.0f);
	spehs::console::addVariable("fps", applicationData->showFps);
	spehs::console::addVariable("maxfps", applicationData->maxFps);

	State* state = new DemoState2D();

	while (run)
	{
		spehs::beginFPS();

		run = state->update();

	mainWindow->renderBegin();
		state->render();
		spehs::console::render();
		spehs::drawFPS();
	mainWindow->renderEnd();

		spehs::endFPS();
	}

	spehs::uninitialize();
	Sleep(500);
}