#include <SpehsEngine/RNG.h>
#include <SpehsEngine/Console.h>
#include <SpehsEngine/Camera3D.h>
#include <SpehsEngine/BatchManager.h>
#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/ShaderManager.h>
#include <SpehsEngine/OpenGLError.h>
#include <SpehsEngine/Mesh.h>
#include <SpehsEngine/Time.h>
#include "TeoState3D.h"

TeoUniforms::TeoUniforms(spehs::GLSLProgram& glslProgram, TeoState3D& _teoState) : Uniforms(&glslProgram), teoState(_teoState)
{
	lightsArray1UniformLocation = glslProgram.getUniformLocation("lights1");
	lightsArray2UniformLocation = glslProgram.getUniformLocation("lights2");
	numLightsUniformLocation = glslProgram.getUniformLocation("numLights");
	teoState.lights1.resize((numLightsUniformLocation - 1)/2);
	teoState.lights2.resize((numLightsUniformLocation - 1)/2);
	textureLocation = glslProgram.getUniformLocation("tex");
}
void TeoUniforms::setUniforms()
{
	Uniforms::setUniforms();

	//Pass number of lights
	glUniform1f(numLightsUniformLocation, teoState.lights1.size());
	spehs::checkOpenGLErrors(__FILE__, __LINE__);

	//Pass lights arrays
	glUniform4fv(lightsArray1UniformLocation, teoState.lights1.size(), (float*)teoState.lights1.data());
	glUniform4fv(lightsArray2UniformLocation, teoState.lights2.size(), (float*)teoState.lights2.data());
	spehs::checkOpenGLErrors(__FILE__, __LINE__);

	//Set texture
	spehs::bind2DTexture(textureDataID);
	spehs::setUniform_int(textureLocation, 0);
}




TeoState3D::TeoState3D() : currentLightIndex(1)
{
	camera = new spehs::Camera3D();
	camera->setSmoothCamera(true);
	batchManager = new spehs::BatchManager(camera);
	spehs::setActiveBatchManager(batchManager);

	spehs::GLSLProgram* glslProgram(new spehs::GLSLProgram);
	glslProgram->compileShaders("Shaders/teo3d.vert", "Shaders/teo3d.frag");
	glslProgram->addAttribute("vertexPosition");
	glslProgram->addAttribute("vertexColor");
	glslProgram->addAttribute("vertexNormal");
	glslProgram->addAttribute("vertexUV");
	glslProgram->addAttribute("lights");
	glslProgram->linkShaders();
	TeoUniforms* uniforms(new TeoUniforms(*glslProgram, *this));
	spehs::Shader* shader(new spehs::Shader(5, glslProgram, uniforms));
	shaderManager->pushShader(shader);

	for (unsigned i = 0; i < 30; i++)
	{
		float dev(5.0f);
		meshes.push_back(batchManager->createMesh("Models/duck_ship.obj"));
		if (i == 0)
		{
			meshes.back()->setPosition(0, 0, 0);
		}
		else
		{
			meshes.back()->setPosition(rng->frandom(-dev, dev), rng->frandom(-dev, dev), rng->frandom(-dev, dev));
		}
		meshes.back()->setColor(0, 0, 0, 255);
		meshes.back()->setShader(5);
		//meshes.back()->setTexture("Textures/moon_rock.png");
	}
}
TeoState3D::~TeoState3D()
{
	for (unsigned i = 0; i < meshes.size(); i++)
		meshes[i]->destroy();
	delete camera;
	delete batchManager;
}
bool TeoState3D::update()
{
	camera->update();	
	spehs::console::update();
	inputManager->update();
	if (inputManager->isKeyPressed(KEYBOARD_ESCAPE) || inputManager->isQuitRequested())
		return false;










	static float speed = 1.0f;
	static float lookSpeed = 0.5f;
	if (inputManager->isKeyDown(KEYBOARD_W))
	{
		camera->move(spehs::FORWARD, speed * spehs::getDeltaTime().asSeconds);
	}
	if (inputManager->isKeyDown(KEYBOARD_S))
	{
		camera->move(spehs::BACKWARD, speed * spehs::getDeltaTime().asSeconds);
	}
	if (inputManager->isKeyDown(KEYBOARD_A))
	{
		camera->move(spehs::LEFT, speed * spehs::getDeltaTime().asSeconds);
	}
	if (inputManager->isKeyDown(KEYBOARD_D))
	{
		camera->move(spehs::RIGHT, speed * spehs::getDeltaTime().asSeconds);
	}
	if (inputManager->isKeyDown(KEYBOARD_LSHIFT))
	{
		camera->move(spehs::DOWN, speed * spehs::getDeltaTime().asSeconds);
	}
	if (inputManager->isKeyDown(KEYBOARD_SPACE))
	{
		camera->move(spehs::UP, speed * spehs::getDeltaTime().asSeconds);
	}
	camera->pitch(inputManager->getMouseMovementX() * spehs::getDeltaTime().asSeconds * lookSpeed);
	camera->yaw(inputManager->getMouseMovementY() * spehs::getDeltaTime().asSeconds * lookSpeed);
	


	//Move camera light
	lights1[0] = glm::vec4(camera->getPosition().x, camera->getPosition().y, camera->getPosition().z, 20.0f);
	lights2[0] = glm::vec4(1.0f, 0.7f, 0.3f, 1.0f);
	//Dropping light
	if (inputManager->isKeyPressed(KEYBOARD_F))
	{//"Drop light"

		//Set light specs
		lights1[currentLightIndex] = glm::vec4(camera->getPosition().x, camera->getPosition().y, camera->getPosition().z, 7.0f);
		lights2[currentLightIndex] = glm::vec4(rng->frandom(0.0f, 1.0f), rng->frandom(0.0f, 1.0f), rng->frandom(0.0f, 1.0f), rng->frandom(0.5f, 0.9f));

		//Increment current light index
		if (++currentLightIndex >= lights1.size())
			currentLightIndex = 1;
	}








	return true;
}
void TeoState3D::render()
{
	spehs::setActiveBatchManager(batchManager);
	batchManager->render();
}