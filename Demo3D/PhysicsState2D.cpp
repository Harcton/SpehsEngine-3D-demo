
#include "PhysicsState2D.h"
#include "CreateObject.h"
#include "SpehsEngine/Camera2D.h"
#include "SpehsEngine/BatchManager.h"
#include "SpehsEngine/GameObject.h"
#include "SpehsEngine/InputManager.h"
#include "SpehsEngine/RNG.h"
#include "SpehsEngine/Transform2D.h"
#include "SpehsEngine/Sprite.h"
#include "SpehsEngine/ApplicationData.h"
#include "SpehsEngine/Polygon.h"
#include "SpehsEngine/SATCollision.h"

#define PI 3.14159265358f

#define USER_X_SIZE 100.0f
#define USER_Y_SIZE 100.0f


PhysicsState2D::PhysicsState2D() : collisionPoint(nullptr)
{
	camera = new spehs::Camera2D();
	batchManager = new spehs::BatchManager(camera);
	spehs::setActiveBatchManager(batchManager);

	objects.push_back(createPhysicsObject(USER_X_SIZE, USER_Y_SIZE, 4));
	userOBJ = objects.back();
}
PhysicsState2D::~PhysicsState2D()
{
	for (unsigned i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
	delete batchManager;
	delete camera;
}


bool PhysicsState2D::update()
{
	if (!input())
	{
		return false;
	}
	collisionTesting();
	for (unsigned i = 0; i < objects.size(); i++)
	{
		objects[i]->update();
	}
	return true;
}
void PhysicsState2D::render()
{
	spehs::setActiveBatchManager(batchManager);
	batchManager->render();
}
bool PhysicsState2D::input()
{
	inputManager->update();
	if (inputManager->isQuitRequested() || inputManager->isKeyPressed(KEYBOARD_ESCAPE))
	{
		return false;
	}

	//spawn
	if (inputManager->isKeyPressed(KEYBOARD_SPACE))
	{
		objects.push_back(createPhysicsObject(rng->frandom(40.0f, 200.0f), rng->frandom(40.0f, 200.0f), rng->irandom(3, 11)));
		objects.back()->getComponent<spehs::Transform2D>()->setRotation(rng->frandom(0.0f, 2 * PI));
		objects.back()->getComponent<spehs::Transform2D>()->setPosition(glm::vec2(rng->frandom((float) -applicationData->getWindowWidthHalf(), (float) applicationData->getWindowWidthHalf()), rng->frandom((float) -applicationData->getWindowHeightHalf(), (float) applicationData->getWindowHeightHalf())));
	}

	//User object
	static float rotation = 0;

	if (inputManager->isKeyPressed(KEYBOARD_3))
		userOBJ->getComponent<spehs::Sprite>()->setPolygon(USER_X_SIZE, USER_Y_SIZE, 3);
	if (inputManager->isKeyPressed(KEYBOARD_4))
		userOBJ->getComponent<spehs::Sprite>()->setPolygon(USER_X_SIZE, USER_Y_SIZE, 4);
	if (inputManager->isKeyPressed(KEYBOARD_5))
		userOBJ->getComponent<spehs::Sprite>()->setPolygon(USER_X_SIZE, USER_Y_SIZE, 5);
	if (inputManager->isKeyPressed(KEYBOARD_6))
		userOBJ->getComponent<spehs::Sprite>()->setPolygon(USER_X_SIZE, USER_Y_SIZE, 6);
	if (inputManager->isKeyPressed(KEYBOARD_7))
		userOBJ->getComponent<spehs::Sprite>()->setPolygon(USER_X_SIZE, USER_Y_SIZE, 7);
	if (inputManager->isKeyPressed(KEYBOARD_8))
		userOBJ->getComponent<spehs::Sprite>()->setPolygon(USER_X_SIZE, USER_Y_SIZE, 8);
	if (inputManager->isKeyPressed(KEYBOARD_9))
		userOBJ->getComponent<spehs::Sprite>()->setPolygon(USER_X_SIZE, USER_Y_SIZE, 9);
	if (inputManager->isKeyPressed(KEYBOARD_0))
		userOBJ->getComponent<spehs::Sprite>()->setPolygon(USER_X_SIZE, USER_Y_SIZE, 25);

	if (inputManager->isKeyDown(KEYBOARD_Q))
		rotation += 0.01;
	if (inputManager->isKeyDown(KEYBOARD_E))
		rotation -= 0.01;
	userOBJ->getComponent<spehs::Transform2D>()->setPosition(inputManager->getMouseCoords() - glm::vec2(applicationData->getWindowWidthHalf(), applicationData->getWindowHeightHalf()));
	userOBJ->getComponent<spehs::Transform2D>()->setRotation(rotation);
	return true;
}
void PhysicsState2D::collisionTesting()
{
	//reset colors to white
	for (unsigned i = 0; i < objects.size(); i++)
	{
		objects[i]->getComponent<spehs::Sprite>()->sprite->setColor(spehs::WHITE);
	}
	//remove collision visuals
	for (unsigned i = 0; i < collisionPointVisuals.size(); i++)
	{
		collisionPointVisuals[i]->destroy();
	}
	collisionPointVisuals.clear();

	for (unsigned f = 0; f < objects.size(); f++)
	{
		for (unsigned i = f + 1; i < objects.size(); i++)
		{
			if (objects[f]->getComponent<spehs::Sprite>()->sprite->worldVertexArray.size() > 10) //if circle
			{
				if (objects[i]->getComponent<spehs::Sprite>()->sprite->worldVertexArray.size() > 10) //both
				{
					collisionPoint = spehs::CircleMTVCollision(glm::vec2(objects[i]->getComponent<spehs::Sprite>()->sprite->getPosition().x, objects[i]->getComponent<spehs::Sprite>()->sprite->getPosition().y), objects[f]->getComponent<spehs::Sprite>()->sprite->getRadius(),
						glm::vec2(objects[f]->getComponent<spehs::Sprite>()->sprite->getPosition().x, objects[f]->getComponent<spehs::Sprite>()->sprite->getPosition().y), objects[f]->getComponent<spehs::Sprite>()->sprite->getRadius());
				}
				collisionPoint = spehs::SATMTVCollision(objects[i]->getComponent<spehs::Sprite>()->sprite->worldVertexArray.data(), objects[i]->getComponent<spehs::Sprite>()->sprite->worldVertexArray.size(),
					glm::vec2(objects[f]->getComponent<spehs::Sprite>()->sprite->getPosition().x, objects[f]->getComponent<spehs::Sprite>()->sprite->getPosition().y), objects[f]->getComponent<spehs::Sprite>()->sprite->getRadius());
			}
			else
			{
				if (objects[i]->getComponent<spehs::Sprite>()->sprite->worldVertexArray.size() > 10) //circle
				{
					collisionPoint = spehs::SATMTVCollision(objects[f]->getComponent<spehs::Sprite>()->sprite->worldVertexArray.data(), objects[f]->getComponent<spehs::Sprite>()->sprite->worldVertexArray.size(),
						glm::vec2(objects[i]->getComponent<spehs::Sprite>()->sprite->getPosition().x, objects[i]->getComponent<spehs::Sprite>()->sprite->getPosition().y), objects[i]->getComponent<spehs::Sprite>()->sprite->getRadius());
				}
				collisionPoint = spehs::SATMTVCollision(objects[i]->getComponent<spehs::Sprite>()->sprite->worldVertexArray.data(), objects[i]->getComponent<spehs::Sprite>()->sprite->worldVertexArray.size(),
					objects[f]->getComponent<spehs::Sprite>()->sprite->worldVertexArray.data(), objects[f]->getComponent<spehs::Sprite>()->sprite->worldVertexArray.size());
			}

			if (collisionPoint != nullptr)
			{
				objects[f]->getComponent<spehs::Sprite>()->sprite->setColor(spehs::RED);
				objects[i]->getComponent<spehs::Sprite>()->sprite->setColor(spehs::RED);

				collisionPointVisuals.push_back(batchManager->createPolygon(20, 0, 5.0f, 5.0f));
				collisionPointVisuals.back()->setPosition(collisionPoint->point);

				delete collisionPoint;
				collisionPoint = nullptr;
			}
		}
	}
}