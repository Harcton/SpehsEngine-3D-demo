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
extern bool shake;
/**DEMO DESCRIPTION
This demo will showcase slightly modified blinn phong shader with multiple lights, each with their own intensity, color and radius.
To select the scene, run the program and left click Teo3D. WASD + mouse controls movement, R,G,B keys drop correspondingly colored light on the camera position.
When the number of dropped lights exceeds the set limit, new lights will start to overwrite the previously dropped ones, starting from the oldest.
Engine features were done by Juuso Turunen
The shaders (teo3D.vert, teo3D.frag) and the demo state was done by me.
-Teo Hiltunen
*/





/**
Model class creates the 3D model from a .obj file.
*/
class Model
{
public:
	Model()
	{
		mesh = spehs::getActiveBatchManager()->createMesh("Models/asteroid.obj");///< Loads the model from .obj into model manager
		mesh->setTexture("Textures/moon_rock.png");///< Set texture. Model has the UV coordinates to display this specific texture.
		mesh->setShader(5);///< Set shader based on shader's index

		float dev(10.0f);///< Position randomization range
		mesh->setPosition(spehs::rng::frandom(-dev, dev), spehs::rng::frandom(-dev, dev), spehs::rng::frandom(-dev, dev));///< Randomize position (useful when spawning a number of models)
		mesh->setRotation(spehs::rng::frandom(-TWO_PI, TWO_PI), spehs::rng::frandom(-TWO_PI, TWO_PI), spehs::rng::frandom(-TWO_PI, TWO_PI));

		//These values are used for the shake effect
		movSpeed = spehs::rng::frandom(1.0f, spehs::rng::frandom(20.0f, 100.0f));
		rotSpeed = spehs::rng::frandom(0.5f, spehs::rng::frandom(10.0f, 50.0f));
	}
	~Model()
	{
		mesh->destroy();///< batch manager manages mesh deallocation
	}
	void update()
	{
		if (shake)
		{//Shake effect:	/set shake true
			float r(rotSpeed * spehs::getDeltaTime().asSeconds), m(movSpeed * spehs::getDeltaTime().asSeconds);
			mesh->setRotation(
				mesh->getRotation().x + spehs::rng::frandom(-r, r),
				mesh->getRotation().y + spehs::rng::frandom(-r, r),
				mesh->getRotation().z + spehs::rng::frandom(-r, r));
			mesh->setPosition(
				mesh->getPosition().x + spehs::rng::frandom(-m, m),
				mesh->getPosition().y + spehs::rng::frandom(-m, m),
				mesh->getPosition().z + spehs::rng::frandom(-m, m));
		}
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
	void dropLight(float r, float g, float b, float a);///< Drops a light to current camera position

	/**Lights vectors will always have matching lengths and will store information of a single light.
	The reason I used two vectors was because I couldn't find a basic data type capable of holding 8*4B floats in GLSL.*/
	std::vector<glm::vec4> lights1;///< light position: xyz, light radius: w
	std::vector<glm::vec4> lights2;///< light color rgba

	int currentLightIndex;//What index to drop the next light into
	int activeLights;//Number of lights active. Caps at lights1/2.size()

private:
	//Spehs Engine
	spehs::Camera3D* camera;
	spehs::BatchManager* batchManager;///< Batch manager handles primitive sorting into renderable batches
	std::vector<Model*> models;///< 
	spehs::SkyBox* skybox;///< not used
};

/**Spehs Engine requires user to specify their own uniform class.*/
class TeoUniforms : public spehs::Uniforms
{
public:
	TeoUniforms(spehs::GLSLProgram& glslProgram, TeoState3D& teoState);
	void setUniforms();///< A call to setUniforms() is made from the engine side upon rendering, allowing the user to submit their uniform data.
private:
	TeoState3D& teoState;
	
	//Uniform locations as integer handles, used to submit uniform data to the right location
	GLint textureLocation;
	GLint numLightsUniformLocation;
	GLint lightsArray1UniformLocation;
	GLint lightsArray2UniformLocation;
};