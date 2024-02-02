# version 460 core
out vec4 FragColor;

in vec2 Color;

void main()
{
	FragColor = vec4(Color, 0.0, 1.0);
}