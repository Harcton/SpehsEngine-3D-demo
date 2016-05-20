#version 150

in vec3 vertexPosition;

uniform mat4 cameraMatrix;
uniform vec3 lightPosition;

void main()
{
	gl_PointSize = min(2.1 / (length(vertexPosition - lightPosition) / 1000.0), 4.0);
	gl_Position = cameraMatrix * vec4(vertexPosition.xyz, 1.0);
}