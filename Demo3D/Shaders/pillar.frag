#version 150

#extension GL_NV_shadow_samplers_cube : enable

in vec3 fragmentPosition;
in vec3 fragmentNormal;
in vec2 fragmentUV;

out vec4 outColor;

uniform sampler2D tex;
uniform samplerCube reflectionTex;
uniform vec3 lightPosition;

void main()
{

	vec3 normal = normalize(-fragmentNormal);
	vec3 lightDirection = normalize(lightPosition - fragmentPosition);
	vec3 viewDirection = normalize(-fragmentPosition);
	float distance = length(lightPosition - fragmentPosition);
	float radius = 20.0;
	float attenuation = 1 - pow((distance / radius), 2);
	attenuation = 1.0;
	float shininess = 2.0;
	
	outColor = textureCube(reflectionTex, reflect(-lightDirection, normal)) * 0.00000000000004;
	
	vec3 ambient = texture(tex, fragmentUV).rgb;
	
	vec3 diffuse = vec3(1.0);
	
	vec3 specular = vec3(1.0);
	float spec = 0.0;
	float lambertian = max(dot(lightDirection, normal), 0.0);
	if(lambertian > 0.0)
	{
		vec3 halfwayDirection = normalize(lightDirection + viewDirection);
		float specAngle = max(dot(normal, halfwayDirection), 0.0);\
		spec = pow(specAngle, shininess);
	}
	diffuse = diffuse * lambertian;\
	specular = specular * spec;
	outColor += vec4(ambient, 1.0);
	outColor *= vec4(attenuation * (diffuse + specular), 1.0);
}