#version 150

uniform sampler2D tDiffuse;
uniform float v;
uniform float r;
in vec2 varyingtexcoord;
out vec4 outputColor;

void main() {
    vec2 vUv = varyingtexcoord;
    vec4 sum = vec4( 0.0 );
    
    float vv = v * abs( r - vUv.y );
    
    sum += texture( tDiffuse, vec2( vUv.x, vUv.y - 4.0 * vv ) ) * 0.051;
    sum += texture( tDiffuse, vec2( vUv.x, vUv.y - 3.0 * vv ) ) * 0.0918;
    sum += texture( tDiffuse, vec2( vUv.x, vUv.y - 2.0 * vv ) ) * 0.12245;
    sum += texture( tDiffuse, vec2( vUv.x, vUv.y - 1.0 * vv ) ) * 0.1531;
    sum += texture( tDiffuse, vec2( vUv.x, vUv.y ) ) * 0.1633;
    sum += texture( tDiffuse, vec2( vUv.x, vUv.y + 1.0 * vv ) ) * 0.1531;
    sum += texture( tDiffuse, vec2( vUv.x, vUv.y + 2.0 * vv ) ) * 0.12245;
    sum += texture( tDiffuse, vec2( vUv.x, vUv.y + 3.0 * vv ) ) * 0.0918;
    sum += texture( tDiffuse, vec2( vUv.x, vUv.y + 4.0 * vv ) ) * 0.051;
    
    outputColor = sum;
}