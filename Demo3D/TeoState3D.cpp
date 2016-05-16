#include <SpehsEngine/Console.h>
#include "TeoState3D.h"
bool shake(false);
bool camlight(false);

TeoUniforms::TeoUniforms(spehs::GLSLProgram& glslProgram, TeoState3D& _teoState) : Uniforms(&glslProgram), teoState(_teoState)
{
	//Get every uniform location from the engine
	lightsArray1UniformLocation = glslProgram.getUniformLocation("lights1");
	lightsArray2UniformLocation = glslProgram.getUniformLocation("lights2");
	numLightsUniformLocation = glslProgram.getUniformLocation("numLights");

	//A really dirty way of getting the length of the lights array written in the shader
	teoState.lights1.resize((lightsArray2UniformLocation - 1));
	teoState.lights2.resize((lightsArray2UniformLocation - 1));

	textureLocation = glslProgram.getUniformLocation("tex");
}
void TeoUniforms::setUniforms()
{/**This function is called from the engine and it will let us to pass on the uniform data, required for the shading.*/
	Uniforms::setUniforms();

	//Pass number of lights
	glUniform1f(numLightsUniformLocation, teoState.activeLights);
	spehs::checkOpenGLErrors(__FILE__, __LINE__);

	//Pass lights arrays. See lights array declarations for info what kind of data these vectors hold
	glUniform4fv(lightsArray1UniformLocation, teoState.lights1.size(), (float*)teoState.lights1.data());
	glUniform4fv(lightsArray2UniformLocation, teoState.lights2.size(), (float*)teoState.lights2.data());
	spehs::checkOpenGLErrors(__FILE__, __LINE__);

	//Set texture
	spehs::bind2DTexture(textureDataID, 0);
	spehs::setUniform_int(textureLocation, 0);
}




TeoState3D::TeoState3D() : currentLightIndex(0), activeLights(0), skybox(nullptr)
{
	camera = new spehs::Camera3D();
	camera->setSmoothCamera(true);
	batchManager = new spehs::BatchManager(camera);
	spehs::setActiveBatchManager(batchManager);

	//These variables can be set either true or false during run time with the comman /set <variablename> true/false
	spehs::console::addVariable("shake", shake);//When enabled, models wildly shake around
	spehs::console::addVariable("camlight", camlight);//When enabled, a thin long range yellowish light is emitted from the camera location

	//GLSL program class is used to compile/set up shaders engine side
	spehs::GLSLProgram* glslProgram(new spehs::GLSLProgram);
	glslProgram->compileShaders("Shaders/teo3d.vert", "Shaders/teo3d.frag");
	glslProgram->addAttribute("vertexPosition");
	glslProgram->addAttribute("vertexColor");
	glslProgram->addAttribute("vertexNormal");
	glslProgram->addAttribute("vertexUV");
	glslProgram->addAttribute("lights");
	glslProgram->linkShaders();
	
	TeoUniforms* uniforms(new TeoUniforms(*glslProgram, *this));
	spehs::Shader* shader(new spehs::Shader(5/*This is the shader index used from Model struct constructor*/, glslProgram, uniforms));
	shaderManager->pushShader(shader);

	//Create a number of randomly placed models around the scene
	for (unsigned i = 0; i < 5; i++)
		models.push_back(new Model());

	//skybox = new spehs::SkyBox("Textures/Skybox/skybox", ".png");
	dropLight(0.7f, 0.8f, 1.0f, 1.0f);
}
TeoState3D::~TeoState3D()
{//Dynamic memory responsibilities
	for (unsigned i = 0; i < models.size(); i++)
		delete models[i];
	if (skybox)
		delete skybox;
	delete camera;
	delete batchManager;
}
bool TeoState3D::update()
{
	//Engine update calls
	camera->update();	
	spehs::console::update();
	inputManager->update();
	if (inputManager->isKeyPressed(KEYBOARD_ESCAPE) || inputManager->isQuitRequested())
		return false;

	//Camera movement
	static float speed = 1.0f;
	static float lookSpeed = 0.5f;
	if (inputManager->isKeyDown(KEYBOARD_W))
		camera->move(spehs::FORWARD, speed * spehs::getDeltaTime().asSeconds);
	if (inputManager->isKeyDown(KEYBOARD_S))
		camera->move(spehs::BACKWARD, speed * spehs::getDeltaTime().asSeconds);
	if (inputManager->isKeyDown(KEYBOARD_A))
		camera->move(spehs::LEFT, speed * spehs::getDeltaTime().asSeconds);
	if (inputManager->isKeyDown(KEYBOARD_D))
		camera->move(spehs::RIGHT, speed * spehs::getDeltaTime().asSeconds);
	if (inputManager->isKeyDown(KEYBOARD_LSHIFT))
		camera->move(spehs::DOWN, speed * spehs::getDeltaTime().asSeconds);
	if (inputManager->isKeyDown(KEYBOARD_SPACE))
		camera->move(spehs::UP, speed * spehs::getDeltaTime().asSeconds);
	if (inputManager->isKeyDown(MOUSEBUTTON_RIGHT))
	{
		camera->pitch(inputManager->getMouseMovementX() * spehs::getDeltaTime().asSeconds * lookSpeed);
		camera->yaw(inputManager->getMouseMovementY() * spehs::getDeltaTime().asSeconds * lookSpeed);
	}

	//Update models (shake effect)
	for (unsigned i = 0; i < models.size(); i++)
		models[i]->update();
	
	//Move camera light (if enabled)
	if (camlight)
	{
		if (activeLights == 0)
		{
			activeLights = 1;
			currentLightIndex = 1;
		}
		lights1[0] = glm::vec4(camera->getPosition().x, camera->getPosition().y, camera->getPosition().z, 30.0f);
		lights2[0] = glm::vec4(1.0f, 0.7f, 0.3f, 0.1f);
	}

	//Dropping light
	float alpha(0.4f);
	if (inputManager->isKeyPressed(KEYBOARD_R))
		dropLight(1.0f, 0.3f, 0.3f, alpha);
	if (inputManager->isKeyPressed(KEYBOARD_G))
		dropLight(0.3f, 1.0f, 0.3f, alpha);
	if (inputManager->isKeyPressed(KEYBOARD_B))
		dropLight(0.3f, 0.3f, 1.0f, alpha);
	if (inputManager->isKeyPressed(KEYBOARD_Y))
		dropLight(1.0f, 1.0f, 0.3f, alpha);

	return true;
}
void TeoState3D::render()
{
	spehs::setActiveBatchManager(batchManager);
	batchManager->render();
}
void TeoState3D::dropLight(float r, float g, float b, float a)
{//"Drop light"

	//Set light specs
	lights1[currentLightIndex] = glm::vec4(camera->getPosition().x, camera->getPosition().y, camera->getPosition().z, 50.0f);
	lights2[currentLightIndex] = glm::vec4(r, g, b, a);

	//Increment current light index
	if (++currentLightIndex >= lights1.size())
		currentLightIndex = 0;
	if (++activeLights > lights1.size())
		activeLights = lights1.size();//Limit number of active lights

}