#pragma once

#include <SpehsEngine/ShaderManager.h>
#include <SpehsEngine/GLSLProgram.h>

#include <vector>

/*
CREATING NEW SHADERS:
1. Create new Uniforms class
2. Add ShaderName to enum
3. Add shader to list to be sent to engine
Order of initialization is important!

Access Uniform values with: shaderManager->getShader( %SHADER_NAME )->getCustomUniforms< %UNIFORM_CLASS >()-> %UNIFORM_VALUE
*/

enum class ShaderName : int
{//0-4 already in use for Defaults
	
};

extern void initShaders();
extern void reloadShader(const ShaderName _shaderIndex);

//Custom Uniforms:




