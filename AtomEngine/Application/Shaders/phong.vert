#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 fPos;
out vec3 fNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    fPos = vec3(model * vec4(aPos, 1.0f));
    fNormal = mat3(transpose(inverse(model))) *aNormal;
    gl_Position = projection * view * model * vec4(fPos, 1.0f);
}