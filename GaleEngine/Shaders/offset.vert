#version 430 core

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uv;
layout(location = 3) in vec4 in_tangent;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform vec3 offset;

layout(location = 0) out vec3 out_position;
layout(location = 1) out vec3 out_normal;
layout(location = 2) out vec2 out_uv;
layout(location = 3) out vec3 out_tangent;
layout(location = 4) out vec3 out_bitangent;

void main() {
	vec4 offset_position = (modelMatrix * in_position) + vec4(offset, 0);

	gl_Position = projectionMatrix * viewMatrix * offset_position;

	out_position = (viewMatrix * offset_position).xyz;
	out_uv = in_uv;

	vec3 N = in_normal;
	vec3 T = in_tangent.xyz;
	vec3 B = cross(N, T) * in_tangent.w;
	out_normal = normalize(normalMatrix * N);
	out_tangent = normalize(modelViewMatrix * vec4(T, 0)).xyz;
	out_bitangent = normalize(modelViewMatrix * vec4(B, 0)).xyz;
}