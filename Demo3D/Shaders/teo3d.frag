#version 150
#define lightCount 10
in vec3 fragmentPosition;
in vec4 fragmentColor;
in vec3 fragmentNormal;
in vec2 fragmentUV;
out vec4 color;
uniform sampler2D tex;
uniform float numLights;
uniform vec4[lightCount] lights1;
uniform vec4[lightCount] lights2;


void main()
{
	vec3 normal = (fragmentNormal);
	vec3 viewDirection = normalize(-fragmentPosition);
	vec3 lighting = vec3(0.0, 0.0, 0.0);
	
	//Ambient
	vec3 ambient = texture(tex, fragmentUV).rgb;	
	
	for	(int i = 0; i < numLights; i++)
	{
		float spec = 0.0;
		float shininess = 64.0;
		
		//Light specs
		vec3 lightPosition = vec3(lights1[i].x, lights1[i].y, lights1[i].z);
		vec3 lightDirection = normalize(lightPosition - fragmentPosition);
		float lightDistance = length(lightPosition - fragmentPosition);
		float lightRadius = lights1[i].w;
		float lightAttenuation = 1 - pow((min(lightDistance, lightRadius) / lightRadius), 2);
		
		float lambertian = max(dot(lightDirection, normal), 0.0);
		if(lambertian > 0.0)
		{
			vec3 halfwayDirection = normalize(lightDirection + viewDirection);
			float specAngle = max(dot(normal, halfwayDirection), 0.0);
			spec = pow(specAngle, shininess);
		}
		
		//Diffuse
		vec3 diffuse = fragmentColor.rgb * lambertian;
		
		//Specular
		vec3 specular = vec3(1.0, 1.0, 1.0) * spec;
		
		lighting = lighting + ((diffuse + lights2[i].rgb + specular) * lightAttenuation * lights2[i].a);
	}
	color = vec4(ambient + lighting, 1.0);
}