#version 330

in vec3 a_Position;
//attribute vec3 a_Position; 둘중 하나
in float a_StartTime;
in vec3 a_Velocity;
in float a_LifeTime;
in float a_Amp;
in float a_Period;
in float a_Value;
in vec4 a_Color;

out vec4 v_Color;

const vec3 c_StartPos=vec3(-1,0,0);
const vec3 c_Velocity=vec3(2.0,0,0);
const vec3 c_ParaVelocity=vec3(2.0,2.0,0);
const vec2 c_2DGravity=vec2(0.0,-0.9);
const float c_PI=3.141592;

uniform float u_Time=0;
uniform float u_Period=2.0;
uniform vec2 u_WindAcc=vec2(0,0);
uniform vec2 u_AttractPos=vec2(0,0);

void Line(){
	float newTime= abs(fract(u_Time/u_Period)-0.5)*2.0; //abs(fract)방식 알기.

	vec4 newPosition;
	//newPosition=vec4(a_Position+c_StartPos+ c_Velocity*u_Time,1);
	
	newPosition.xyz=(c_StartPos+a_Position)+c_Velocity*newTime;
	newPosition.w=1;



	gl_Position = newPosition;
	v_Color=a_Color;
}

void Circle(){
	
	float newTime=fract(u_Time/u_Period)*c_PI*2.0;
	vec2 trans = vec2(cos(newTime),sin(newTime));
	vec4 newPosition;
	newPosition.xy=a_Position.xy+trans;
	newPosition.zw=vec2(0,1);
	gl_Position=newPosition;
	v_Color=a_Color;

}

void Parabola(){
	float newTime= fract(u_Time/u_Period);
	float t=newTime;
	float tt= t*t;
	vec4 newPosition;
	float transX= (a_Position.x+c_StartPos.x)+c_ParaVelocity.x*newTime+
			0.5*c_2DGravity.x*tt;
	float transY= (a_Position.y+c_StartPos.y)+c_ParaVelocity.y*newTime+
			0.5*c_2DGravity.y*tt;

	

	newPosition.xy=vec2(transX,transY);
	newPosition.zw=vec2(0,1);
	gl_Position=newPosition;
	v_Color=a_Color;
}

void Triangle(){
	

}

void SinShape(){//중력 설정 vec2(0.0,-4.9);
	float t= u_Time-a_StartTime;
	
	//t= a_LifeTime* fract(t/a_LifeTime);
	float amp=a_Amp;
	float period=a_Period;

	vec4 newPosition = vec4(a_Position,1);

	if(t>0){
		float tt=t*t;
		t= a_LifeTime* fract(t/a_LifeTime); //생존 주기 반복을 위한 필수 식.
		vec2 newVel = a_Velocity.xy+c_2DGravity*t;
		vec2 newDir= vec2(-newVel.y,newVel.x);
		newDir=normalize(newDir);
		newPosition.xy= newPosition.xy+ a_Velocity.xy *t+0.5*c_2DGravity*tt;
		newPosition.xy= newPosition.xy+newDir*(t*0.5)*sin(t*c_PI*period)*amp;
	}
	else{
		newPosition.x= 10000000;
	}
	gl_Position= newPosition;
	v_Color=a_Color;
}

void CircleEffect(){
	float t= u_Time-a_StartTime;
	float amp=a_Amp;
	float period=a_Period;
	vec4 newPosition = vec4(a_Position,1);
	//t= a_LifeTime* fract(t/a_LifeTime);
	

	if(t>0){
	t= a_LifeTime* fract(t/a_LifeTime);	 //생존 주기 반복을 위한 필수 식.
		float tt=t*t;
	
		float value= a_Value*2.0*c_PI;
		float a= cos(value);
		float b=sin(value);
		newPosition.xy=newPosition.xy+vec2(a,b);

		vec2 newVel = a_Velocity.xy+c_2DGravity*t;
		vec2 newDir= vec2(-newVel.y,newVel.x);
		newDir=normalize(newDir);
		newPosition.xy= newPosition.xy+ a_Velocity.xy *t+0.5*c_2DGravity*tt;
		newPosition.xy= newPosition.xy+newDir*(t*0.1)*sin(t*c_PI*period)*amp;
	}
	else{
		newPosition.x= 10000000;
	}
	gl_Position= newPosition;
	v_Color=a_Color;
}

void CircleEffectCycle(){
	float t= u_Time-a_StartTime;
	float amp=a_Amp;
	float period=a_Period;
	vec4 newPosition = vec4(a_Position,1);
	//t= a_LifeTime* fract(t/a_LifeTime);
	

	if(t>0){
	t= a_LifeTime* fract(t/a_LifeTime);	 //생존 주기 반복을 위한 필수 식.
		float tt=t*t;
	
		float value= a_StartTime*2.0*c_PI;
		float a= cos(value);
		float b=sin(value);
		newPosition.xy=newPosition.xy+vec2(a,b);

		vec2 newVel = a_Velocity.xy+c_2DGravity*t;
		vec2 newDir= vec2(-newVel.y,newVel.x);
		newDir=normalize(newDir);
		newPosition.xy= newPosition.xy+ a_Velocity.xy *t+0.5*c_2DGravity*tt;
		newPosition.xy= newPosition.xy+newDir*(t*0.1)*sin(t*c_PI*period)*amp;
	}
	else{
		newPosition.x= 10000000;
	}
	gl_Position= newPosition;
v_Color=a_Color;
}


void HeartEffectCycle(){
	float t= u_Time-a_StartTime;
	float amp=a_Amp;
	float period=a_Period;
	vec4 newPosition = vec4(a_Position,1);
	//t= a_LifeTime* fract(t/a_LifeTime);
	
	
	if(t>0){
	t= a_LifeTime* fract(t/a_LifeTime);	 //생존 주기 반복을 위한 필수 식. 0~LifeTime까지 반복
	float particleAlpha= 1-t/a_LifeTime;
		float tt=t*t;
	
		float value= a_StartTime*2.0*c_PI;
		float a= 16*pow(sin(value),3);
		float b=13*cos(value)-5*cos(2*value)-2*cos(3*value)-cos(4*value);
		a*=0.05;
		b*=0.05;
		newPosition.xy=newPosition.xy+vec2(a,b);

		vec2 newVel = a_Velocity.xy+c_2DGravity*t;
		vec2 newDir= vec2(-newVel.y,newVel.x);
		newDir=normalize(newDir);
		newPosition.xy= newPosition.xy+ a_Velocity.xy *t+0.5*c_2DGravity*tt;
		newPosition.xy= newPosition.xy+newDir*(t*0.1)*sin(t*c_PI*period)*amp;
		v_Color=vec4(a_Color.rgb,particleAlpha);
	}
	else{
		newPosition.x= 10000000;
		v_Color=a_Color;
	}
	gl_Position= newPosition;

	
}


void RocketFlare(){ //중력 설정 vec2(0.0,-4.9);
//시작위치를 일정한 좌표로 설정하고, a_Position을 특정좌표
// 이후 vx,vy,vz를 0,0.5,0 으로 하면 위로, 등 방향을 velocity로 정해주고
	/*float startTime =  8*((float)rand() / (float)RAND_MAX);
		centerX = 0; //((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		centerY = 0; //((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		
		vx = 0;
		vy = 0.5;
		vz =0;
	
		lifeTime =  8 * ((float)rand() / (float)RAND_MAX)+1;

		amp = (((float)rand() / (float)RAND_MAX)-0.5f)*2.f;
		period = ((float)rand() / (float)RAND_MAX);*/

	float t= u_Time-a_StartTime;
	
	//t= a_LifeTime* fract(t/a_LifeTime);
	float amp=a_Amp;
	float period=a_Period;

	vec4 newPosition = vec4(a_Position,1);

	if(t>0){
		
		t= a_LifeTime* fract(t/a_LifeTime); //생존 주기 반복을 위한 필수 식.
		vec2 newDir= vec2(-a_Velocity.y,a_Velocity.x);
		newDir=normalize(newDir);
		newPosition.xy= newPosition.xy+ a_Velocity.xy *t;
		newPosition.xy= newPosition.xy+newDir*(t*0.5)*sin(t*c_PI*period)*amp;
	}
	else{
		newPosition.x= 10000000;
	}
	gl_Position= newPosition;
	v_Color=a_Color;
}

void Fountain(){ //중력 설정 vec2(0.0,-4.9);
	//시작위치를 일정한 좌표로 설정하고, a_Position을 특정좌표
	// 이후 vx,vy,vz를 0,0.5,0 으로 하면 위로, 등 방향을 velocity로 정해주고
	//한 점에서 퍼지도록 해야 분수효과

	/*float startTime =  8*((float)rand() / (float)RAND_MAX);
		centerX = 0; //((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		centerY = 0; //((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		
		vx = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		vy =  ((float)rand() / (float)RAND_MAX) * 2.f - 1.f+3.f;
		vz = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
	
		lifeTime =  8 * ((float)rand() / (float)RAND_MAX)+1;

		amp = (((float)rand() / (float)RAND_MAX)-0.5f)*2.f;
		period = ((float)rand() / (float)RAND_MAX);*/

	float t= u_Time-a_StartTime;
	
	//t= a_LifeTime* fract(t/a_LifeTime);
	float amp=a_Amp;
	float period=a_Period;

	vec4 newPosition = vec4(a_Position,1);

	if(t>0){
		
		t= a_LifeTime* fract(t/a_LifeTime); //생존 주기 반복을 위한 필수 식.
		float tt=t*t;
		vec2 newVel= a_Velocity.xy+c_2DGravity*t;
		vec2 newDir= vec2(-a_Velocity.y,a_Velocity.x);
		newDir=normalize(newDir);
		newPosition.xy= newPosition.xy+ a_Velocity.xy *t  + 0.5*c_2DGravity*tt;
		newPosition.xy= newPosition.xy+newDir*(t*0.5)*sin(t*c_PI*period)*amp;
	}
	else{
		newPosition.x= 10000000;
	}
	gl_Position= newPosition;
	v_Color=a_Color;
}

void Basic(){

	vec4 newPosition = vec4(a_Position.xy*a_StartTime,0,1);
	
	gl_Position=newPosition;
	v_Color=a_Color;
}

void Velocity()
{

	float t= u_Time-a_StartTime;

	//t= a_LifeTime* fract(t/a_LifeTime);

	vec4 newPosition = vec4(a_Position,1);

	if(t>0){
		t= a_LifeTime* fract(t/a_LifeTime); //생존 주기 반복을 위한 필수 식.
		float attractValue= fract(t/a_LifeTime);
		float tt=t*t;
		/*
		newPosition.xy= newPosition.xy+ a_Velocity.xy *t+0.5*(c_2DGravity+u_WindAcc)*tt;
		newPosition.xy= mix(newPosition.xy,u_AttractPos,attractValue); //작아지면서 움직임
		*/
		
		vec2 trans= a_Velocity.xy *t+0.5*(c_2DGravity+u_WindAcc)*tt;
		/*trans= mix(newPosition.xy,u_AttractPos,attractValue);
		newPosition.xy= newPosition.xy+trans ;*/
		newPosition.xy=mix(newPosition.xy,u_AttractPos,attractValue);
		
	}
	else{
		newPosition.x= 10000000;
	}
	gl_Position= newPosition;
	v_Color=a_Color;
}

void main()
{
	//Line();
	//Circle();
	//Parabola();
	//Basic();
	//Velocity();
	//SinShape();
	//RocketFlare();
	//Fountain();
	/*
	if(a_StartTime>3.0f)
		CircleEffect();
	else
		Fountain();
	*/
	//Velocity();

		HeartEffectCycle();


		

}
