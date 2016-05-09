#version 300 es

layout (location = 0) in vec3 PosW;
layout (location = 1) in vec3 NormalW;
layout (location = 2) in vec2 TexCoordOut;


layout(location = 0) out vec4 outColor;

uniform vec3 g_Color;
uniform vec3 g_CamPos;
//uniform vec3 g_LightDir;

void main(){
	const vec3 lightDir = normalize(vec3(0.4f, -1.0f, -0.1f));

	vec3 normal = normalize(NormalW);
	float NdotL = dot(normal, -lightDir);
	float diffuse = NdotL * 0.5f + 0.5f; //half lambert shading
	float specular = 0.0f;
	vec3 LightColor = vec3(0);
	if(NdotL > 0.0f){
		vec3 halfDir = normalize( -lightDir + (g_CamPos - PosW));
		specular = max(0.0f, dot(halfDir, normal));
		specular = pow(specular, 2.0f);
	}
	LightColor = g_Color * (diffuse + specular + 0.1f); //0.2 ambient
	outColor = vec4(LightColor,1);
}