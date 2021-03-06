#version 150

uniform sampler2D tex;
uniform vec2 aspect;
uniform float hue;
uniform float saturation;

in vec2 varyingtexcoord;
out vec4 outputColor;

vec2 texel = vec2(1.0 / aspect.x, 1.0 / aspect.y);

// hard coded matrix values
mat3 G[9] = mat3[]( mat3( 0.3535533845424652, 0, -0.3535533845424652, 0.5, 0, -0.5, 0.3535533845424652, 0, -0.3535533845424652 ),
                   mat3( 0.3535533845424652, 0.5, 0.3535533845424652, 0, 0, 0, -0.3535533845424652, -0.5, -0.3535533845424652 ),
                   mat3( 0, 0.3535533845424652, -0.5, -0.3535533845424652, 0, 0.3535533845424652, 0.5, -0.3535533845424652, 0 ),
                   mat3( 0.5, -0.3535533845424652, 0, -0.3535533845424652, 0, 0.3535533845424652, 0, 0.3535533845424652, -0.5 ),
                   mat3( 0, -0.5, 0, 0.5, 0, 0.5, 0, -0.5, 0 ),
                   mat3( -0.5, 0, 0.5, 0, 0, 0, 0.5, 0, -0.5 ),
                   mat3( 0.1666666716337204, -0.3333333432674408, 0.1666666716337204, -0.3333333432674408, 0.6666666865348816, -0.3333333432674408, 0.1666666716337204, -0.3333333432674408, 0.1666666716337204 ),
                   mat3( -0.3333333432674408, 0.1666666716337204, -0.3333333432674408, 0.1666666716337204, 0.6666666865348816, 0.1666666716337204, -0.3333333432674408, 0.1666666716337204, -0.3333333432674408 ),
                   mat3( 0.3333333432674408, 0.3333333432674408, 0.3333333432674408, 0.3333333432674408, 0.3333333432674408, 0.3333333432674408, 0.3333333432674408, 0.3333333432674408, 0.3333333432674408 ));

vec3 hsv(float h,float s,float v) { return mix(vec3(1.),clamp((abs(fract(h+vec3(3.,2.,1.)/3.)*6.-3.)-1.),0.,1.),s)*v; }

void main(void)
{
    mat3 I;
    float cnv[9];
    vec3 sample;
    
    /* fetch the 3x3 neighbourhood and use the RGB vector's length as intensity value */
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
        {
            sample = texture(tex, varyingtexcoord + texel * vec2(i-1.0,j-1.0)).rgb;
            I[i][j] = length(sample);
        }
    }
    
    /* calculate the convolution values for all the masks */
    for (int i=0; i<9; i++)
    {
        float dp3 = dot(G[i][0], I[0]) + dot(G[i][1], I[1]) + dot(G[i][2], I[2]);
        cnv[i] = dp3 * dp3;
    }
    
    float M = (cnv[0] + cnv[1]) + (cnv[2] + cnv[3]);
    float S = (cnv[4] + cnv[5]) + (cnv[6] + cnv[7]) + (cnv[8] + M);
    
    outputColor = vec4(hsv(hue, saturation, sqrt(M/S)), 1.0);
}