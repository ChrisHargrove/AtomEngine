#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D myTexture;

void main()
{
    FragColor = vec4(texture(myTexture, TexCoords).rgb, 1.0);
}