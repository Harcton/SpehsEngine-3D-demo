#include <SpehsEngine/Time.h>
#include <SpehsEngine/SpehsEngine.h>
#include <SpehsEngine/Console.h>
#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/Window.h>
#include <SpehsEngine/ApplicationData.h>
#include <SpehsEngine/PolygonBatch.h>
#include <SpehsEngine/Camera3D.h>
#include "Camera3DController.h"

void main()
{

	spehs::initialize("3D Demo");
	mainWindow->clearColor(0, 0, 0, 1.0f);
	spehs::console::addVariable("fps", applicationData->showFps);
	spehs::console::addVariable("maxfps", applicationData->maxFps);

	//Camera controller
	spehs::Camera3D camera;
	Camera3DController cameraController(camera);

	//Drawables
	spehs::PolygonBatch square(4, 0.5f, 0.5f);

	bool run = true;
	while (run)
	{
		mainWindow->clearBuffer();
		spehs::beginFPS();


		//Update
		spehs::console::update();
		inputManager->update();
		cameraController.update();
		if (inputManager->isKeyDown(KEYBOARD_ESCAPE))
			run = false;

		//Render
		square.draw();
		spehs::console::render();

		spehs::endFPS();
		spehs::drawFPS();
		mainWindow->swapBuffers();
	}

	spehs::uninitialize();
}