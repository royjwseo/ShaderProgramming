#version 330

layout(location=0) out vec4 FragColor0;
layout(location=1) out vec4 FragColor1;

uniform sampler2D u_Texture;

in vec4 v_Color;
//varying vec4 v_Color;//둘 다 상관 X

in vec2 v_TexPos;

void main()
{
	FragColor0 = v_Color;
	FragColor1= texture(u_Texture,v_TexPos);


}
