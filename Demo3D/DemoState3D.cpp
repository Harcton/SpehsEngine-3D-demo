
#include "DemoState3D.h"

#include <SpehsEngine/InputManager.h>


DemoState3D::DemoState3D()
{
}
DemoState3D::~DemoState3D()
{
}


bool DemoState3D::update()
{

	if (!input())
		return false;

	return true;
}

void DemoState3D::render()
{

}

bool DemoState3D::input()
{
	inputManager->update();
	if (inputManager->isKeyDown(KEYBOARD_ESCAPE) || inputManager->isQuitRequested())
		return false;

	return true;
}
