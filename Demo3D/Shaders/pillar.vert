#version 150

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUV;

out vec3 fragmentPosition;
out vec3 fragmentNormal;
out vec2 fragmentUV;

uniform mat4 cameraMatrix;

void main()
{
	gl_Position = cameraMatrix * vec4(vertexPosition.xyz, 1.0);
	fragmentNormal = vertexNormal;
	fragmentPosition = vertexPosition;
	fragmentUV = vertexUV;
}