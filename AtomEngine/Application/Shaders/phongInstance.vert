#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 5) in mat4 aInstance;

out vec3 fPos;
out vec3 fNormal;

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    mat4 mvp = projection * view * aInstance;
    mat3 normalMatrix = mat3(aInstance);
    normalMatrix = inverse(normalMatrix);
    normalMatrix = transpose(normalMatrix);
    normalMatrix = normalMatrix;

    fPos = vec3(aInstance * vec4(aPos, 1.0f));
    fNormal = normalize(normalMatrix * aNormal);
    gl_Position = mvp * vec4(aPos, 1.0f);
}