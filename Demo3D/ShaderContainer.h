#pragma once

#include <SpehsEngine/ShaderManager.h>
#include <SpehsEngine/GLSLProgram.h>

#include <glm/vec3.hpp>

#include <vector>

/*
CREATING NEW SHADERS:
1. Create new Uniforms class
2. Add ShaderName to enum
3. Add shader to list to be sent to engine
Order of initialization is important!

Access Uniform values with: shaderManager->getShader( %SHADER_NAME )->getCustomUniforms< %UNIFORM_CLASS >()-> %UNIFORM_VALUE
*/


#define NUM_SHADERS 8


enum class ShaderName : int
{
	Water = NEXT_SHADER_INDEX,
	Environment,
	Sky,
	Pillar,
	Grass,
	Particle,
	FarWater,
	Rocks,
};

extern void initShaders();
extern void reloadShader(const ShaderName _shaderIndex);


//Custom Uniforms:

class DemoUniforms : public spehs::Uniforms
{
public:
	DemoUniforms(spehs::GLSLProgram* _shader);
	virtual ~DemoUniforms();

	virtual void setUniforms();

	glm::vec3 lightPosition;

private:
	unsigned int textureLocation = 0;
	unsigned int lightPositionLocation = 0;
};

class WaterUniforms : public DemoUniforms
{
public:
	WaterUniforms(spehs::GLSLProgram* _shader);
	~WaterUniforms();

	void setUniforms();

	float seconds;
	unsigned int reflectionTextureID;
	unsigned int heightMapTextureID;

private:
	unsigned int secondsLocation = 0;
	unsigned int reflectionTextureLocation = 0;
	unsigned int heightMapTextureLocation = 0;
};

class PillarUniforms : public DemoUniforms
{
public:
	PillarUniforms(spehs::GLSLProgram* _shader);
	~PillarUniforms();

	void setUniforms();

	unsigned int reflectionTextureID;

private:
	unsigned int reflectionTextureLocation = 0;
};

class RocksUniforms : public DemoUniforms
{
public:
	RocksUniforms(spehs::GLSLProgram* _shader);
	~RocksUniforms();

	void setUniforms();

	unsigned int bumbMapTextureID;

private:
	unsigned int bumbMapTextureLocation = 0;
};

class GrassUniforms : public DemoUniforms
{
public:
	GrassUniforms(spehs::GLSLProgram* _shader);
	~GrassUniforms();

	void setUniforms();

	float time;

private:
	unsigned int timeLocation = 0;
};