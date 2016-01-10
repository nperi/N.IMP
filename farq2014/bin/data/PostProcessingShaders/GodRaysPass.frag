#version 150

uniform sampler2D rtex;
uniform sampler2D otex;

const int NUM_SAMPLES = 50;

uniform vec2 lightPositionOnScreen;
uniform float lightDirDOTviewDir;

in vec2 varyingtexcoord;
in vec4 col;
out vec4 outputcolor;

void main(void)
{
    vec4 origColor = texture(otex, varyingtexcoord);
    vec4 raysColor = texture(rtex, varyingtexcoord);
    
    if (lightDirDOTviewDir>0.0){
        float exposure	= 0.1/float(NUM_SAMPLES);
        float decay		= 1.0 ;
        float density	= 0.5;
        float weight	= 6.0;
        float illuminationDecay = 1.0;
        
        vec2 deltaTextCoord = vec2( varyingtexcoord - lightPositionOnScreen);
        vec2 textCoo = varyingtexcoord;
        deltaTextCoord *= 1.0 / float(NUM_SAMPLES) * density;
        
        
        
        for(int i=0; i < NUM_SAMPLES ; i++)
        {
            textCoo -= deltaTextCoord;
            vec4 tsample = texture(rtex, textCoo );
            tsample *= illuminationDecay * weight;
            raysColor += tsample;
            illuminationDecay *= decay;
        }
        raysColor *= exposure * lightDirDOTviewDir;
        float p = 0.3 *raysColor.g + 0.59*raysColor.r + 0.11*raysColor.b;
        outputcolor = origColor + p;
    } else {
        outputcolor = origColor;
    }
}