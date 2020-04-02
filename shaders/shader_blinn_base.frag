#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(binding = 1) uniform vec3 diffuse;

layout(location = 0) in vec3 fragColor;
layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(diffuse, 1.0);
}