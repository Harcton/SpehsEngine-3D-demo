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
	//Blinn Phong lighting
	//Grass normal needs to be set differently to get better lighting effects
	vec3 normal = vec3(0.0, 1.0, 0.0);
	vec3 lightPos = vec3(0.0, 1000.0, 0.0);
	vec3 lightDirection = normalize(lightPos - fragmentPosition);
	vec3 viewDirection = normalize(lightPosition - fragmentPosition);
	float distance = length(lightPos - fragmentPosition);
	float radius = 3000.0;
	float attenuation = 1 - pow((distance / radius), 2);
	float shininess = 64.0;
	
	//Ambient
	vec3 ambient = texture(tex, fragmentUV).rgb * 0.57;
	
	//Blending away unwanted texture background
	const float eps = 0.09;
	if(ambient.r < eps || ambient.g < eps || ambient.b < eps)
		discard;
		
	ambient = ambient * 0.6;
	
	//Diffuse
	vec3 diffuse = texture(tex, fragmentUV).rgb * 0.2;
	
	//Specular
	vec3 specular = vec3(0.1);
	
	float spec = 0.0;
	float lambertian = max(dot(lightDirection, normal), 0.0);
	if(lambertian > 0.0)
	{
		vec3 halfwayDirection = normalize(lightDirection + viewDirection);
		float specAngle = max(dot(normal, halfwayDirection), 0.0);
		spec = pow(specAngle, shininess);
	}
	diffuse = diffuse * lambertian;
	specular = specular * spec;
	outColor = vec4(ambient + attenuation * (diffuse + specular), 1.0);
	
	//Darken far away objects
	float viewDistance = 400.0;
	if(length(fragmentPosition) > viewDistance)
	{
		float dist = min((length(fragmentPosition)), 1800.0);
		outColor = outColor - vec4(0.00012, 0.00012, 0.00012, 1.0) * (dist - viewDistance);
	}
	
	//Color correction / Fog effect
	outColor = mix(outColor, vec4(0.05, 0.13, 0.18, 1.0), max(min(0.065*(length(fragmentPosition - lightPosition)/80.0), 0.7), 0.25));
}