#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}

~

#version 330 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform float exposure;

void main()
{    
    vec3 envColor = texture(skybox, TexCoords).rgb;
    
    // Tone mapping
    envColor = vec3(1.0) - exp(-envColor * exposure);
    
    // Gamma correction
    envColor = pow(envColor, vec3(1.0/2.2));
    
    FragColor = vec4(envColor, 1.0);
}