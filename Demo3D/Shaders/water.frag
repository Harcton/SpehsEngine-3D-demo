#version 150

#extension GL_NV_shadow_samplers_cube : enable

in vec3 fragmentPosition;
in vec4 fragmentColor;
in vec2 texCoord;
in vec2 heightCoord;

out vec4 outColor;

uniform sampler2D tex;
uniform samplerCube reflectionTex;
uniform sampler2D heightMapTex;
uniform float seconds;
uniform vec3 lightPosition;

const float PI = 3.1415926535897932;

float speed = 2.5;
const float speed_x = 0.25;
const float speed_y = 0.3;

float emboss = 0.50;
const float intensity = 1.0;
const int steps = 4;
const float frequency = 7.0;
const int angle = 20;

const float delta = 30.0;
const float intence = 80.0;

const float reflectionCutOff = 0.0019;
const float reflectionIntence = 200000.0;


float time = seconds / 12.0;

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
	vec2 p = (texCoord / 30.0), c1 = p, c2 = p;
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
		
	float viewDistance = 600.0;
	vec3 view = normalize(fragmentPosition - lightPosition);
	vec3 normal = vec3(0.0, 1.0, 0.0);
	outColor = mix(textureCube(reflectionTex, reflect(view, normal)), vec4(0.0), 0.5 - (texture(heightMapTex, heightCoord).y)) * 0.18;
		
	vec4 col = mix(texture(tex, c1), vec4(0.01, 0.01, 0.01, 1.0), 1.10 - (texture(heightMapTex, heightCoord).y))*(alpha);
	outColor += col;
	outColor = mix(outColor, vec4(0.04, 0.10, 0.14, 1.0), min(0.065*(length(fragmentPosition)/280.0), 1.0))*(alpha);	
	outColor = mix(outColor, vec4(0.05, 0.15, 0.20, 1.0), min(0.065*(length(fragmentPosition - lightPosition)/80.0), 0.7));
}