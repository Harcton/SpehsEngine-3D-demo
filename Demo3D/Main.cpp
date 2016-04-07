
#include "State.h"
#include "DemoState2D.h"
#include "DemoState3D.h"
#include "AudioState2D.h"
#include "PhysicsState2D.h"

#include <SpehsEngine/SpehsEngine.h>
#include <SpehsEngine/Window.h>
#include <SpehsEngine/Console.h>
#include <SpehsEngine/ApplicationData.h>
#include <SpehsEngine/Time.h>
#include <SpehsEngine/GUIRectangleList.h>
#include <SpehsEngine/Camera2D.h>
#include <SpehsEngine/BatchManager.h>
#include <SpehsEngine/InputManager.h>

#include <Windows.h>

static bool run = true;
static bool stateActive = false;

#define BUTTONSIZEX 400
#define BUTTONSIZEY 80

spehs::Camera2D* camera;
spehs::BatchManager* batchManager;


void menu()
{
	mainWindow->clearColor(0.35f, 0.35f, 0.35f, 1.0f);
	spehs::console::addVariable("fps", applicationData->showFps);
	spehs::console::addVariable("maxfps", applicationData->maxFps);

#pragma region Inits
	spehs::setActiveBatchManager(batchManager);

	State* state = nullptr;
	spehs::GUIRectangle exitButton = spehs::GUIRectangle(BUTTONSIZEX, BUTTONSIZEY);
	spehs::GUIRectangle demo2DButton = spehs::GUIRectangle(BUTTONSIZEX, BUTTONSIZEY);
	spehs::GUIRectangle audio2DButton = spehs::GUIRectangle(BUTTONSIZEX, BUTTONSIZEY);
	spehs::GUIRectangle physics2DButton = spehs::GUIRectangle(BUTTONSIZEX, BUTTONSIZEY);
	spehs::GUIRectangle demo3DButton = spehs::GUIRectangle(BUTTONSIZEX, BUTTONSIZEY);

	exitButton.setColor(120, 0, 0);
	exitButton.setPosition(applicationData->getWindowWidthHalf() - BUTTONSIZEX / 2, applicationData->getWindowHeight() * 0.85f);
	exitButton.setString("EXIT");

	demo2DButton.setColor(0, 120, 0);
	demo2DButton.setPosition(applicationData->getWindowWidth() * 0.7 - BUTTONSIZEX / 2, applicationData->getWindowHeight() * 0.6f);
	demo2DButton.setString("GRAPHICS 2D");

	audio2DButton.setColor(70, 70, 70);
	audio2DButton.setPosition(applicationData->getWindowWidth() * 0.7 - BUTTONSIZEX / 2, applicationData->getWindowHeight() * 0.45f);
	audio2DButton.setString("AUDIO 2D");

	physics2DButton.setColor(0, 50, 120);
	physics2DButton.setPosition(applicationData->getWindowWidth() * 0.7 - BUTTONSIZEX / 2, applicationData->getWindowHeight() * 0.3f);
	physics2DButton.setString("PHYSICS 2D");

	demo3DButton.setColor(10, 80, 10);
	demo3DButton.setPosition(applicationData->getWindowWidth() * 0.3 - BUTTONSIZEX / 2, applicationData->getWindowHeight() * 0.6f);
	demo3DButton.setString("GRAPHICS 3D");
#pragma endregion

	while (run)
	{
		while (!stateActive)
		{
			spehs::setActiveBatchManager(batchManager);

			exitButton.update();
			exitButton.postUpdate();
			demo2DButton.update();
			demo2DButton.postUpdate();
			audio2DButton.update();
			audio2DButton.postUpdate();
			physics2DButton.update();
			physics2DButton.postUpdate();
			demo3DButton.update();
			demo3DButton.postUpdate();

			inputManager->update();

			if (inputManager->isKeyPressed(MOUSEBUTTON_LEFT))
			{
				if (exitButton.getMouseHover())
				{
					stateActive = true;
					run = false;
				}
				if (demo2DButton.getMouseHover())
				{
					stateActive = true;
					if (state != nullptr)
					{
						delete state;
					}
					state = new DemoState2D;
				}
				if (audio2DButton.getMouseHover())
				{
					stateActive = true;
					if (state != nullptr)
					{
						delete state;
					}
					state = new AudioState2D;
				}
				if (physics2DButton.getMouseHover())
				{
					stateActive = true;
					if (state != nullptr)
					{
						delete state;
					}
					state = new PhysicsState2D;
				}
				if (demo3DButton.getMouseHover())
				{
					stateActive = true;
					if (state != nullptr)
					{
						delete state;
					}
					state = new DemoState3D;
				}
			}

			spehs::setActiveBatchManager(batchManager);
			mainWindow->renderBegin();
			batchManager->render();
			exitButton.DRAW_TEXT();
			demo2DButton.DRAW_TEXT();
			audio2DButton.DRAW_TEXT();
			physics2DButton.DRAW_TEXT();
			demo3DButton.DRAW_TEXT();
			mainWindow->renderEnd();
		}
		if (!run)
			break;

		spehs::beginFPS();

		stateActive = state->update();

		mainWindow->renderBegin();
		state->render();
		spehs::console::render();
		spehs::drawFPS();
		mainWindow->renderEnd();

		spehs::endFPS();
	}

	if (state != nullptr)
	{
		delete state;
	}
}


void main()
{
	spehs::initialize("SpehsEngine Testing");
	camera = new spehs::Camera2D();
	batchManager = new spehs::BatchManager(camera);
	menu();
	delete batchManager;
	delete camera;
	spehs::uninitialize();
}