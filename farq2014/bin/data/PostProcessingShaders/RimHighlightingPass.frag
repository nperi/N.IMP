#version 150

uniform sampler2D myTexture;
in vec3 norm;
in vec3 sides;
in vec2 v_texCoord;
in vec4 v_color;
out vec4 outputcolor;

void main()
{
    float intensity;
    vec3 n = normalize(normal);
    vec4 color;
    intensity = dot(sides,n);
    
    outputcolor = texture(myTexture, v_texCoord);
    if (intensity >= 64.0)
    {
        outputcolor.b = outputcolor.b / 1.5;
        outputcolor.r = outputcolor.r * 1.5;
        outputcolor.g = outputcolor.g * 1.25;
    }
    else
    {
        outputcolor = outputcolor * v_color;
    }
}