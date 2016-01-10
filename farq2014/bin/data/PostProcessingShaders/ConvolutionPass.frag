#version 150

uniform float kernel[KERNEL_SIZE];
uniform sampler2D readTex;
uniform vec2 imageIncrement;
//            uniform float kernel_size;

in vec2 vUv;
in vec2 scaledImageIncrement;
in int size_kernel;
out vec4 outputcolor;

void main()
{
    
    vec2 imageCoord = vUv;
    vec4 sum = vec4( 0.0, 0.0, 0.0, 0.0 );
    
    for( int i = 0; i < size_kernel; i++ )
    {
        sum += texture( readTex, imageCoord ) * kernel[ i ];
        imageCoord += scaledImageIncrement;
    }
    
    outputcolor = sum;
}