#version 150

in vec3 fragmentPosition;
in vec4 fragmentColor;
in vec3 fragmentNormal;
in vec2 fragmentUV;

out vec4 outColor;

uniform sampler2D tex;
uniform sampler2D bumpMapTex;
uniform vec3 lightPosition;

void main()
{
	//Bump map for advanced lighting
	vec3 bumpMap = texture(bumpMapTex, fragmentUV).rgb * 0.6 - 0.5;
	
	//Blinn Phong lighting
	vec3 lightPos = vec3(0.0, 1000.0, 0.0);
	vec3 lightDirection = normalize(lightPos - fragmentPosition);
	vec3 viewDirection = normalize(lightPosition - fragmentPosition);
	float distance = length(lightPos - fragmentPosition);
	float radius = 3000.0;
	float attenuation = 1 - pow((distance / radius), 2);
	float shininess = 64.0;
	vec3 normal = normalize(fragmentNormal + bumpMap * normalize(viewDirection));
	
	//Ambient
	vec3 ambient = texture(tex, fragmentUV).rgb * 0.6;
	
	//Diffuse
	vec3 diffuse = texture(tex, fragmentUV).rgb * 0.4;
	
	//Specular
	vec3 specular = fragmentColor.rgb;
	float spec = 0.0;
	float lambertian = max(dot(normal, lightDirection), 0.0);
	if(lambertian > 0.0)
	{
		vec3 halfwayDirection = normalize(lightDirection + viewDirection);
		float specAngle = max(dot(normal, halfwayDirection), 0.0);
		spec = pow(specAngle, shininess);
	}
	diffuse = diffuse * lambertian;
	specular = specular * spec;
	outColor = vec4(ambient + attenuation * (diffuse + specular), 1.0);
	
	//Darken far away environment
	float viewDistance = 400.0;
	if(length(fragmentPosition) > viewDistance)
	{
		float dist = min((length(fragmentPosition)), 1800.0);
		outColor = outColor - vec4(0.0001, 0.0001, 0.0001, 1.0) * (dist - viewDistance);
	}
	
	//Color correction / Fog effect
	outColor = mix(outColor, vec4(0.05, 0.15, 0.20, 1.0), max(min(0.065*(length(fragmentPosition - lightPosition)/80.0), 0.7), 0.25));
}



