#version 150

out vec4 outColor;

uniform sampler2D tex;

void main()
{
	outColor = texture(tex, gl_PointCoord);
	
	//Green screen out the unwanted texture background
	if(outColor.g > 0.85)
		discard;
		
	//Color correction
	outColor = outColor * 0.6;
}