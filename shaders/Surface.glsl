#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;

uniform mat4 mvp;
uniform mat4 model_matrix;
uniform mat3 normal_matrix;

out vec3 frag_world_normal;
out vec3 frag_pos;
out vec2 frag_uv;

void main() {
	frag_pos = vec3(model_matrix * vec4(position, 1.0));
	frag_uv = uv;
    gl_Position = mvp * vec4(position, 1.0);
}

~

#version 330 core

in vec3 frag_pos;
in vec2 frag_uv;

// Attenuation constants
float CA = 1.0;
float LA = 0.7;
float QA = 1.7;

const float Shininess = 0.0;
const float Strength = 0.5;
const float MaxRadius = 5.0;

uniform vec3 directionalLightPosition;
uniform vec3 directionalLightColor;
uniform vec3 ambientLightColor;
uniform vec3 EyeDirection;

uniform sampler2D ground;
uniform sampler2D ground_normal;

out vec4 FragColor;

void main() {
	vec3 lightDirection = directionalLightPosition - frag_pos;
	float distanceFromLight = length(lightDirection);
	lightDirection = normalize(lightDirection);

	vec3 normal = vec3(0.0, 1.0, 0.0);

	float diffuse = max(0.0, dot(normal, lightDirection));
	float attenuation = 1 / (CA + LA * distanceFromLight + QA * distanceFromLight * distanceFromLight);
	
	if(distanceFromLight > MaxRadius) {
		attenuation = 0.0;
	}

	vec3 directionalLight = directionalLightColor * diffuse * attenuation;
	
    vec3 litColor = vec3(0.4, 0.4, 0.4) * (ambientLightColor + directionalLight);
	FragColor = vec4(litColor, 1.0);
}