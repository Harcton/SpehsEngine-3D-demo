
#include "PhysicsState2D.h"
#include "CreateObject.h"
#include "SpehsEngine/Camera2D.h"
#include "SpehsEngine/BatchManager.h"
#include "SpehsEngine/GameObject.h"
#include "SpehsEngine/InputManager.h"
#include "SpehsEngine/RNG.h"
#include "SpehsEngine/Transform2D.h"
#include "SpehsEngine/RigidBody2D.h"
#include "SpehsEngine/Sprite.h"
#include "SpehsEngine/ApplicationData.h"
#include "SpehsEngine/Polygon.h"
#include "SpehsEngine/SATCollision.h"
#include "SpehsEngine/PhysicsWorld2D.h"
#include "SpehsEngine/Time.h"
#include "SpehsEngine/Geometry.h"

#define PI 3.14159265358f

#define USER_X_SIZE 100.0f
#define USER_Y_SIZE 100.0f

#define THRUSTER_STRENGTH 6.0f

#define SPAWN_INTERVAL 0.3f
#define INITIAL_OBJECTS 10


PhysicsState2D::PhysicsState2D() : collisionPoint(nullptr), gravitySimulation(false), spawnTimer(0.0f), flyingOBJ(nullptr)
{
	camera = new spehs::Camera2D();
	batchManager = new spehs::BatchManager(camera);
	spehs::setActiveBatchManager(batchManager);

	physicsWorld = new spehs::PhysicsWorld2D();

	objects.push_back(createPhysicsObject(USER_X_SIZE, USER_Y_SIZE, 4));
	physicsWorld->addRigidBody(*objects.back()->getComponent<spehs::RigidBody2D>());
	objects.back()->getComponent<spehs::RigidBody2D>()->setStatic(true);
	userOBJ = objects.back();

	for (unsigned i = 0; i < INITIAL_OBJECTS; i++)
	{
		objects.push_back(createPhysicsObject(70.0f, 70.0f, rng->irandom(3, 11)));
		physicsWorld->addRigidBody(*objects.back()->getComponent<spehs::RigidBody2D>());
		objects.back()->getComponent<spehs::Transform2D>()->setPosition(glm::vec2(rng->frandom(-applicationData->getWindowWidthHalf(), applicationData->getWindowWidthHalf()), 
			rng->frandom(-applicationData->getWindowHeightHalf(), applicationData->getWindowHeightHalf())));
	}
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
	//collisionTesting();
	physicsSimulation();
	physicsWorld->update();
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
		if (flyingOBJ == nullptr)
		{
			objects.push_back(createPhysicsObject(100.0f, 100.0f, 3));
			physicsWorld->addRigidBody(*objects.back()->getComponent<spehs::RigidBody2D>());
			objects.back()->setName("FlyingObject");
			flyingOBJ = objects.back();
			flyingOBJ->getComponent<spehs::Sprite>()->sprite->worldVertexArray[1].color.setColor(spehs::RED);
			flyingOBJ->getComponent<spehs::Sprite>()->sprite->worldVertexArray[2].color.setColor(spehs::RED);
		}
		else
		{
			flyingOBJ->getComponent<spehs::Transform2D>()->setPosition(glm::vec2(0.0f, 0.0f));
		}
	}
	if (inputManager->isKeyPressed(KEYBOARD_F3))
	{
		gravitySimulation = !gravitySimulation;
	}

	//User object
	static float rotation = 0;

	if (rotation > 2 * PI)
		rotation = 0.0f;
	if (rotation < 0)
		rotation = 2 * PI;

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
		rotation += 7.0f * spehs::getDeltaTime().asSeconds;
	if (inputManager->isKeyDown(KEYBOARD_E))
		rotation -= 7.0f * spehs::getDeltaTime().asSeconds;
	userOBJ->getComponent<spehs::Transform2D>()->setPosition(inputManager->getMouseCoords() - glm::vec2(applicationData->getWindowWidthHalf(), applicationData->getWindowHeightHalf()));
	userOBJ->getComponent<spehs::Transform2D>()->setRotation(rotation);
	//

	//Flying object
	if (flyingOBJ != nullptr)
	{
		if (inputManager->isKeyDown(KEYBOARD_RIGHT))
		{
			glm::vec2 direction = glm::vec2(cos(flyingOBJ->getComponent<spehs::Sprite>()->sprite->getRotation()), sin(flyingOBJ->getComponent<spehs::Sprite>()->sprite->getRotation()));
			direction = glm::vec2(-direction.y, direction.x) * THRUSTER_STRENGTH;
			flyingOBJ->getComponent<spehs::RigidBody2D>()->applyForceAtPosition(direction * flyingOBJ->getComponent<spehs::RigidBody2D>()->getMass(), glm::vec2(spehs::toVec3(flyingOBJ->getComponent<spehs::Sprite>()->sprite->worldVertexArray[1])));
		}
		if (inputManager->isKeyDown(KEYBOARD_LEFT))
		{
			glm::vec2 direction = glm::vec2(cos(flyingOBJ->getComponent<spehs::Sprite>()->sprite->getRotation()), sin(flyingOBJ->getComponent<spehs::Sprite>()->sprite->getRotation()));
			direction = glm::vec2(-direction.y, direction.x) * THRUSTER_STRENGTH;
			flyingOBJ->getComponent<spehs::RigidBody2D>()->applyForceAtPosition(direction * flyingOBJ->getComponent<spehs::RigidBody2D>()->getMass(), glm::vec2(spehs::toVec3(flyingOBJ->getComponent<spehs::Sprite>()->sprite->worldVertexArray[2])));
		}
	}
	//

	return true;
}
void PhysicsState2D::physicsSimulation()
{
	if (gravitySimulation)
	{
		physicsWorld->setGravity(glm::vec2(0.0f, -9.81f));

		spawnTimer += spehs::getDeltaTime().asSeconds;
		if (spawnTimer > SPAWN_INTERVAL)
		{
			spawnTimer -= SPAWN_INTERVAL;
			objects.push_back(createPhysicsObject(70.0f, 70.0f, rng->irandom(3, 11)));
			physicsWorld->addRigidBody(*objects.back()->getComponent<spehs::RigidBody2D>());
			objects.back()->getComponent<spehs::Transform2D>()->setRotation(rng->frandom(0.0f, 2 * PI));
			objects.back()->getComponent<spehs::Transform2D>()->setPosition(glm::vec2(rng->frandom((float) -applicationData->getWindowWidthHalf(), (float) applicationData->getWindowWidthHalf()), (float) applicationData->getWindowHeightHalf()));
		}
	}
	else
		physicsWorld->setGravity(glm::vec2(0.0f));

	spehs::Transform2D* transform;
	spehs::Sprite* poly;
	//Remove objects that are not inside the screen
	for (unsigned i = 1; i < objects.size(); i++)
	{
		transform = objects[i]->getComponent<spehs::Transform2D>();
		poly = objects[i]->getComponent<spehs::Sprite>();
		if (transform->getPosition().y < (float) -applicationData->getWindowHeightHalf() - poly->sprite->getRadius() * 2.0f ||
			transform->getPosition().y >(float) applicationData->getWindowHeightHalf() + poly->sprite->getRadius() * 2.0f ||
			transform->getPosition().x >(float) applicationData->getWindowWidthHalf() + poly->sprite->getRadius() * 2.0f ||
			transform->getPosition().x < (float) -applicationData->getWindowWidthHalf() - poly->sprite->getRadius()* 2.0f)
		{
			if (objects[i] == flyingOBJ)
				flyingOBJ = nullptr;
			delete objects[i];
			objects[i] = objects.back();
			objects.pop_back();
		}
	}
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
				else
				{
					collisionPoint = spehs::SATMTVCollision(objects[i]->getComponent<spehs::Sprite>()->sprite->worldVertexArray.data(), objects[i]->getComponent<spehs::Sprite>()->sprite->worldVertexArray.size(),
 						glm::vec2(objects[f]->getComponent<spehs::Sprite>()->sprite->getPosition().x, objects[f]->getComponent<spehs::Sprite>()->sprite->getPosition().y), objects[f]->getComponent<spehs::Sprite>()->sprite->getRadius());
				}
			}
			else
			{
				if (objects[i]->getComponent<spehs::Sprite>()->sprite->worldVertexArray.size() > 10) //circle
				{
					collisionPoint = spehs::SATMTVCollision(objects[f]->getComponent<spehs::Sprite>()->sprite->worldVertexArray.data(), objects[f]->getComponent<spehs::Sprite>()->sprite->worldVertexArray.size(),
						glm::vec2(objects[i]->getComponent<spehs::Sprite>()->sprite->getPosition().x, objects[i]->getComponent<spehs::Sprite>()->sprite->getPosition().y), objects[i]->getComponent<spehs::Sprite>()->sprite->getRadius());
				}
				else
				{
					collisionPoint = spehs::SATMTVCollision(objects[i]->getComponent<spehs::Sprite>()->sprite->worldVertexArray.data(), objects[i]->getComponent<spehs::Sprite>()->sprite->worldVertexArray.size(),
						objects[f]->getComponent<spehs::Sprite>()->sprite->worldVertexArray.data(), objects[f]->getComponent<spehs::Sprite>()->sprite->worldVertexArray.size());
				}
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