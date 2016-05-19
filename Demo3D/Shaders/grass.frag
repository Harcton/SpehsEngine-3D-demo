#version 150

#extension GL_NV_shadow_samplers_cube : enable

in vec3 fragmentPosition;
in vec3 fragmentColor;
in vec3 fragmentNormal;
in vec2 fragmentUV;

out vec4 outColor;

uniform sampler2D tex;
uniform vec3 lightPosition;

void main()
{
	outColor = texture(tex, fragmentUV) * 0.6;

	float viewDistance = 700.0;
	if(length(fragmentPosition - lightPosition) > viewDistance)
	{
		float dist = min(abs((length(fragmentPosition - lightPosition))), 1300.0);
		outColor = outColor - vec4(0.000, 0.0002, -0.0001, 1.0) * (dist - viewDistance);
	}
	
	if(outColor.b > 0.22)
		discard;
}