#version 150

uniform sampler2D tex;

uniform float xPixels;
uniform float yPixels;

in vec2 varyingtexcoord;
out vec4 outputcolor;
                        
void main()
{
    vec2 texCoords = vec2(floor(varyingtexcoord.x * xPixels) / xPixels, floor(varyingtexcoord.y * yPixels) / yPixels);
    outputcolor = texture(tex, texCoords);
}