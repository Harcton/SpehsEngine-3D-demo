
#include "ShaderContainer.h"
#include <SpehsEngine/Console.h>
#include <SpehsEngine/Time.h>


spehs::Shader* buildShader(const ShaderName _name)
{
	spehs::GLSLProgram* shader = new spehs::GLSLProgram();
	switch (_name)
	{
	case ShaderName::Water:
		shader->compileShaders("Shaders/water.vert", "Shaders/water.frag");
		shader->addAttribute("vertexPosition");
		shader->addAttribute("vertexColor");
		shader->linkShaders();
		return new spehs::Shader((int)ShaderName::Water, shader, new WaterUniforms(shader));
		break;

	case ShaderName::Environment:
		shader->compileShaders("Shaders/environment.vert", "Shaders/environment.frag");
		shader->addAttribute("vertexPosition");
		shader->addAttribute("vertexColor");
		shader->addAttribute("vertexNormal");
		shader->addAttribute("vertexUV");
		shader->linkShaders();
		return new spehs::Shader((int) ShaderName::Environment, shader, new DemoUniforms(shader));
		break;

	case ShaderName::Sky:
		shader->compileShaders("Shaders/sky.vert", "Shaders/sky.frag");
		shader->addAttribute("vertexPosition");
		shader->linkShaders();
		return new spehs::Shader((int) ShaderName::Sky, shader, new spehs::DefaultSkyBoxUniforms(shader));
		break;

	case ShaderName::Pillar:
		shader->compileShaders("Shaders/pillar.vert", "Shaders/pillar.frag");
		shader->addAttribute("vertexPosition");
		shader->addAttribute("vertexNormal");
		shader->addAttribute("vertexUV");
		shader->linkShaders();
		return new spehs::Shader((int) ShaderName::Pillar, shader, new PillarUniforms(shader));
		break;

	default:
		delete shader;
		return nullptr;
		break;
	}
}

void initShaders()
{
	//Water
	shaderManager->pushShader(buildShader(ShaderName::Water));

	//Environment
	shaderManager->pushShader(buildShader(ShaderName::Environment));

	//Sky
	shaderManager->pushShader(buildShader(ShaderName::Sky));

	//Pillar
	shaderManager->pushShader(buildShader(ShaderName::Pillar));
}
void reloadShader(const ShaderName _shaderIndex)
{
	//if (_shaderIndex == 0 || _shaderIndex == 1 || _shaderIndex == 2 || _shaderIndex == 3 || _shaderIndex == 4)
	//{
	//	spehs::console::error("Cannot reload default shaders!");
	//	return;
	//}
	spehs::Shader* shader = buildShader(_shaderIndex);
	if (!shader)
	{
		spehs::console::error("Invalid shader index!");
		return;
	}
	shaderManager->reload((int) _shaderIndex, shader);
	spehs::console::log("Shader reloaded!");
}



DemoUniforms::DemoUniforms(spehs::GLSLProgram* _shader) : Uniforms(_shader)
{
	textureLocation = shader->getUniformLocation("tex");
	lightPositionLocation = shader->getUniformLocation("lightPosition");
}
DemoUniforms::~DemoUniforms()
{

}
void DemoUniforms::setUniforms()
{
	spehs::bind2DTexture(textureDataID, 0);
	spehs::setUniform_int(textureLocation, 0);
	spehs::setUniform_vec3(lightPositionLocation, lightPosition);
	Uniforms::setUniforms();
}

WaterUniforms::WaterUniforms(spehs::GLSLProgram* _shader) : DemoUniforms(_shader)
{
	secondsLocation = shader->getUniformLocation("seconds");
	reflectionTextureLocation = shader->getUniformLocation("reflectionTex");
}
WaterUniforms::~WaterUniforms()
{

}
void WaterUniforms::setUniforms()
{
	seconds += spehs::getDeltaTime().asSeconds;
	spehs::setUniform_float(secondsLocation, seconds);
	spehs::bindCubeMapTexture(reflectionTextureID, 1);
	spehs::setUniform_int(reflectionTextureLocation, 1);
	DemoUniforms::setUniforms();
}

PillarUniforms::PillarUniforms(spehs::GLSLProgram* _shader) : DemoUniforms(_shader)
{
	reflectionTextureLocation = shader->getUniformLocation("reflectionTex");
}
PillarUniforms::~PillarUniforms()
{

}
void PillarUniforms::setUniforms()
{
	spehs::bindCubeMapTexture(reflectionTextureID, 1);
	spehs::setUniform_int(reflectionTextureLocation, 1);
	DemoUniforms::setUniforms();
}