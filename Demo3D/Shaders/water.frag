#version 150

in vec3 fragmentPosition;
in vec2 texCoord;
in vec4 fragmentColor;

out vec4 outColor;

uniform sampler2D tex;
uniform float seconds;
uniform vec3 lightPosition;

const float PI = 3.1415926535897932;

const float speed = 2.5;
const float speed_x = 0.2;
const float speed_y = 0.3;

const float emboss = 0.50;
const float intensity = 1.5;
const int steps = 5;
const float frequency = 20.0;
const int angle = 7;

const float delta = 40.0;
const float intence = 400.0;

const float reflectionCutOff = 0.002;
const float reflectionIntence = 200000.;


float time = seconds / 10.0;

float col(vec2 coord)
{
	float delta_theta = 2.0 * PI / float(angle);
	float col = 0.0;
	float theta = 0.0;
	for (int i = 0; i < steps; i++)
	{
		vec2 adjc = coord;
		theta = delta_theta*float(i);
		adjc.x += cos(theta)*time*speed + time * speed_x;
		adjc.y -= sin(theta)*time*speed - time * speed_y;
		col = col + cos( (adjc.x*cos(theta) - adjc.y*sin(theta))*frequency)*intensity;
	}
	return cos(col);
}


void main()
{
	vec2 p = (texCoord / 100.0), c1 = p, c2 = p;
	float cc1 = col(c1);

	c2.x += fragmentPosition.x/delta;
	float dx = emboss*(cc1-col(c2))/delta;

	c2.x = p.x;
	c2.y += fragmentPosition.y/delta;
	float dy = emboss*(cc1-col(c2))/delta;

	c1.x += dx*2.;
	c1.y = -(c1.y+dy*2.);

	float alpha = 1.+dot(dx,dy)*intence;
		
	float ddx = dx - reflectionCutOff;
	float ddy = dy - reflectionCutOff;
	if (ddx > 0. && ddy > 0.)
		alpha = pow(alpha, ddx*ddy*reflectionIntence);
		
	vec4 col = texture(tex,c1)*(alpha);
	outColor = col;
	
	float viewDistance = 150.0;
	if(length(fragmentPosition - lightPosition) > viewDistance)
	{
		float dist = min((length(fragmentPosition - lightPosition)), 700.0);
		outColor = outColor - vec4(0.0008, 0.0006, 0.0006, 1.0) * (dist - viewDistance);
	}
}