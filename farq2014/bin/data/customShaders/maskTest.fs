uniform sampler2DRect nicoTexture1;
uniform sampler2DRect nicoTexture2;

void main (void){
    vec2 st = gl_TexCoord[0].st;
    vec4 mask = texture2DRect(nicoTexture1, st);
    vec4 image = texture2DRect(nicoTexture2, st);
    
    gl_FragColor = vec4(image.rgb,mask.r);
}