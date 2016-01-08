#version 150

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

uniform vec3 LightPosition;
out vec3 worldNormal;
out vec3 eyeVec;
out vec3 lightVec;
out vec3 vertPos;
out vec3 lightPos;

in vec4 position;
in vec3 normal;

in vec2 texcoord;
out vec2 varyingtexcoord;


void subScatterVS(vec4 ecVert){
    lightVec = LightPosition - ecVert.xyz;
    eyeVec = -ecVert.xyz;
    vertPos = ecVert.xyz;
    lightPos = LightPosition;
}


void main(){
    mat3 normalMat = transpose(inverse(mat3(modelViewMatrix)));
    vec3 vertexNormal = normalize(normalMat * normal);
    worldNormal = normalMat * vertexNormal;
    vec4 ecPos = modelViewProjectionMatrix * position;
    subScatterVS(ecPos);
    varyingtexcoord = texcoord;
    gl_Position = ecPos;
}