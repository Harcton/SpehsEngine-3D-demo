#version 150

in vec3 fragmentPosition;

out vec4 color;

uniform samplerCube tex;

void main()
{
	color = texture(tex, fragmentPosition);
	color.r = color.r * min(fragmentPosition.y * 3.0, 1.0);
	color.g = color.g * min((fragmentPosition.y + 0.05) * 3.0, 1.0);
	color.b = color.b * min((fragmentPosition.y + 0.3) * 1.7, 1.0);
	
	color.rgb = ((color.rgb - 0.5) * max(1-fragmentPosition.y + 0.3, 1)) + 0.5;
}