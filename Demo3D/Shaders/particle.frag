#version 150

out vec4 outColor;

uniform sampler2D tex;

void main()
{
	outColor = texture(tex, gl_PointCoord);
	if(outColor.g > 0.85)
		discard;
	outColor = outColor * 0.6;
}