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


#define NUM_SHADERS 5


enum class ShaderName : int
{
	Water = NEXT_SHADER_INDEX,
	Environment,
	Sky,
	Pillar,
	Grass,
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

private:
	unsigned int secondsLocation = 0;
	unsigned int reflectionTextureLocation = 0;
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