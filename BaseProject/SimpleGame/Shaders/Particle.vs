#version 330

in vec3 a_Position;

const vec3 c_StartPos=vec3(-1,0,0);
const vec3 c_Velocity=vec3(2.0,0,0);
const vec3 c_ParaVelocity=vec3(2.0,2.0,0);
const vec2 c_2DGravity=vec2(0.0,-4.9);
const float c_PI=3.141592;

uniform float u_Time=0;
uniform float u_Period=2.0;

void Line(){
	float newTime= abs(fract(u_Time/u_Period)-0.5)*2.0;

	vec4 newPosition;
	//newPosition=vec4(a_Position+c_StartPos+ c_Velocity*u_Time,1);
	
	newPosition.xyz=(c_StartPos+a_Position)+c_Velocity*newTime;
	newPosition.w=1;



	gl_Position = newPosition;
}

void Circle(){
	
	float newTime=fract(u_Time/u_Period)*c_PI*2.0;
	vec2 trans = vec2(cos(newTime),sin(newTime));
	vec4 newPosition;
	newPosition.xy=a_Position.xy+trans;
	newPosition.zw=vec2(0,1);
	gl_Position=newPosition;

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
}

void Triangle() {
    // 시간을 주기로 나누어 제한합니다.
    float newTime = fract(u_Time / u_Period);
    float t = newTime;
    float tt = t * t;

    vec4 newPosition;
    

        // 삼각형의 각 꼭지점에 대해 파라볼라 운동을 적용합니다.
        float transX = (a_Position.x + c_StartPos.x) + c_ParaVelocity.x * newTime +
                       0.5 * c_2DGravity.x * tt;
        float transY = (a_Position.y + c_StartPos.y) + c_ParaVelocity.y * newTime +
                       0.5 * c_2DGravity.y * tt;

        // 새로운 위치를 저장합니다.
        newPosition.xy = vec2(transX, transY);
        newPosition.zw = vec2(0, 1);
        
        // 삼각형의 각 정점의 새로운 위치를 출력합니다.
        gl_Position = newPosition;
    
}



void Basic(){

vec4 newPosition = vec4(a_Position,1);
gl_Position=newPosition;

}

void main()
{
	//Line();
	//Circle();
	//Parabola();
	//Basic();
	Triangle();
}
