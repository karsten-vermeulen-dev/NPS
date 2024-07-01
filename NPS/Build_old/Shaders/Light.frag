#version 400
#define TOTAL_LIGHTS 1

struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;

	//float attenuationLinear;
	//float attenuationConstant;
	//float attenuationQuadratic;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

in vec3 vertexOut;
in vec4 colorOut;
in vec2 textureOut;
in vec3 normalOut;

out vec4 pixelColor;

uniform bool isTextured;
uniform sampler2D textureImage;

uniform Light light;
uniform Material material;
uniform vec3 cameraPosition;
//uniform Light lights[TOTAL_LIGHTS];

//=====================================================================
vec3 AmbientColor(in Light light)
{
	return light.ambient * material.ambient;
}
//=====================================================================
vec3 DiffuseColor(in Light light)
{
	//calculate the vector between light position and each vertex
	vec3 lightDirection = normalize(light.position - vertexOut);

	//calculate the dot product between the two vectors
	float lightIntensity = max(dot(lightDirection, normalOut), 0.0);

	//final diffuse color value
	return light.diffuse * material.diffuse * lightIntensity;
}
//=====================================================================
vec3 SpecularColor(in Light light)
{
	//calculate the vector between light position and each vertex
	vec3 lightDirection = normalize(light.position - vertexOut);

	//calculate viewing vector between camera and each vertex
	vec3 viewDirection = normalize(cameraPosition - vertexOut);

	//reflect inverted light direction around the normal
	vec3 reflection = reflect(-lightDirection, normalOut);

	//calculate the specular term
	float specularTerm = pow(max(dot(viewDirection, reflection), 0.0), material.shininess);

	//final specular color
	return light.specular * material.specular * specularTerm;
}
//=====================================================================
//float Attenuation(in Light light)
//{
//	float lightDistance = length(light.position - vertexOut);
//	return 1.0 / (light.attenuationConstant + 
//	              light.attenuationLinear * lightDistance + 
//	              light.attenuationQuadratic * lightDistance * lightDistance);
//}
//=====================================================================
void main(void)
{
	for(int i = 0; i < TOTAL_LIGHTS; i++)
	{
		vec3 totalColor = AmbientColor(light) + 
							DiffuseColor(light) + 
							SpecularColor(light);

		//make sure the RGB components never exceed 1.0
		totalColor.r = min(totalColor.r, 1.0);
		totalColor.g = min(totalColor.g, 1.0);
		totalColor.b = min(totalColor.b, 1.0);

		//add on ATTENUATION before appending to final color value
		//pixelColor += vec4(totalColor) * Attenuation(lights[i]), 1.0);
			
		//TEMP do not apply attenuation
		pixelColor += (colorOut * vec4(totalColor, 1.0));
	}

	//if object is also textured then add texel color value
	if(isTextured)
	{
		vec2 uv;
		uv.x = textureOut.x;
		uv.y = 1 - textureOut.y;
		pixelColor *= texture(textureImage, uv);
	}
}