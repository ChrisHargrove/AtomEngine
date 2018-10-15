#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 fColor;

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
	fColor = aPos;

    mat4 camFix = mat4(mat3(view));

	gl_Position = (projection * camFix * model * vec4(aPos, 1.0f)).xyww;
}
