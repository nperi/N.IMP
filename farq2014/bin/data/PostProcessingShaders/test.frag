#version 150

uniform sampler2D tex0;

in vec2 varyingtexcoord;
out vec4 outputColor;

void main (void){
    vec2 pos = varyingtexcoord;
    vec4 col = texture(tex0, pos);
    col.r = 0;
    col.g = 0;
//    outputColor = texture(tex0, pos);
    outputColor = col;
}
