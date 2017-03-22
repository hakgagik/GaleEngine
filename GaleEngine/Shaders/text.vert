#version 430 core

layout(location = 0) in vec2 uv;

uniform vec4 posAndScale;

layout(location = 0) out vec2 texCoord;

void main() {
	gl_Position = vec4(posAndScale.x + uv.u * posAndScale.z, posAndScale.y + uv.v * psoAndScale.w, 0, 1);
	texCoord = uv;
}