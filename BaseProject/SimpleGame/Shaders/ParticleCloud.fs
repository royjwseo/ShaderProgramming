#version 330

layout(location=0) out vec4 FragColor;

in vec4 v_Color;
//varying vec4 v_Color;//�� �� ��� X

void main()
{
	FragColor = v_Color;

}
