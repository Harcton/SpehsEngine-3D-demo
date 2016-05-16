#version 150

in vec3 vertexPosition;
in vec4 vertexColor;

out vec3 fragmentPosition;
out vec4 fragmentColor;
out vec2 texCoord;

uniform mat4 cameraMatrix;

void main()
{
	gl_Position = cameraMatrix * vec4(vertexPosition.xyz, 1.0);
	fragmentColor = vertexColor;
	fragmentPosition = vertexPosition;
	texCoord = vec2(vertexPosition.x, vertexPosition.z);
}