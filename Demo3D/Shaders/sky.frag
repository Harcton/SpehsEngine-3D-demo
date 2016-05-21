#version 150

#extension GL_NV_shadow_samplers_cube : enable

in vec3 fragmentPosition;

out vec4 outColor;

uniform samplerCube tex;

void main()
{
	outColor = textureCube(tex, fragmentPosition);
	outColor.r = outColor.r * min(fragmentPosition.y * 2.0, 1.0);
	outColor.g = outColor.g * max(min((fragmentPosition.y + 0.0) * 2.0, 1.0), 0.7);
	outColor.b = outColor.b * max(min((fragmentPosition.y + 0.0) * 2.0, 1.0), 0.7);
	
	outColor.rgb = ((outColor.rgb - 0.5) * max(1-fragmentPosition.y + 0.3, 1)) + 0.3;
	outColor = mix(outColor, vec4(0.05, 0.15, 0.20, 1.0), min((1.0 - fragmentPosition.y), 0.9));
}