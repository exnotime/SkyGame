#version 300 es
precision highp float;
layout (location = 0) in vec3 PosW;
layout (location = 1) in vec3 NormalW;
layout (location = 2) in vec2 TexCoordOut;
layout (location = 3) in vec3 TangentW;

layout(location = 0) out vec4 outColor;

uniform vec3 g_Color;
uniform vec3 g_CamPos;
uniform sampler2D g_Texture;
uniform sampler2D g_NormalMap;
//uniform vec3 g_LightDir;
#define MOD3 vec3(443.8975,397.2973, 491.1871)
#define LIGHT_DIFFUSE vec3(0.6)
#define LIGHT_SPECULAR vec3(0.7)
vec3 hash32(vec2 p)
{
	vec3 p3 = fract(vec3(p.xyx) * MOD3);
    p3 += dot(p3, p3.yxz+19.19);
    return fract(vec3((p3.x + p3.y)*p3.z, (p3.x+p3.z)*p3.y, (p3.y+p3.z)*p3.x));
}
vec3 ditherRGB(vec3 c, vec2 seed){
	return c + hash32(seed) / 255.0;
}

vec3 calcBumpedNormal(vec3 Normal, vec3 Tangent, sampler2D normalMap, vec2 uv){
	vec3 normal = normalize(Normal);
	vec3 tangent = normalize(Tangent);
	tangent = normalize(tangent - dot(tangent,normal) * normal);
	vec3 bitangent = cross(tangent,normal);

	vec3 bump = pow(texture(normalMap,uv).xyz,vec3(2.2));
	bump = (bump * 2.0) - 1.0;

	vec3 newNormal;
	mat3 TBN = mat3(tangent,bitangent,normal);
	newNormal = TBN * bump;
	return normalize(newNormal);
}

void main(){
	const vec3 lightDir = normalize(vec3(0.4f, -1.0f, -0.1f));

	vec3 normal = calcBumpedNormal(NormalW, TangentW, g_NormalMap, TexCoordOut);
	float NdotL = dot(normal, -lightDir);
	float diffuse = NdotL * 0.5f + 0.5f;//max(0.0f, NdotL);
	float specular = 0.0f;
	vec3 LightColor = vec3(0);
	if(NdotL > 0.0f){
		vec3 halfDir = normalize( -lightDir + (g_CamPos - PosW));
		specular = max(0.0f, dot(halfDir, normal));
		//specular = pow(specular, 1.0f);
	}
	vec3 color = g_Color * pow(texture(g_Texture, TexCoordOut).rgb, vec3(2.2)); //perform gamma correction
	LightColor = color * ((diffuse * LIGHT_DIFFUSE) + (specular * LIGHT_SPECULAR) + 0.05f); //0.05 ambient
	LightColor = ditherRGB(LightColor, gl_FragCoord.xy - vec2(0.5));
	outColor = vec4(pow(LightColor, vec3(1.0 / 2.2)),1);
}