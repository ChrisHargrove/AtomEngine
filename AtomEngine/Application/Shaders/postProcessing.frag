#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D myTexture;

uniform bool inversion = false;
uniform bool greyscale = false;
uniform bool useKernel = false;

//Offsets for the Kernal to use when grabbing pixels.
const float offset = 1.0 / 300.0;

//Creating Offset Structure for use with Kernals
vec2 kernelOffsets[9] = vec2[](
    vec2(-offset, offset),
    vec2(0.0f, offset),
    vec2(offset, offset),
    vec2(-offset, 0.0f),
    vec2(0.0f,0.0f),
    vec2(offset, 0.0f),
    vec2(-offset, -offset),
    vec2(0.0f, -offset),
    vec2(offset, -offset)
);

//Generic Kernel Structure that will be populated by shader calls.
uniform float kernel[9];


void main()
{
    if(useKernel == true){
        vec3 sampleTex[9];
        for(int i = 0; i < 9; i++){
            sampleTex[i] = vec3(texture(myTexture, TexCoords.st + kernelOffsets[i]));
        }
        vec3 color = vec3(0.0);
        for(int i = 0; i < 9; i++){
            color += sampleTex[i] * kernel[i];
        }
        FragColor = vec4(color, 1.0);
    }
    else if(inversion == true){
        FragColor = vec4(vec3(1.0 - texture(myTexture, TexCoords).rgb), 1.0);
    }
    else if(greyscale == true){
        FragColor = vec4(texture(myTexture, TexCoords).rgb, 1.0);
        float average = (0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b) / 3;
        FragColor = vec4(average, average, average, 1.0);
    }
    else{
        FragColor = vec4(texture(myTexture, TexCoords).rgb, 1.0);
    }
}