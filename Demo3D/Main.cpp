
#include "State.h"
#include "TeoState3D.h"
#include "DemoState2D.h"
#include "DemoState3D.h"
#include "AudioState2D.h"
#include "PhysicsState2D.h"
#include "ShaderContainer.h"

#include <SpehsEngine/SpehsEngine.h>
#include <SpehsEngine/Window.h>
#include <SpehsEngine/Console.h>
#include <SpehsEngine/ApplicationData.h>
#include <SpehsEngine/Time.h>
#include <SpehsEngine/GUIRectangleList.h>
#include <SpehsEngine/Camera2D.h>
#include <SpehsEngine/BatchManager.h>
#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/Polygon.h>

#include <Windows.h>

static bool run = true;
static bool stateActive = false;

#define BUTTONSIZEX 400
#define BUTTONSIZEY 80

spehs::Camera2D* camera;
spehs::BatchManager* batchManager;


void menu()
{
	mainWindow->clearColor(0.0f, 0.0f, 0.0f, 1.0f);
	spehs::console::addVariable("fps", applicationData->showFps);
	spehs::console::addVariable("maxfps", applicationData->maxFps);

#pragma region Inits
	spehs::setActiveBatchManager(batchManager);

	initShaders();

	State* state = nullptr;
	spehs::GUIRectangle demo3DButton = spehs::GUIRectangle(BUTTONSIZEX, BUTTONSIZEY);

	spehs::Polygon* temp = batchManager->createPolygon(4, 0, applicationData->getWindowWidth() * 0.7f, applicationData->getWindowHeight());
	temp->setTexture("Textures/spehs_engine_demo.png");
	
	temp = batchManager->createPolygon(4, -1, applicationData->getWindowWidth(), applicationData->getWindowHeight());
	temp->setTexture("Textures/background.png");
	
	demo3DButton.setColor(120, 120, 120);
	demo3DButton.setPosition(applicationData->getWindowWidthHalf() - BUTTONSIZEX / 2, applicationData->getWindowHeightHalf() * 0.7f);
	demo3DButton.setString("START");
	demo3DButton.setJustification(GUIRECT_TEXT_JUSTIFICATION_CENTER);
#pragma endregion

	while (run)
	{
		mainWindow->clearColor(0.8f, 0.8f, 0.8f, 1.0f);
		while (!stateActive)
		{
			spehs::setActiveBatchManager(batchManager);

			demo3DButton.update();
			demo3DButton.postUpdate();

			inputManager->update();

			if (inputManager->isQuitRequested())
			{
				run = false;
				break;
			}

			if (inputManager->isKeyPressed(MOUSEBUTTON_LEFT))
			{
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
			if (inputManager->isKeyPressed(KEYBOARD_ESCAPE))
			{
				run = false;
				stateActive = true;
			}

			if (!stateActive)
			{
				spehs::beginFPS();
				spehs::setActiveBatchManager(batchManager);
				mainWindow->renderBegin();
				batchManager->render();
				demo3DButton.DRAW_TEXT();
				mainWindow->renderEnd();
				spehs::endFPS();
			}
		}
		if (!run)
			break;

		spehs::beginFPS();

		stateActive = state->update();

		mainWindow->renderBegin();
		state->render();
		//spehs::console::render();
		spehs::drawFPS();
		mainWindow->renderEnd();

		spehs::endFPS();
	}

	if (state != nullptr)
	{
		delete state;
	}
}


int main(int argc, char *argv [])
{
	spehs::initialize("Spehs Engine Demo");
	camera = new spehs::Camera2D();
	batchManager = new spehs::BatchManager(camera);
	menu();
	delete batchManager;
	delete camera;
	spehs::uninitialize();
}