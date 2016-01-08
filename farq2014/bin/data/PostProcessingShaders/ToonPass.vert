#version 150

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec3 normal;
in vec4 color;
in vec2 texcoord;
out vec2 varyingtexcoord;
out vec3 v
out vec3 N;

void main(){
    mat3 normalMat = transpose(inverse(mat3(modelViewMatrix)));
    vec3 N = normalize(normalMat * normal);
    
    v = modelViewMatrix * position;
    
    varyingtexcoord = texcoord;
    gl_Position = modelViewProjectionMatrix * position;
}