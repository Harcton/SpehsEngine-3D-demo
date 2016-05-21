#version 150

in vec3 fragmentPosition;
in vec4 fragmentColor;
in vec3 fragmentNormal;
in vec2 fragmentUV;

out vec4 outColor;

uniform sampler2D tex;
uniform vec3 lightPosition;

void main()
{
	vec3 lightPos = vec3(0.0, 1000.0, 0.0);
	vec3 lightDirection = normalize(lightPos - fragmentPosition);
	vec3 viewDirection = normalize(-fragmentPosition);
	float distance = length(lightPos - fragmentPosition);
	float radius = 3000.0;
	float attenuation = 1 - pow((distance / radius), 2);
	float shininess = 128.0;
	vec3 normal = normalize(fragmentNormal);
	
	//Ambient
	vec3 ambient = texture(tex, fragmentUV).rgb * 0.15;
	
	//Diffuse
	vec3 diffuse = texture(tex, fragmentUV).rgb;
	
	//Specular
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
	
	float viewDistance = 400.0;
	if(length(fragmentPosition - lightPosition) > viewDistance)
	{
		float dist = min((length(fragmentPosition - lightPosition)), 1800.0);
		outColor = outColor - vec4(0.0002, 0.0001, 0.0001, 1.0) * (dist - viewDistance);
	}
	outColor = mix(outColor, vec4(0.05, 0.15, 0.20, 1.0), max(min(0.065*(length(fragmentPosition - lightPosition)/80.0), 0.7), 0.25));
}



