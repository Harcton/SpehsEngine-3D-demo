#version 150

#extension GL_NV_shadow_samplers_cube : enable

in vec3 fragmentPosition;

out vec4 color;

uniform samplerCube tex;

void main()
{
	color = textureCube(tex, fragmentPosition);
	color.r = color.r * min(fragmentPosition.y * 2.0, 1.0);
	color.g = color.g * max(min((fragmentPosition.y + 0.0) * 2.0, 1.0), 0.7);
	color.b = color.b * max(min((fragmentPosition.y + 0.0) * 2.0, 1.0), 0.7);
	
	color.rgb = ((color.rgb - 0.5) * max(1-fragmentPosition.y + 0.3, 1)) + 0.3;
}