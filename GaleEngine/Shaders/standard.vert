#version 430 core

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uv;
layout(location = 3) in vec4 in_tangent;

uniform mat4 modelViewMatrix;
uniform mat4 MVPMatrix;
uniform mat3 normalMatrix;

layout(location = 0) out vec3 out_position;
layout(location = 1) out vec3 out_normal;
layout(location = 2) out vec2 out_uv;
layout(location = 3) out vec3 out_tangent;
layout(location = 4) out vec3 out_bitangent;

void main() {
	gl_Position = MVPMatrix * in_position;

	out_position = (modelViewMatrix * in_position).xyz;
	out_uv = in_uv;

	vec3 N = in_normal;
	vec3 T = in_tangent.xyz;
	vec3 B = cross(N, T) * in_tangent.w;
	out_normal = normalize(normalMatrix * N);
	out_tangent = normalize(modelViewMatrix * vec4(T, 0)).xyz;
	out_bitangent = normalize(modelViewMatrix * vec4(B, 0)).xyz;
}