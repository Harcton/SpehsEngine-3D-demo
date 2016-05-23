#version 150

in vec2 texCoord;

out vec4 outColor;

uniform sampler2D tex;

void main()
{
	//Simple bloom / brightness / blur effect
	vec4 sum = vec4(0.0);

	for(int i= -5 ;i < 5; i++)
	{
		for (int j = -6; j < 6; j++)
		{
			sum += texture2D(tex, texCoord + vec2(j, i)*0.001) * 0.06;
		}
	}
	if (texture2D(tex, texCoord).b < 0.2)
	{
		outColor = sum*sum*0.012 + texture2D(tex, texCoord);
	}
    else
	{
		if (texture2D(tex, texCoord).b < 0.9 || texture2D(tex, texCoord).r > 0.99)
		{
			outColor = sum*sum*0.015 + texture2D(tex, texCoord);
		}
		else
		{
			outColor = sum*sum*0.0075 + texture2D(tex, texCoord);
		}
	}
}