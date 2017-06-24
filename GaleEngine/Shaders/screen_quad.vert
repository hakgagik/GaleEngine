#version 430 core

layout(location = 0) in vec2 uv;

uniform vec4 posAndScale;
uniform float depth;

layout(location = 0) out vec2 texCoord;

void main() {
	gl_Position = vec4(posAndScale.x + uv.x * posAndScale.z, posAndScale.y + uv.y * posAndScale.w, depth, 1);
	texCoord = uv;
}