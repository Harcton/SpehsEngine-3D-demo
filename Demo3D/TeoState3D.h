#pragma once
#include "../SpehsEngine/Dependencies/include/GL/glew.h"
#include <SpehsEngine/GLSLProgram.h>
#include <SpehsEngine/ShaderManager.h>
#include "State.h"
namespace spehs { class Camera3D; class BatchManager; class Mesh; }


class TeoState3D : public State
{
public:
	TeoState3D();
	~TeoState3D();

	bool update();
	void render();

	//Lights
	std::vector<glm::vec4> lights1;//xyz + radius
	std::vector<glm::vec4> lights2;//rgba
	int currentLightIndex;

private:
	spehs::Camera3D* camera;
	spehs::BatchManager* batchManager;
	std::vector<spehs::Mesh*> meshes;
};






class TeoUniforms : public spehs::Uniforms
{
public:
	TeoUniforms(spehs::GLSLProgram& glslProgram, TeoState3D& teoState);
	void setUniforms();
private:
	TeoState3D& teoState;
	GLint textureLocation;
	GLint numLightsUniformLocation;
	GLint lightsArray1UniformLocation;
	GLint lightsArray2UniformLocation;
};