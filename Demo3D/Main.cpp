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

	SpehsEngine::initialize("3D Demo");
	mainWindow->clearColor(0, 0, 0, 1.0f);
	console->addVariable("fps", applicationData->showFps);
	console->addVariable("maxfps", applicationData->maxFps);

	//Camera controller
	SpehsEngine::Camera3D camera;
	Camera3DController cameraController(camera);

	//Drawables
	SpehsEngine::PolygonBatch square(4, 0.5f, 0.5f);

	bool run = true;
	while (run)
	{
		mainWindow->clearBuffer();
		SpehsEngine::beginFPS();


		//Update
		console->update();
		inputManager->update();
		cameraController.update();
		if (inputManager->isKeyDown(KEYBOARD_ESCAPE))
			run = false;

		//Render
		square.draw();
		console->render();

		SpehsEngine::endFPS();
		SpehsEngine::drawFPS();
		mainWindow->swapBuffers();
	}

	SpehsEngine::uninitialize();
}