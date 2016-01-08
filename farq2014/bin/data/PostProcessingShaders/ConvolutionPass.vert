#version 150

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform vec2 imageIncrement;
uniform vec2 resolution;
uniform int KERNEL_SIZE;

in vec4 position;
in vec4 color;
in vec4 normal;
in vec2 texcoord;
out vec2 vUv;
out vec2 scaledImageIncrement;
out int size_kernel;

void main()
{
    //                gl_TexCoord[0] = gl_MultiTexCoord0;
    scaledImageIncrement = imageIncrement * resolution;
    size_kernel = KERNEL_SIZE;
    vUv = texcoord - ( ( size_kernel - 1.0 ) / 2.0 ) * scaledImageIncrement;
    gl_Position = modelViewProjectionMatrix * position;
}