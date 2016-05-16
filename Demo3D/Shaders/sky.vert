#version 150

in vec3 vertexPosition;

out vec3 fragmentPosition;

uniform mat4 cameraMatrix;

void main()
{
	vec4 pos = mat4(mat3(cameraMatrix)) * vec4(vertexPosition.xyz, 1.0);
	gl_Position = pos.xyww;
	fragmentPosition = vertexPosition;
}