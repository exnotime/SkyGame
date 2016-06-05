#version 300 es
precision highp float;
layout (location = 0) in vec2 TexCoordOut;

layout(location = 0) out vec4 outColor;
uniform sampler2D g_Image;

void main(){
	outColor = texture(g_Image, TexCoordOut);
}