#version 150
in vec3 vertexPosition;
in vec4 vertexColor;
in vec3 vertexNormal;
in vec2 vertexUV;

out vec3 fragmentPosition;
out vec4 fragmentColor;
out vec3 fragmentNormal;
out vec2 fragmentUV;

uniform mat4 cameraMatrix;
void main()
{
	gl_Position = cameraMatrix * vec4(vertexPosition.xyz, 1.0);
	fragmentPosition = vertexPosition;
	fragmentColor = vertexColor;
	fragmentNormal = vertexNormal;
	fragmentUV = vertexUV;
}