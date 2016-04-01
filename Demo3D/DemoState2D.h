
#pragma once

#include "State.h"

#include <vector>

namespace spehs{ class Camera2D; class BatchManager; class Polygon; class Line; class Point; }

class DemoState2D : public State
{
public:
	DemoState2D();
	~DemoState2D();

	bool update();
	void render();

	bool input();

private:
	float distrib;
	spehs::Camera2D* camera;
	spehs::BatchManager* batchManager;

	std::vector<spehs::Polygon*> polygons;
	std::vector<spehs::Point*> points;
	std::vector<spehs::Line*> lines;
};

