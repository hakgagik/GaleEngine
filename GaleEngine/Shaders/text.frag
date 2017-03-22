#version 430 core

layout(location = 0) in vec2 texCoord;

uniform sampler2D tex;
//uniform vec4 color;

layout(location = 0) out vec4 out_color;

void main() {
	out_color = vec4(1, 1, 1, texture2D(tex, texCoord).r);
}