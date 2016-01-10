#version 150
uniform sampler2DRect image;
uniform float rand;
uniform int range;

in vec3 pos;
out vec4 outputColor;

void main (void)
{
    float e = 2.718281828459045235360287471352;
    vec2 texCoord = vec2(pos.x , pos.y);
    vec4 col = texture(image,texCoord);
    
    int blur_w = 8;
    float pi = 3.1415926535;
    vec4 gws = vec4(0.0,0.0,0.0,1.0);
    float weight;
    float k = 1.0;
    
    weight = 1.0/(float(blur_w)*2.0+1.0)/(float(blur_w)*2.0+1.0);

// This algorithm doesn't support Intel HD graphics...

//    for (int i = -blur_w;i < blur_w;i++){
//       for (int j = -blur_w;j < blur_w;j++){
//            weight = pow(1.0/2.0*pi*k*k,-((float(i*i)+float(j*j))/2.0*k*k))/(float(blur_w)+1.0);//ガウシアンフィルタの重み係数を計算
//            gws = gws + texture2DRect(image,vec2(pos.x+float(j),pos.y+float(i)))*weight*1.3;
//       }
//    }

    for (int i = 0;i < blur_w*blur_w;i++){
        gws = gws + texture(image,vec2(pos.x+float(mod(float(i),float(blur_w))),pos.y+float(i/blur_w)))*weight*1.3;
        gws = gws + texture(image,vec2(pos.x-float(mod(float(i),float(blur_w))),pos.y+float(i/blur_w)))*weight*1.3;
        gws = gws + texture(image,vec2(pos.x+float(mod(float(i),float(blur_w))),pos.y-float(i/blur_w)))*weight*1.3;
        gws = gws + texture(image,vec2(pos.x-float(mod(float(i),float(blur_w))),pos.y-float(i/blur_w)))*weight*1.3;
    }


    outputColor.rgba = col+gws;
}