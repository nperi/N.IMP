#version 150

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;

in vec2 texcoord;
out vec2 varyingtexcoord;
void main(void)
{

    //.. and passes the multi texture coordinates along to the fragment shader
    varyingtexcoord = texcoord;
	//this is a default vertex shader all it does is this...
	gl_Position = modelViewProjectionMatrix * position;

}
