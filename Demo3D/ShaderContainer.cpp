
#include "ShaderContainer.h"
#include <SpehsEngine/Console.h>


spehs::Shader* buildShader(const ShaderName _name)
{
	spehs::GLSLProgram* shader;
	switch (_name)
	{

	default:
		return nullptr;
		break;
	}
}

void initShaders()
{

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