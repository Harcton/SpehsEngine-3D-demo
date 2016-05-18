#version 150

#extension GL_NV_shadow_samplers_cube : enable

in vec3 fragmentPosition;
in vec3 fragmentColor;
in vec3 fragmentNormal;
in vec2 fragmentUV;

out vec4 outColor;

uniform sampler2D tex;
uniform samplerCube reflectionTex;
uniform vec3 lightPosition;

void main()
{
	vec3 normal = normalize(fragmentNormal);
	vec3 lightDirection = normalize(lightPosition - fragmentPosition);
	vec3 viewDirection = normalize(-fragmentPosition);
	float distance = length(lightPosition - fragmentPosition);
	float radius = 400.0;
	float attenuation = 1 - pow((distance / radius), 2);
	float shininess = 128.0;
		
	vec3 ambient = mix(fragmentColor.rgb, texture(tex, fragmentUV).rgb, 0.9);
	
	vec3 diffuse = textureCube(reflectionTex, reflect(-lightDirection, normal)).rgb;
	
	vec3 specular = fragmentColor.rgb;
	
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
}