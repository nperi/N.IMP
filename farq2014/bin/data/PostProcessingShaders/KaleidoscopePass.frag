#version 150

uniform sampler2D tex0;
uniform float segments;

in vec2 varyingtexcoord;
out vec4 outputColor;

void main (void){
    vec2 uv = varyingtexcoord;
    vec2 normed = 2.0 * uv - 1.0;
    float r = length(normed);
    float theta = atan(normed.y / abs(normed.x));
    theta *= segments;
    
    vec2 newUv = (vec2(r * cos(theta), r * sin(theta)) + 1.0) / 2.0;
    
    outputColor = texture(tex0, newUv);
}
