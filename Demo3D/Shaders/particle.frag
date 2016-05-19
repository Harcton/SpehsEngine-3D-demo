#version 150

out vec4 color;

uniform sampler2D tex;

void main()
{
	color = texture(tex, gl_PointCoord);
	if(color.g > 0.72)
		discard;
}