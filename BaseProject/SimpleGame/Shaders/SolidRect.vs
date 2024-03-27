#version 330

in vec3 a_Position;
in vec4 a_Color;
out vec4 fs_Color;

void main()
{
	vec4 newPosition= vec4(a_Position,1);
	gl_Position = newPosition;
	
	if(gl_VertexID==1){
	fs_Color=vec4(1.0f,0.f,0.f,1.f);
	}
	else
	fs_Color=a_Color;
}
