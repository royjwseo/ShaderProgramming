#version 330

layout(location=0) out vec4 FragColor;
in vec4 fs_Color;


void main()
{
	FragColor = vec4(fs_Color);

}
