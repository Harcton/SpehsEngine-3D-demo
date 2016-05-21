#version 150

in vec2 vertexPosition;

out vec2 texCoord;

void main()
{
	gl_Position = vec4(vertexPosition, 0.0, 1.0);
	texCoord = (vertexPosition + 1.0) / 2.0;
}