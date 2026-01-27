#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 mvp;

out vec3 pos;

void main() {
	gl_Position = mvp * vec4(position + vec3(-1.0, -1.0, 1.0), 1.0);
	pos = normal;
}

~

#version 330 core

in vec3 pos;
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
	FragColor = vec4((pos + 0.5) + (objectColor * lightColor), 1.0);
}