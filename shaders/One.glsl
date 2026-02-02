#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 color;
layout(location = 3) in vec3 normal;

uniform mat4 mvp;
uniform mat3 mv;

out vec3 frag_VertexColor;
out vec2 frag_uv;
out vec3 frag_world_normal;
out vec3 frag_pos;
out vec3 frag_normal;


void main() {
	frag_VertexColor = color;
	frag_uv = uv;
	frag_pos = mv * position;
	frag_world_normal = normalize(mv * normal);
	frag_normal = normal;
	gl_Position = mvp * vec4(position, 1.0);
}

~

#version 330 core

in vec3 frag_VertexColor;
in vec2 frag_uv;
in vec3 frag_world_normal;
in vec3 frag_pos;
in vec3 frag_normal;

// Attenuation constants
float ConstantAttenuation = 0.2;
float LinearAttenuation = 1.0;
float QuadraticAttenuation = 1.0;

const float Shininess = 1.0;
const float Strength = 1.5;

uniform vec3 directionalLightPosition;
uniform vec3 directionalLightColor;
uniform vec3 ambientLightColor;
uniform vec3 EyeDirection;

uniform sampler2D tex;

out vec4 FragColor;

void main() {
    // Calculate light direction and distance
    vec3 lightDirection = directionalLightPosition - frag_pos;
    float lightDistance = length(lightDirection);
    
    // Avoid division by zero
    if (lightDistance > 0.0) {
        lightDirection = lightDirection / lightDistance;
    } else {
        lightDirection = vec3(0.0, 0.0, 1.0); // Default direction
    }
    
    // Calculate attenuation (point light)
    float attenuation = 1.0 / (ConstantAttenuation + 
                              LinearAttenuation * lightDistance + 
                              QuadraticAttenuation * lightDistance * lightDistance);
    
    // Calculate half vector for specular (Blinn-Phong)
    vec3 viewDirection = normalize(EyeDirection);
    vec3 halfVector = normalize(lightDirection + viewDirection);
    
    // Use world-space normal (transformed by normal matrix)
    vec3 normal = normalize(frag_world_normal);
    
    // Diffuse lighting (Lambertian)
    float diffuse = max(0.0, dot(normal, lightDirection));
    
    // Specular lighting (Blinn-Phong)
    float specular = 0.0;
    if (diffuse > 0.0) {
        float specAngle = max(0.0, dot(normal, halfVector));
        specular = pow(specAngle, Shininess) * Strength;
    }
    
    // Combine lighting components
    vec3 scatteredLight = ambientLightColor + directionalLightColor * diffuse * attenuation;
    vec3 reflectedLight = directionalLightColor * specular * attenuation;
    
    // Get texture color
    vec4 texColor = texture(tex, frag_uv);
    
    // Apply lighting to texture - THIS IS LINE 46
    vec3 litColor = texColor.rgb * scatteredLight + reflectedLight;
    
    // Clamp to valid range [0, 1]
    litColor = min(litColor, vec3(1.0));
    
    FragColor = vec4(litColor, texColor.a);
}