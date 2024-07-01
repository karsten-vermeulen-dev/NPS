#version 400

in vec3 vertexIn;  
in vec4 colorIn;
out vec4 colorOut;

void main(void)
{
	colorOut = colorIn;
	gl_Position = vec4(vertexIn, 1.0f);
}