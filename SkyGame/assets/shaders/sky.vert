#version 300 es
in vec3 PosL;
in vec3 NormalL;
in vec2 TexIn;

out vec3 Pos;

uniform mat4 g_ProjView;
uniform mat4 g_World;

void main(){
	vec4 PosV = g_ProjView * (g_World * vec4(PosL.xyz,1));
	gl_Position = PosV.xyww;
	Pos = PosL.xyz;
}