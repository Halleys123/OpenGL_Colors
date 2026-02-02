#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 mvp;

out vec3 pos;

void main() {
	gl_Position = mvp * vec4(position, 1.0);
}

~

#version 330 core

in vec3 pos;
uniform vec3 lightColor;

out vec4 FragColor;

void main() {
	FragColor = vec4(lightColor, 1.0);
}