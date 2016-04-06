
#pragma once

#include "State.h"


class PhysicsState2D : public State
{
public:
	PhysicsState2D();
	~PhysicsState2D();

	bool update();
	void render();

private:

};

