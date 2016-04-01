
#pragma once

#include "State.h"


class DemoState3D : public State
{
public:
	DemoState3D();
	~DemoState3D();

	bool update();
	void render();
	bool input();

private:

};

