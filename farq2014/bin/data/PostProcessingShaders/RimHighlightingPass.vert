#version 150

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec3 normal;
in vec4 color;
in vec2 texcoord;
out vec2 v_texCoord;
out vec3 sides
out vec3 norm;
out vec4 v_color;

void main(){
    mat3 normalMat = transpose(inverse(mat3(modelViewMatrix)));
    vec3 vertexNormal = normalize(normalMat * normal);
    worldNormal = normalMat * vertexNormal;
    v_texCoord = texcoord;
    norm = normal;
    v_color = color;
    gl_Position = modelViewProjectionMatrix * position;
    sides = gl_Position.xyz;
    sides.x = sides.x - 128.0;
}