#version 400

in vec4 colorOut;
out vec4 pixelColor;

void main(void)
{
	pixelColor = colorOut;
}