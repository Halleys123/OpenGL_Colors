#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 mvp;

out vec3 pos;
out vec2 frag_uv;

void main() {
	gl_Position = mvp * vec4(position.x, position.z, position.y, 1.0);
	pos = position;
	frag_uv = uv;
}

~

#version 330 core

in vec3 pos;
in vec2 frag_uv;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform sampler2D tex;

out vec4 FragColor;

void main() {
	FragColor = texture(tex, frag_uv);
}