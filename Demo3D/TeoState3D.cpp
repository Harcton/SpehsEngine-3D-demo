
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
	glUniform1f(numLightsUniformLocation, teoState.activeLights);
	spehs::checkOpenGLErrors(__FILE__, __LINE__);

	//Pass lights arrays
	glUniform4fv(lightsArray1UniformLocation, teoState.lights1.size(), (float*)teoState.lights1.data());
	glUniform4fv(lightsArray2UniformLocation, teoState.lights2.size(), (float*)teoState.lights2.data());
	spehs::checkOpenGLErrors(__FILE__, __LINE__);

	//Set texture
	spehs::bind2DTexture(textureDataID);
	spehs::setUniform_int(textureLocation, 0);
}




TeoState3D::TeoState3D() : currentLightIndex(1), activeLights(1), skybox(nullptr)
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

	for (unsigned i = 0; i < 2000; i++)
		things.push_back(new Thing());
	//skybox = new spehs::SkyBox("Textures/Skybox/skybox", ".png");
}
TeoState3D::~TeoState3D()
{
	for (unsigned i = 0; i < things.size(); i++)
		delete things[i];
	if (skybox)
		delete skybox;
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

	for (unsigned i = 0; i < things.size(); i++)
	{
		things[i]->update();
	}
	


	//Move camera light
	lights1[0] = glm::vec4(camera->getPosition().x, camera->getPosition().y, camera->getPosition().z, 30.0f);
	lights2[0] = glm::vec4(1.0f, 0.7f, 0.3f, 0.1f);
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
		currentLightIndex = 1;
	if (++activeLights > lights1.size())
		activeLights = lights1.size();	

}