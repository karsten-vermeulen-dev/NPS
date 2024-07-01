#version 400

in vec3 vertexIn;  
in vec4 colorIn;
in vec2 textureIn;

out vec3 vertexOut;
out vec4 colorOut;
out vec2 textureOut;

uniform mat4 model;

void main(void)
{
	colorOut = colorIn;
	textureOut = textureIn;
	gl_Position = model * vec4(vertexIn, 1.0f);
}