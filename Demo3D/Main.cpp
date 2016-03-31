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


#define TARGET_POLYGON_COUNT 2000


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
	mainWindow->clearColor(0.2f, 0.3f, 0.3f, 1.0f);
	spehs::console::addVariable("fps", applicationData->showFps);
	spehs::console::addVariable("maxfps", applicationData->maxFps);

	camera = new spehs::Camera2D();
	batchManager = new spehs::BatchManager(camera);

	spehs::setActiveBatchManager(batchManager);
	textureManager->setDefaultTexture("test_texture.png");
	static float distrib = 2000.0f;

	for (unsigned i = 0; i < 0; i++)
	{
		polygons.push_back(batchManager->createPolygon(6, 0, 100.0f, 100.0f));
		//polygons.back()->setColor(glm::vec4(glm::vec3(1.0f - i / 100.0f), 1.0f));
		polygons.back()->setPosition(rng->frandom(-distrib, distrib), rng->frandom(-distrib, distrib));
	}

	for (unsigned i = 0; i < 0; i++)
	{
		lines.push_back(batchManager->createLine(glm::vec2(rng->frandom(-distrib, distrib), rng->frandom(-distrib, distrib)), glm::vec2(rng->frandom(-distrib, distrib), rng->frandom(-distrib, distrib))));
		lines.back()->setColor(glm::vec4(1.0f));
	}

	for (unsigned i = 0; i < 1; i++)
	{
		points.push_back(batchManager->createPoint());
		points.back()->setColor(1.0f, 1.0f, 1.0f, 1.0f);
		points.back()->setPosition(rng->frandom(-distrib, distrib), rng->frandom(-distrib, distrib));
	}
	
	while (run)
	{
		spehs::beginFPS();

		//static unsigned int counter = 0;
		//static unsigned int count = 500;
		//static unsigned int temp;
		//counter += spehs::deltaTime;
		//if (counter > 16)
		//{
		//	counter -= 16;
		////REMOVE
		//	for (unsigned i = 0; i < count; i++)
		//	{
		//		temp = rng->irandom(0, polygons.size() - 1);
		//		polygons[temp]->destroy();
		//		if (polygons[temp] != polygons.back())
		//			polygons[temp] = polygons.back();
		//		polygons.pop_back();
		//	}
		////REPLACE
		//	for (unsigned i = 0; i < count; i++)
		//	{
		//		polygons.push_back(batchManager->createPolygon(6, 0, 100.0f, 100.0f));
		//		polygons.back()->setPosition(rng->frandom(-distrib, distrib), rng->frandom(-distrib, distrib));
		//	}
		//}

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
	run = !inputManager->isKeyDown(KEYBOARD_ESCAPE);

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
		//polygons[i]->setPosition(pos / float(i + 1));
		polygons[i]->setRotation(rotation);
		polygons[i]->worldVertexArray[0].color = { R, 0.0f, 0.0f, 1.0f };
		polygons[i]->worldVertexArray[1].color = { 0.0f, G, 0.0f, 1.0f };
		polygons[i]->worldVertexArray[2].color = { 0.0f, 0.0f, B, 1.0f };
		polygons[i]->worldVertexArray[3].color = { 0.0f, 0.0f, 0.0f, A };
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
		cameraScale -= 0.01f * camera->scale;
		camera->scale = cameraScale;
	}
	if (inputManager->isKeyDown(KEYBOARD_PAGEUP))
	{
		cameraScale += 0.01f * camera->scale;
		camera->scale = cameraScale;
	}
}