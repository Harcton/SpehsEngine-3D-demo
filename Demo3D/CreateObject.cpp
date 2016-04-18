
#include "CreateObject.h"
#include "SpehsEngine/Transform2D.h"
#include "SpehsEngine/RigidBody2D.h"
#include "SpehsEngine/Sprite.h"
#include "SpehsEngine/Polygon.h"


spehs::GameObject* createPhysicsObject(float _x, float _y, int _numVertices)
{
	spehs::GameObject* result = new spehs::GameObject;
	result->addComponent<spehs::Transform2D>();
	result->addComponent<spehs::RigidBody2D>();
	result->addComponent<spehs::Sprite>();
	result->getComponent<spehs::Sprite>()->setPolygon(_x, _y, _numVertices);
	return result;
}