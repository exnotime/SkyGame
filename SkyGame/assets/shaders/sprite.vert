#version 300 es
layout (location = 0) in vec3 PosL;
layout (location = 1) in vec3 NormalL;
layout (location = 2) in vec2 TexCoord;

layout (location = 0) out vec2 TexCoordOut;
void main(){
	TexCoordOut = TexCoord;
	gl_Position = vec4(PosL,1);
}