#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 fColor;

uniform vec3 aColor;

uniform mat4 model;

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

void main()
{
    fColor = aColor;

    gl_Position = projection * view * model * vec4(aPos, 1.0f);

}