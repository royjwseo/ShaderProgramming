#version 330

in vec3 a_Position;

const float c_PI=3.141592;

uniform float u_Time=0;
out vec4 v_Color;


void Flag(){
	vec4 newPosition= vec4(a_Position,1);
	float scaledValue= a_Position.x+0.5;

	float period=2;
	float sinInput= period*(scaledValue)*c_PI*2;
	
	float timeScale=6;
	float newTime= fract(u_Time);

	float sinValue= sin(sinInput+u_Time*timeScale);
	float width= 0.5*(1-scaledValue);

	newPosition.y= width*newPosition.y + width * scaledValue * sinValue;

	v_Color= vec4((sinValue+1)/2);  //0~1로 만들어줘야 함.
	gl_Position = newPosition;
	

}


void SphereMapping(){
	
	float r=1;
	float scale=5;
	float newX= r*sin(a_Position.y*scale)*cos(a_Position.x*scale);
	float newY= r*sin(a_Position.y*scale)*sin(a_Position.x*scale);
	float newZ= r*cos(a_Position.y*scale);

	vec4 newPosition=vec4(newX,newY,newZ,1);

	float newTime= fract(u_Time/5);

	vec4 interpoResult= vec4(a_Position,1)*(1-newTime)+newPosition*newTime;
	gl_Position=interpoResult;
	v_Color= vec4(1);
}

void main()
{
	SphereMapping();
	
}
