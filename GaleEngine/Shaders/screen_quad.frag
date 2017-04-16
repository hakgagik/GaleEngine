#version 430 core

layout(location = 0) in vec2 texCoord;

uniform sampler2D tex;
uniform bool hasTex;
uniform vec4 tint;

layout(location = 0) out vec4 out_color;

void main() {
	out_color = tint;
	if (hasTex) {
		out_color *= texture2D(tex, texCoord).x;
	}
}