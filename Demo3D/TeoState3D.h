#pragma once
#include "State.h"
namespace spehs { class Camera3D; class BatchManager; class Mesh; }


class TeoState3D : public State
{
public:
	TeoState3D();
	~TeoState3D();

	bool update();
	void render();

private:
	spehs::Camera3D* camera;
	spehs::BatchManager* batchManager;
	spehs::Mesh* mesh;
};

