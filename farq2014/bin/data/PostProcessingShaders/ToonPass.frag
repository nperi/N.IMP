#version 150

uniform sampler2D normalImage;
uniform float textureSizeX;
uniform float textureSizeY;
uniform float normalEdgeThreshold;
uniform float qLevel;
uniform int bSpecular;
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float shinyness;

in vec3 v;
in vec3 N;
in vec2 varyingtexcoord;
out vec4 outputcolor;


vec3 getNormal(vec2 st){
    vec2 texcoord = clamp(st, 0.001, 0.999);
    return texture(normalImage, texcoord).rgb;
}

void main(void){
    float dxtex = 1.0 / textureSizeX;
    float dytex = 1.0 / textureSizeY;
    
    vec2 st = varyingtexcoord;
    // access center pixel and 4 surrounded pixel
    vec3 center = getNormal(st).rgb;
    vec3 left = getNormal(st + vec2(dxtex, 0.0)).rgb;
    vec3 right = getNormal(st + vec2(-dxtex, 0.0)).rgb;
    vec3 up = getNormal(st + vec2(0.0, -dytex)).rgb;
    vec3 down = getNormal(st + vec2(0.0, dytex)).rgb;
    
    // discrete Laplace operator
    vec3 laplace = abs(-4.0*center + left + right + up + down);
    // if one rgb-component of convolution result is over threshold => edge
    vec4 line = texture(normalImage, st);
    if(laplace.r > normalEdgeThreshold
       || laplace.g > normalEdgeThreshold
       || laplace.b > normalEdgeThreshold){
        line = vec4(0.0, 0.0, 0.0, 1.0); // => color the pixel green
    } else {
        line = vec4(1.0, 1.0, 1.0, 1.0); // black
    }
    
    //end Line;
    
    //start Phong
    
    //                vec3 lightPosition = vec3(100.0, 100.0, 50.0);
    vec3 lightPosition = gl_LightSource[0].position.xyz;
    
    vec3 L = normalize(lightPosition - v);
    vec3 E = normalize(-v);
    vec3 R = normalize(-reflect(L,N));
    
    // ambient term
    vec4 Iamb = ambient;
    
    // diffuse term
    vec4 Idiff = texture( normalImage, varyingtexcoord) * diffuse;
    //vec4 Idiff = vec4(1.0, 1.0, 1.0, 1.0) * diffuse;
    Idiff *= max(dot(N,L), 0.0);
    Idiff = clamp(Idiff, 0.0, 1.0);
    
    // specular term
    vec4 Ispec = specular;
    Ispec *= pow(max(dot(R,E),0.0), shinyness);
    Ispec = clamp(Ispec, 0.0, 1.0);
    
    vec4 color = Iamb + Idiff;
    if ( bSpecular == 1 ) color += Ispec;
    // store previous alpha value
    float alpha = color.a;
    // quantize process: multiply by factor, round and divde by factor
    color = floor(0.5 + (qLevel * color)) / qLevel;
    // set fragment/pixel color
    color.a = alpha;
    
    outputcolor = color * line;
}