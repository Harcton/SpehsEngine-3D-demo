uniform sampler2D texture;
varying vec2 texPos;

void main(void)
{
	vec4 col = vec4(1.0, 1.0, 1.0, 1.0) - texture2D(texture, texPos);
	col.a = 1.0;
	gl_FragColor = col;
	//gl_FragColor = texture2D(texture, texPos);
}