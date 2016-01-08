#version 150

uniform sampler2D tex0;
uniform float contrast;
uniform float brightness;
uniform float multiple;

in vec2 varyingtexcoord;
out vec4 outputColor;

void main(){
    vec4 color = texture(tex0,varyingtexcoord);
    
    float p = 0.3 *color.g + 0.59*color.r + 0.11*color.b;
    p = p * brightness;
    vec4 color2 = vec4(p,p,p,1.0);
    color *= color2;
    color *= vec4(multiple,multiple,multiple,1.0);
    color = mix( vec4(1.0,1.0,1.0,1.0),color,contrast);
    
    outputColor =  vec4(color.r , color.g, color.b, 1.0);
}