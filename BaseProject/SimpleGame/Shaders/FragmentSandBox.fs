#version 330

layout(location=0) out vec4 FragColor;
in vec4 v_Color;
uniform float u_Time;

const float c_PI=3.141592;


void FilledCircle(){
	vec4 newColor=vec4(1,1,1,1);
	float r=0.5;
	vec2 center= vec2(0.5,0.5);
	float dis=distance(v_Color.rg,center);
	if(dis<r){
	newColor=vec4(1,1,1,1);		
	}else{
	newColor=vec4(0,0,0,1);	
	}
FragColor=vec4(newColor);

}

void Circle(){
	
vec4 newColor=vec4(1,1,1,1);
	float r=0.5;
	float width=0.05;
	vec2 center= vec2(0.5,0.5);
	float dis=distance(v_Color.rg,center);

	if(dis>r-width&&dis<r){
		newColor=vec4(1,1,1,1);
	}else{
	newColor=vec4(0,0,0,0);
	}
FragColor=newColor;
}

void Circles(){
	float circleCount=10;
	vec2 circleCenter= vec2(0.5,0.5);
	float dist= distance(v_Color.xy,circleCenter);
	float inputs= circleCount*c_PI*4*dist-u_Time  *10.0; //빈칸 채우기 ex.3개 그려라 //circleCount*c_PI*4*dist  (((-u_Time은 밖으로 , +u_Time은 안으로 들어오게 *10은 속도
	float sinValue= pow(sin(inputs),16);
	FragColor=vec4(sinValue);

}

void SinGraph(){

	vec2 newTexPos= vec2(v_Color.r*2*c_PI,v_Color.g*2-1);

float period=2;
float amp=0.5;
	float sinValue=amp*sin(newTexPos.x*period-u_Time); //주기 폭 적용 
	float width=0.01;
	if(sinValue<newTexPos.y+width&&sinValue>newTexPos.y){
		FragColor=vec4(1);
	}else{
		FragColor= vec4(0);
	}
	
}

void FlagEffect(){

	vec2 newTexPos= vec2(v_Color.r*2*c_PI,v_Color.g*2-1);

	float period=3;
	float amp=0.2;
	float speed=5;
	//float sinValue=amp*sin(newTexPos.x*period-u_Time)-amp*sin(-u_Time); //주기 폭 적용 왼쪽 고정을 위해 -amp*sin(-u_Time*speed)
	float sinValue=v_Color.r*(amp)*sin(newTexPos.x*period-u_Time*speed); //주기 폭 적용  깃발 느낌내기 위해 왼쪽은 0에 수렴, 오른쪽은 큰값에 수렴하는 값 곱하는 v_Color.r


	float width=0.5;
	width= width*(1-v_Color.r); //점점 감소하는 값을 만들기 위해 1-vColor.r을 하여 곱함 오른쪽으로 가면 갈수록 더 좁아지는 깃발 만들기 위해
	if(sinValue<newTexPos.y+width&&sinValue>newTexPos.y-width){
		FragColor=vec4(pow((sinValue+1)/2,3));
	}else{
		discard;
		//FragColor= vec4(0);
	}
	
}





void main()
{
	
	//FilledCircle();
	//SinGraph();
	FlagEffect();
	//Circles();
}
