
#include "DemoState3D.h"
#include "ShaderContainer.h"

#include <SpehsEngine/Window.h>
#include <SpehsEngine/InputManager.h>
#include <SpehsEngine/Camera3D.h>
#include <SpehsEngine/Camera2D.h>
#include <SpehsEngine/BatchManager.h>
#include <SpehsEngine/Mesh.h>
#include <SpehsEngine/RNG.h>
#include <SpehsEngine/Time.h>
#include <SpehsEngine/Console.h>
#include <SpehsEngine/SkyBox.h>
#include <SpehsEngine/TextureManager.h>
#include <SpehsEngine/Geometry.h>
#include <SpehsEngine/Vertex.h>
#include <SpehsEngine/PostProcessing.h>
#include <SpehsEngine/ApplicationData.h>
#include <SpehsEngine/Polygon.h>
#include <SpehsEngine/GUIRectangle.h>

#include <glm/vec3.hpp>
#include <glm/trigonometric.hpp>

#include <iostream>


#define ENVIRONMENT_SCALE 7.0f


DemoState3D::DemoState3D() : position(0.0f, 10.0f, 0.0f), rotation(0.0f)
{
	camera = new spehs::Camera3D();
	camera->setSmoothCamera(true);
	camera->setFar(50000.0f);
	camera->setNear(5.0f);
	camera->setFOV(38.0f);
	camera->setPosition(glm::vec3(-50.0f, 50.0f, -50.0f));
	batchManager = new spehs::BatchManager(camera);

	load();

	spehs::setActiveBatchManager(batchManager);
	spehs::graphics::postproc::enablePostProcessing();
	spehs::graphics::postproc::setPostProcessingShader((int) ShaderName::Bloom);
}
DemoState3D::~DemoState3D()
{
	delete skyBox;
	for (unsigned i = 0; i < meshes.size(); i++)
	{
		meshes[i]->destroy();
	}

	delete camera;
	delete batchManager;
	spehs::graphics::postproc::disablePostProcessing();
}
void DemoState3D::load()
{
	spehs::Camera2D loaderCam;
	spehs::BatchManager loader(&loaderCam);
	spehs::setActiveBatchManager(&loader);

	loaderCam.update();

	spehs::Polygon* temp = loader.createPolygon(4, -10, applicationData->getWindowWidth(), applicationData->getWindowHeight());
	temp->setTexture("Textures/background.png");

	spehs::GUIRectangle loading = spehs::GUIRectangle(400, 80);
	loading.setColor(120, 120, 120);
	loading.setPosition(applicationData->getWindowWidthHalf() - 400 / 2, applicationData->getWindowHeightHalf() * 0.7f);
	loading.setJustification(GUIRECT_TEXT_JUSTIFICATION_CENTER);
	
	auto render = [&]()
	{
		spehs::setActiveBatchManager(&loader);
		mainWindow->renderBegin();
		loader.render();
		loading.DRAW_TEXT();
		mainWindow->renderEnd();
	};

	loading.setString("Loading: Sky");
	loading.update();
	loading.postUpdate();
	render();

	spehs::setActiveBatchManager(batchManager);
	skyBox = new spehs::SkyBox("Textures/Simple/sky", ".png");
	skyBox->setShader((int) ShaderName::Sky);

	loading.setString("Loading: Pillars");
	loading.update();
	loading.postUpdate();
	render();

	spehs::setActiveBatchManager(batchManager);
	meshes.push_back(batchManager->createMesh("Models/pillars.obj"));
	meshes.back()->setTexture("Textures/pillar_texture.jpg");
	meshes.back()->setShader((int) ShaderName::Pillar);
	meshes.back()->setScale(ENVIRONMENT_SCALE);

	loading.setString("Loading: Land");
	loading.update();
	loading.postUpdate();
	render();

	spehs::setActiveBatchManager(batchManager);
	meshes.push_back(batchManager->createMesh("Models/environment_land.obj"));
	meshes.back()->setTexture("Textures/grass.jpg");
	meshes.back()->setScale(ENVIRONMENT_SCALE);
	meshes.back()->setPosition(0.0f, 0.0f, 0.0f);
	meshes.back()->setShader((int) ShaderName::Environment);
	land = meshes.back();

	loading.setString("Loading: Rocks");
	loading.update();
	loading.postUpdate();
	render();

	spehs::setActiveBatchManager(batchManager);
	meshes.push_back(batchManager->createMesh("Models/environment_rock.obj"));
	meshes.back()->setTexture("Textures/rock_texture.png");
	meshes.back()->setScale(ENVIRONMENT_SCALE);
	meshes.back()->setPosition(0.0f, 0.0f, 0.0f);
	meshes.back()->setShader((int) ShaderName::Rocks);

	loading.setString("Loading: Sand");
	loading.update();
	loading.postUpdate();
	render();

	spehs::setActiveBatchManager(batchManager);
	meshes.push_back(batchManager->createMesh("Models/environment_sand.obj"));
	meshes.back()->setTexture("Textures/sand.jpg");
	meshes.back()->setScale(ENVIRONMENT_SCALE);
	meshes.back()->setPosition(0.0f, 0.1f, 0.0f);
	meshes.back()->setShader((int) ShaderName::Environment);

	loading.setString("Loading: Water");
	loading.update();
	loading.postUpdate();
	render();

	spehs::setActiveBatchManager(batchManager);
	meshes.push_back(batchManager->createMesh("Models/water.obj"));
	meshes.back()->setScale(ENVIRONMENT_SCALE);
	meshes.back()->setPosition(0.0f, 0.1f, 0.0f);
	meshes.back()->setShader((int) ShaderName::Water);
	meshes.back()->setTexture("Textures/sand.jpg");

	spehs::setActiveBatchManager(batchManager);
	meshes.push_back(batchManager->createMesh("Models/far_water.obj"));
	meshes.back()->setScale(ENVIRONMENT_SCALE);
	meshes.back()->setPosition(0.0f, 0.0f, 0.0f);
	meshes.back()->setShader((int) ShaderName::FarWater);
	meshes.back()->setTexture("Textures/sand.jpg");

	loading.setString("Loading: Grass");
	loading.update();
	loading.postUpdate();
	render();

	spehs::setActiveBatchManager(batchManager);
	land->updateVertices();
	const glm::vec3 up(0.0f, 1.0f, 0.0f);
	for (unsigned i = 0; i < land->worldVertexArray.size(); i++)
	{
		if (spehs::rng::irandom(0, 3))
		{
			if (land->worldVertexArray[i].position.y > 9.0f && spehs::toVec3(land->vertexArray[i].normal).y > 0.85f)
			{
				meshes.push_back(batchManager->createMesh("Models/grass.obj"));
				meshes.back()->setTexture("Textures/grass_blades.png");
				meshes.back()->setShader((int) ShaderName::Grass);
				meshes.back()->setBackFaceCulling(false);
				meshes.back()->setPosition(land->worldVertexArray[i].position.x, land->worldVertexArray[i].position.y + 18.0f, land->worldVertexArray[i].position.z);
				meshes.back()->setRotation(0.0, spehs::rng::frandom(0.0f, PI), PI);
				//meshes.back()->setRotation(spehs::getRotation(up, spehs::toVec3(land->vertexArray[i].normal)) + meshes.back()->getRotation());
				meshes.back()->setScale(spehs::rng::frandom(9.0f, 11.0f), spehs::rng::frandom(12.0f, 18.0f), spehs::rng::frandom(9.0f, 11.0f));
			}
		}
	}

	temp->destroy();

	shaderManager->getShader((int) ShaderName::Water)->getCustomUniforms<WaterUniforms>()->reflectionTextureID = textureManager->getCubeMapData(skyBox->getCubeMapHash())->textureDataID;
	shaderManager->getShader((int) ShaderName::Water)->getCustomUniforms<WaterUniforms>()->heightMapTextureID = textureManager->getTextureData("Textures/water_height_map.png")->textureDataID;
	shaderManager->getShader((int) ShaderName::FarWater)->getCustomUniforms<WaterUniforms>()->reflectionTextureID = textureManager->getCubeMapData(skyBox->getCubeMapHash())->textureDataID;
	shaderManager->getShader((int) ShaderName::FarWater)->getCustomUniforms<WaterUniforms>()->heightMapTextureID = textureManager->getTextureData("Textures/far_water_height_map.png")->textureDataID;
	shaderManager->getShader((int) ShaderName::Pillar)->getCustomUniforms<PillarUniforms>()->reflectionTextureID = textureManager->getCubeMapData(skyBox->getCubeMapHash())->textureDataID;
	shaderManager->getShader((int) ShaderName::Pillar)->getCustomUniforms<PillarUniforms>()->bumpMapTextureID = textureManager->getTextureData("Textures/pillar_bump_map.png")->textureDataID;
	shaderManager->getShader((int) ShaderName::Rocks)->getCustomUniforms<RocksUniforms>()->bumpMapTextureID = textureManager->getTextureData("Textures/rock_bump_map.png")->textureDataID;
}


bool DemoState3D::update()
{
	shaderManager->getShader((int) ShaderName::Water)->getCustomUniforms<WaterUniforms>()->lightPosition = camera->getPosition();
	shaderManager->getShader((int) ShaderName::FarWater)->getCustomUniforms<WaterUniforms>()->lightPosition = camera->getPosition();
	shaderManager->getShader((int) ShaderName::Environment)->getCustomUniforms<DemoUniforms>()->lightPosition = camera->getPosition();
	shaderManager->getShader((int) ShaderName::Pillar)->getCustomUniforms<PillarUniforms>()->lightPosition = camera->getPosition();
	shaderManager->getShader((int) ShaderName::Grass)->getCustomUniforms<DemoUniforms>()->lightPosition = camera->getPosition();
	shaderManager->getShader((int) ShaderName::Particle)->getCustomUniforms<DemoUniforms>()->lightPosition = camera->getPosition();
	shaderManager->getShader((int) ShaderName::Rocks)->getCustomUniforms<DemoUniforms>()->lightPosition = camera->getPosition();

	camera->update();

	if (particles.size() < 5)
	{
		particles.push_back(batchManager->createMesh("Models/particle_positions.obj"));
		particles.back()->setTexture("Textures/particle.png");
		particles.back()->setShader((int) ShaderName::Particle);
		particles.back()->setDrawMode(spehs::POINT);
		particles.back()->setPosition(camera->getPosition().x + spehs::rng::frandom(-20.0f, 20.0f), spehs::rng::frandom(60.0f, 100.0f), camera->getPosition().z + spehs::rng::frandom(-20.0f, 20.0f));
		particles.back()->setScale(30.0f);
		particles.back()->setRotation(0.0, spehs::rng::frandom(0.0f, PI), 0.0);
	}

	for (unsigned i = 0; i < particles.size(); i++)
	{
		if (particles[i]->getPosition().y < -10.0f)
		{
			particles[i]->destroy();
			particles[i] = particles.back();
			particles.pop_back();
		}
		else
		{
			particles[i]->setPosition(particles[i]->getPosition() - glm::vec3(0.0f, 0.0008f * (i + 1) * spehs::getDeltaTime().asMilliseconds, 0.0f));
		}
	}

	if (!input())
		return false;

	spehs::console::update();	

	return true;
}

void DemoState3D::render()
{
	spehs::setActiveBatchManager(batchManager);
	batchManager->render();
}

bool DemoState3D::input()
{
	static float speed;
	static float lookSpeed = 0.25f;

	inputManager->update();
	if (inputManager->isKeyDown(KEYBOARD_ESCAPE) || inputManager->isQuitRequested())
		return false;

	if (inputManager->isKeyPressed(KEYBOARD_F5))
	{
		for (unsigned i = NEXT_SHADER_INDEX; i < NEXT_SHADER_INDEX + NUM_SHADERS; i++)
		{
			reloadShader((ShaderName)i);
		}

		shaderManager->getShader((int) ShaderName::Water)->getCustomUniforms<WaterUniforms>()->reflectionTextureID = textureManager->getCubeMapData(skyBox->getCubeMapHash())->textureDataID;
		shaderManager->getShader((int) ShaderName::Water)->getCustomUniforms<WaterUniforms>()->heightMapTextureID = textureManager->getTextureData("Textures/water_height_map.png")->textureDataID;
		shaderManager->getShader((int) ShaderName::FarWater)->getCustomUniforms<WaterUniforms>()->reflectionTextureID = textureManager->getCubeMapData(skyBox->getCubeMapHash())->textureDataID;
		shaderManager->getShader((int) ShaderName::FarWater)->getCustomUniforms<WaterUniforms>()->heightMapTextureID = textureManager->getTextureData("Textures/far_water_height_map.png")->textureDataID;
		shaderManager->getShader((int) ShaderName::Pillar)->getCustomUniforms<PillarUniforms>()->reflectionTextureID = textureManager->getCubeMapData(skyBox->getCubeMapHash())->textureDataID;
		shaderManager->getShader((int) ShaderName::Pillar)->getCustomUniforms<PillarUniforms>()->bumpMapTextureID = textureManager->getTextureData("Textures/pillar_bump_map.png")->textureDataID;
		shaderManager->getShader((int) ShaderName::Rocks)->getCustomUniforms<RocksUniforms>()->bumpMapTextureID = textureManager->getTextureData("Textures/rock_bump_map.png")->textureDataID;
	}

	if (inputManager->isKeyDown(KEYBOARD_LSHIFT))
	{
		speed = 15.0f;
	}
	else if (inputManager->isKeyDown(KEYBOARD_LCTRL))
	{
		speed = 0.5f;
	}
	else
	{
		speed = 2.0f;
	}

	//Camera movement
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

	if (inputManager->isKeyDown(KEYBOARD_E))
	{
		camera->move(spehs::DOWN, speed * spehs::getDeltaTime().asSeconds);
	}
	if (inputManager->isKeyDown(KEYBOARD_Q))
	{
		camera->move(spehs::UP, speed * spehs::getDeltaTime().asSeconds);
	}

	if (inputManager->isKeyDown(MOUSEBUTTON_RIGHT))
	{
		inputManager->lockMouse(true);


		camera->pitch(inputManager->getMouseMovementX() * spehs::getDeltaTime().asSeconds * lookSpeed);
		camera->yaw(inputManager->getMouseMovementY() * spehs::getDeltaTime().asSeconds * lookSpeed);
	}
	else if (inputManager->isKeyDown(MOUSEBUTTON_MIDDLE))
	{
		inputManager->lockMouse(true);

		camera->move(spehs::UP, inputManager->getMouseMovementY() * speed * spehs::getDeltaTime().asSeconds);
		camera->move(spehs::RIGHT, inputManager->getMouseMovementX() * speed * spehs::getDeltaTime().asSeconds);
	}
	else
	{
		inputManager->lockMouse(false);
	}

	
	//Restrict Camera
	if (camera->getPosition().y < 10.0f)
	{
		camera->setPosition(glm::vec3(camera->getPosition().x, 10.0f, camera->getPosition().z));
	}

	return true;
}
