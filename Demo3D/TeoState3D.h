#pragma once
#include <SpehsEngine/RNG.h>
#include <SpehsEngine/Console.h>
#include <SpehsEngine/Camera3D.h>
#include <SpehsEngine/BatchManager.h>
#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/ShaderManager.h>
#include <SpehsEngine/OpenGLError.h>
#include <SpehsEngine/Mesh.h>
#include <SpehsEngine/Time.h>
#include <SpehsEngine/SkyBox.h>
#include "../SpehsEngine/Dependencies/include/GL/glew.h"
#include <SpehsEngine/GLSLProgram.h>
#include <SpehsEngine/ShaderManager.h>
#include "State.h"
#define TWO_PI 6.28318530718f
namespace spehs { class Camera3D; class BatchManager; class Mesh; }

struct Thing
{
	Thing()
	{
		float dev(50.0f);
		//mesh = spehs::getActiveBatchManager()->createMesh("Models/asteroid.obj");
		//mesh->setTexture("Textures/moon_rock.png");
		mesh = spehs::getActiveBatchManager()->createMesh("Models/duck_ship.obj");
		mesh->setPosition(rng->frandom(-dev, dev), rng->frandom(-dev, dev), rng->frandom(-dev, dev));
		mesh->setColor((unsigned char)rng->irandom(0, 255), (unsigned char)rng->irandom(0, 255), (unsigned char)rng->irandom(0, 255));

		mesh->setColor(200, 0, 0, 255);
		mesh->setShader(5);
		mesh->setRotation(rng->frandom(-TWO_PI, TWO_PI), rng->frandom(-TWO_PI, TWO_PI), rng->frandom(-TWO_PI, TWO_PI));
		movSpeed = rng->frandom(1.0f, rng->frandom(20.0f, 100.0f));
		rotSpeed = rng->frandom(0.5f, rng->frandom(10.0f, 50.0f));
	}
	~Thing()
	{
		mesh->destroy();
	}
	void update()
	{
		float r(rotSpeed * spehs::getDeltaTime().asSeconds), m(movSpeed * spehs::getDeltaTime().asSeconds);
		mesh->setRotation(
			mesh->getRotation().x + rng->frandom(-r, r),
			mesh->getRotation().y + rng->frandom(-r, r),
			mesh->getRotation().z + rng->frandom(-r, r));
		mesh->setPosition(
			mesh->getPosition().x + rng->frandom(-m, m),
			mesh->getPosition().y + rng->frandom(-m, m),
			mesh->getPosition().z + rng->frandom(-m, m));
	}
	spehs::Mesh* mesh;
	float movSpeed;
	float rotSpeed;
};
class TeoState3D : public State
{
public:
	TeoState3D();
	~TeoState3D();

	bool update();
	void render();

	//Lights
	void dropLight(float r, float g, float b, float a);
	std::vector<glm::vec4> lights1;//xyz + radius
	std::vector<glm::vec4> lights2;//rgba
	int currentLightIndex;
	int activeLights;

private:
	spehs::Camera3D* camera;
	spehs::BatchManager* batchManager;
	spehs::SkyBox* skybox;
	std::vector<Thing*> things;
};






class TeoUniforms : public spehs::Uniforms
{
public:
	TeoUniforms(spehs::GLSLProgram& glslProgram, TeoState3D& teoState);
	void setUniforms();
private:
	TeoState3D& teoState;
	GLint textureLocation;
	GLint numLightsUniformLocation;
	GLint lightsArray1UniformLocation;
	GLint lightsArray2UniformLocation;
};