#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D myTexture;

void main()
{
    //FragColor = vec4(TexCoords.x,TexCoords.y,0.0,1.0); 

    //float var = texture(screenTexture, TexCoords).r;
    //FragColor = vec4(var,var,var,1.0);


    FragColor = vec4(texture(myTexture, TexCoords).rgb, 1.0);
}