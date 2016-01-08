#version 150

uniform sampler2DRect Tex0, Tex1; //these are our texture names, set in openFrameworks on the shader object in set up

in vec2 varyingtexcoord;
out vec4 outputColor;

void main (void)
{
	//sample both textures
	vec4 image = texture(Tex0, varyingtexcoord);
	vec4 composite = texture(Tex1, varyingtexcoord);
	
	//use the color from the image, but use the r channel of the mask as the alpha channel of our output
	outputColor = vec4(image.rgb,composite.r);
	
}
