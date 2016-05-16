#version 150
#extension GL_NV_shadow_samplers_cube : enable
#define lightCount 10//Number of lights for the shader to be able to take in at maximum
in vec3 fragmentPosition;//Fragment position in world space
in vec4 fragmentColor;//Not actually used, texturing overrides
in vec3 fragmentNormal;//Unit vector pointing outwards from the model surface
in vec2 fragmentUV;//UV coordinates are used for retrieving texture(colouring) data

out vec4 color;//The outcome of the lighting calculation, texturing accounted

uniform float numLights;//Number of lights to calculate for, up to previously defined lightCount
uniform vec4[lightCount] lights1;//Light position: xyz, light radius: w
uniform vec4[lightCount] lights2;//Light color: rgba
uniform sampler2D tex;
uniform samplerCube cubeMap;
uniform vec3 cameraPosition;

void main()
{
	vec3 viewDirection = normalize(-fragmentNormal);//Used to determine in what kind of angle the light hits the surface.
	vec3 lighting = vec3(0.0, 0.0, 0.0);
		//The default value of lighting acts as the ambient color.
		//This could have been given as an uniform but was left out for simplicity.
	
	//For each light given, compute lighting and add them all together
	for	(int i = 0; i < numLights; i++)
	{
		float spec = 0.0;
		float shininess = 16.0;//Shininess controls the pure white, shining area on the model surface.
		
		//Light specs
		vec3 lightPosition = vec3(lights1[i].x, lights1[i].y, lights1[i].z);
		vec3 lightDirection = normalize(lightPosition - fragmentPosition);//A vector from fragment to light
		float lightDistance = length(lightPosition - fragmentPosition);
		float lightRadius = lights1[i].w;
		float lightAttenuation = 1 - pow((min(lightDistance, lightRadius) / lightRadius), 2);//Exponentially fading out the light, the farther the distance.
		
		float lambertian = max(dot(lightDirection, fragmentNormal), 0.0);
			//Dot product gives the cosine between fragment normal and light direction,
			//positive values meaning that the light actually hit the surface.
			//Max is used to prevent negative values giving negative lighting.
			
		if(lambertian > 0.0)
		{//Applying specular lighting
			vec3 halfwayDirection = normalize(lightDirection + viewDirection);//This normalized vector points in between the two given vectors
			float specAngleCos = max(dot(fragmentNormal, halfwayDirection), 0.0);//Same principle as above in lambertian variable assignment
			
			spec = pow(specAngleCos, shininess);
				//Because spec angle cos is in the range of [0, 1],
				//when calculated to the power of shininess,
				//it gives very small (lighting) values unless the surface direction is closely towards the light.
				//In that case it will give values closer to 1, thus giving us the shining effect.
		}
				
		//Diffuse
		vec3 diffuse = lambertian * lights2[i].rgb;//Multiply light color by the lambertian factor determined earlier.
		
		//Specular
		vec3 specular = vec3(1.0, 1.0, 1.0) * spec;//Multiply pure white(shining) by the specular factor.
		
		lighting = lighting + lightAttenuation * lights2[i].a * (diffuse + specular);
			//Add to the total lighting: light attenuation * light alpha * (diffuse colouring + specular shining)
	}
	
	//Finally, get the fragment color determined by the texture and multiply it by the total lighting computed
	color = textureCube(cubeMap, reflect(normalize(fragmentPosition - cameraPosition), normalize(fragmentNormal))) * 0.5;
	color += texture(tex, fragmentUV) * vec4(lighting, 1.0) * 1.00;
}