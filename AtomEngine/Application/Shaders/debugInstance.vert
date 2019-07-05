#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 5) in mat4 aInstance;


layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    gl_Position = projection * view * aInstance * vec4(aPos, 1.0f);
}