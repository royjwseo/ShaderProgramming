#version 330

layout(location=0) out vec4 FragColor;

uniform sampler2D uTexSampler;
in vec2 v_TexPos;


void p1(){
float tx= v_TexPos.x;
	float ty= fract(v_TexPos.y*2);
	vec2 newTexPos= vec2(tx,ty);

	FragColor = texture(uTexSampler, newTexPos);
   if(FragColor.a<0.01)discard;
}

void reflectY(){
	float tx= v_TexPos.x;
	float ty= -abs(v_TexPos.y-0.5)*2+1;
	vec2 newTexPos= vec2(tx,ty);

	FragColor = texture(uTexSampler, newTexPos);
   if(FragColor.a<0.01)discard;
}

void reflectX(){
	float tx= v_TexPos.y;
	float ty= 1-abs(v_TexPos.x-0.5)*2;
	vec2 newTexPos= vec2(tx,ty);

	FragColor = texture(uTexSampler, newTexPos);
   if(FragColor.a<0.01)discard;
}

void REFLECT(){
	
	float tx= v_TexPos.x;
	float ty= v_TexPos.y;
	tx=tx*3;
	ty=(ty+(tx-fract(tx)))/3.0;

	
	//ty= ty/3;
	
	
	vec2 newTexPos= vec2(tx,ty);
	FragColor= texture(uTexSampler,newTexPos);
	
}


void main()
{

	//reflectX();
	reflectY();
	//p1();
   //FragColor=vec4(1,1,1,1);
   //REFLECT();
}
