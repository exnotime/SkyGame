#version 300 es
layout (location = 0) in vec3 PosL;
layout (location = 1) in vec3 NormalL;
layout (location = 2) in vec2 TexCoord;

layout (location = 0) out vec3 PosW;
layout (location = 1) out vec3 NormalW;
layout (location = 2) out vec2 TexCoordOut;

uniform mat4 g_World;
uniform mat4 g_ViewProj;
void main(){
	TexCoordOut = TexCoord;
	PosW = (g_World * vec4(PosL, 1)).xyz;
	gl_Position = g_ViewProj * vec4(PosW,1);
	NormalW = (g_World * vec4(NormalL, 0)).xyz;
}