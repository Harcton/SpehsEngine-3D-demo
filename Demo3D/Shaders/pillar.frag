#version 150

#extension GL_NV_shadow_samplers_cube : enable

in vec3 fragmentPosition;
in vec3 fragmentColor;
in vec3 fragmentNormal;
in vec2 fragmentUV;

out vec4 outColor;

uniform sampler2D tex;
uniform samplerCube reflectionTex;
uniform sampler2D bumpMapTex;
uniform vec3 lightPosition;

void main()
{
	vec3 bumpMap = texture(bumpMapTex, fragmentUV).rgb * 1.0 - 0.5;

	vec3 lightPos = vec3(-150.0, 750.0, -150.0);
	vec3 lightDirection = normalize(lightPos - fragmentPosition);
	vec3 viewDirection = normalize(-fragmentPosition);
	float distance = length(lightPos - fragmentPosition);
	float radius = 2200.0;
	float attenuation = 1 - pow((distance / radius), 2);
	float shininess = 64.0;
	vec3 normal = normalize(fragmentNormal + bumpMap * normalize(viewDirection));
		
	vec3 ambient = mix(fragmentColor.rgb, texture(tex, fragmentUV).rgb, 0.95) * 0.5;
	
	vec3 diffuse = textureCube(reflectionTex, reflect(lightDirection, normal)).rgb * 0.1;
	
	vec3 specular = textureCube(reflectionTex, reflect(lightDirection, normal)).rgb * min(max((length(fragmentPosition - lightPosition)/450.0), 1.8), 3.5);
	
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
	
	float viewDistance = 400.0;
	if(length(fragmentPosition - lightPosition) > viewDistance)
	{
		float dist = min((length(fragmentPosition - lightPosition)), 1800.0);
		outColor = outColor - vec4(0.0002, 0.0001, 0.0001, 1.0) * (dist - viewDistance);
	}
	outColor = mix(outColor, vec4(0.05, 0.15, 0.20, 1.0), max(min(0.065*(length(fragmentPosition - lightPosition)/80.0), 0.7), 0.25));
}