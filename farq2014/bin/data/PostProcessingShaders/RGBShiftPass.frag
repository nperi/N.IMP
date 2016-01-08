#version 150

uniform sampler2D tDiffuse;
uniform float amount;
uniform float angle;

in vec2 varyingtexcoord;
out vec4 outputcolor;

void main() {
    vec2 vUv = varyingtexcoord;
    vec2 offset = amount * vec2( cos(angle), sin(angle));
    vec4 cr = texture(tDiffuse, vUv + offset);
    vec4 cga = texture(tDiffuse, vUv);
    vec4 cb = texture(tDiffuse, vUv - offset);
    outputcolor = vec4(cr.r, cga.g, cb.b, cga.a);
    
}