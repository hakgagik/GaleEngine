#version 430

#define MAX_LIGHTS 40

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;

uniform vec4 diffuseColor;
uniform bool hasDiffuseTexture;
uniform sampler2D diffuseTexture;

uniform int lightCount;
uniform vec3 lightPosition[MAX_LIGHTS];
uniform vec3 lightAttenuation[MAX_LIGHTS];
uniform vec3 lightColor[MAX_LIGHTS];
uniform float lightCutoff[MAX_LIGHTS];

layout(location = 0) out vec4 out_color;

void main() {
	vec4 diffuse = diffuseColor;
	if (hasDiffuseTexture) {
		vec4 tex = texture2D(diffuseTexture, in_uv);
		diffuse *= tex;
	}

	vec4 result = vec4(0, 0, 0, diffuse.a);
	vec3 n = normalize(in_normal);
	for (int i = 0; i < lightCount; i++) {
		vec3 l = lightPosition[i] - in_position;
		float d = length(l);
		if (d > lightCutoff[i]) { continue; }
		l = normalize(l);
		float attenuation = dot(lightAttenuation[i], vec3(1, d, d*d));
		float dotProd = dot(n, l);
		if (dotProd <= 0) { continue; }
		result.xyz += diffuse.xyz * dotProd * lightColor[i] / attenuation;
	}
	out_color = result;
}