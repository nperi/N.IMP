#version 150

uniform sampler2D myTexture;
uniform float fAspect;

uniform vec3 startColor;
uniform vec3 endColor;

uniform float radialScale;//0. - 1.0 - 2.0
uniform float brightness;//0.-1.0, deff:2.5

in vec2 varyingtexcoord;
out vec4 outputcolor;

void main() {
    vec2 vUv = varyingtexcoord;
    vec2 vSunPositionScreenSpace = vec2(0.5);
    
    vec2 diff = vUv - vSunPositionScreenSpace;
    
    // Correct for aspect ratio
    
    diff.x *= fAspect;
    
    float prop = length( diff ) / radialScale;
    prop = clamp( 2.5 * pow( 1.0 - prop, 3.0 ), 0.0, 1.0 );
    
    vec3 color = mix( startColor, endColor, 1.0 - prop );
    
    vec4 base = texture(myTexture, vUv);
    outputcolor = vec4(base.xyz * color, 1.0);
    
}