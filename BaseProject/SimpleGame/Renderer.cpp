#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_ParticleShader = CompileShaders("./Shaders/Particle.vs", "./Shaders/Particle.fs");
	m_ParticleCloudShader = CompileShaders("./Shaders/ParticleCloud.vs", "./Shaders/ParticleCloud.fs");
	m_FSSandBoxShader = CompileShaders("./Shaders/FragmentSandBox.vs", "./Shaders/FragmentSandBox.fs");
	m_GridMeshShader = CompileShaders("./Shaders/GridMesh.vs", "./Shaders/GridMesh.fs");
	m_TextureSandBoxShader = CompileShaders("./Shaders/TextureSandBox.vs", "./Shaders/TextureSandBox.fs");
	
	//Create VBOs
	CreateVertexBufferObjects();
	CreateParticleVertexBufferObjects();
	CreateParticleCloud(10000);
	CreateGridMesh(32,32);

	

	m_RGBTexture = CreatePngTexture("./rgb.png", GL_NEAREST);

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}


GLuint Renderer::CreatePngTexture(char* filePath, GLuint samplingMethod)

{

	//Load Png

	std::vector<unsigned char> image;

	unsigned width, height;

	unsigned error = lodepng::decode(image, width, height, filePath);

	if (error != 0)

	{

		std::cout << "PNG image loading failed:" << filePath << std::endl;

		assert(0);

	}



	GLuint temp;

	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,

		GL_UNSIGNED_BYTE, &image[0]);



	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMethod);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMethod);



	return temp;

}

void Renderer::CreateVertexBufferObjects()
{
	/*glGenVertexArrays(1, &PointRectVAO);
	glBindVertexArray(PointRectVAO);*/


	//버텍스 버퍼 객체(Vertex Buffer Object, VBO)는 OpenGL에서 정점 데이터를 저장하는 데 사용되는 버퍼입니다. 
	//이는 그래픽 애플리케이션에서 사용되는 3D 모델의 정점 좌표, 텍스처 좌표, 법선 벡터 등의 데이터를 저장하는 데에 이용됩니다. 
	//VBO는 GPU에게 정점 데이터를 효율적으로 전송하여 렌더링 성능을 향상시킵니다.

	//VBO를 사용하면 CPU에서 GPU로 데이터를 복사하는 오버헤드를 최소화할 수 있습니다.
	//이는 데이터를 한 번만 업로드하고 그 이후에는 GPU 메모리에 보관되어 렌더링에 재사용될 수 있기 때문입니다.
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	float vertices[] = { 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f };

	glGenBuffers(1, &m_TestVBO);
	//하나의 VBO 식별자를 생성하고, 그 식별자를 m_TestVBO에 저장합니다.
	glBindBuffer(GL_ARRAY_BUFFER, m_TestVBO);
	//m_TestVBO를 현재 바인딩된 버퍼로 설정합니다. 이후의 버퍼 관련 작업은 이 VBO에 대해 이루어집니다.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//현재 바인딩된 버퍼에 데이터를 복사합니다.첫 번째 매개변수는 대상 버퍼의 유형을 지정하고, 
	//두 번째 매개변수는 데이터의 크기를 바이트 단위로 지정합니다.세 번째 매개변수는 복사할 데이터를 가리키는 포인터이며, 
	//네 번째 매개변수는 데이터의 사용 방법을 지정합니다.여기서는 정점 데이터를 한 번만 업로드하므로 GL_STATIC_DRAW를 사용합니다.
	
	// Assimp 함수 : 즉시 동작이 이루어지는 함수
	
	float verticesColor[] = { 1.0f,0.f,0.f,1.f,0.f,1.f,0.f,1.f,0.f,0.f,1.f,1.0f };
	
		glGenBuffers(1, &m_ParticleColorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleColorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesColor), verticesColor, GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//glBindVertexArray(0);
	
	float size = 0.5;
	float FSSandBoxVerts[] = { 
	-size,-size,0,
	size,size,0,
	-size,size,0,
	-size,-size,0,
	size,-size,0,
	size,size,0,
	};

	glGenBuffers(1, &m_FSSandBoxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_FSSandBoxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(FSSandBoxVerts), FSSandBoxVerts, GL_STATIC_DRAW);


	//---------------------tex sandbox buffer
	float Boxsize = 0.5;
	float FSTextureSandBoxVerts[] = {
	-Boxsize,-Boxsize,0, 0,1,
	Boxsize,Boxsize,0,1,0,
	-Boxsize,Boxsize,0,0,0,
	-Boxsize,-Boxsize,0,0,1,
	Boxsize,-Boxsize,0,1,1,
	Boxsize,Boxsize,0,1,0
	};

	glGenBuffers(1, &m_TextureSandBoxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureSandBoxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(FSTextureSandBoxVerts), FSTextureSandBoxVerts, GL_STATIC_DRAW);
}


void Renderer::CreateParticleCloud(int numParticles)
{
	float centerX, centerY;
	centerX = 0.f;
	centerY = 0.f;
	float size = 0.005f;
	int particleCount = numParticles;
	int verticesCount = particleCount * 6;
	int floatCount = verticesCount * (3+1+3+1+1+1+1+4);
	// x, y, z , startTime, velocity.xyz, lifetime, amp, period, value. colorR,G,B

	float vx, vy, vz;
	float lifeTime;

	float amp, period;
	float value;
	float colorR, colorG, colorB,colorA;

	std::unique_ptr<float[]> vertices = nullptr;
	vertices = std::make_unique<float[]>(floatCount);
	//vertices = std::make_unique<float[]>(new float[floatCount]);
	
	int index = 0;
	for (int i = 0; i < particleCount; i++) {

		float velocityScale = 0.2f;
		
		//centerX = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		//centerY = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		centerX = 0;
		centerY = 0;

		vx = (((float)rand() / (float)RAND_MAX) * 2.f - 1.f)* velocityScale;
		vy = -0.01f; // (((float)rand() / (float)RAND_MAX) * 2.f - 1.f)* velocityScale;
		vz = (((float)rand() / (float)RAND_MAX) * 2.f - 1.f) * velocityScale;
	
		float startTime = 8 * ((float)rand() / (float)RAND_MAX);
		lifeTime =  1 * ((float)rand() / (float)RAND_MAX)+1;

		amp = (((float)rand() / (float)RAND_MAX)-0.5f)*2.f;
		period = ((float)rand() / (float)RAND_MAX);
		value = ((float)rand() / (float)RAND_MAX);

		colorR = ((float)rand() / (float)RAND_MAX);
		colorG = ((float)rand() / (float)RAND_MAX);
		colorB = ((float)rand() / (float)RAND_MAX);
		colorA = ((float)rand() / (float)RAND_MAX);

		vertices[index] = centerX-size; index++;
		vertices[index] = centerY - size;; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = startTime; index++;
		vertices[index] = vx; index++;
		vertices[index] = vy; index++;
		vertices[index] = vz; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = amp; index++;
		vertices[index] = period; index++;
		vertices[index] = value; index++;
		vertices[index] = colorR; index++;
		vertices[index] = colorG; index++;
		vertices[index] = colorB; index++;
		vertices[index] = colorA; index++;


		vertices[index] = centerX + size; index++;
		vertices[index] = centerY + size;; index++;
		vertices[index] = 0.f; index++;	
		vertices[index] = startTime; index++;
		vertices[index] = vx; index++;
		vertices[index] = vy; index++;
		vertices[index] = vz; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = amp; index++;
		vertices[index] = period; index++;
		vertices[index] = value; index++;
		vertices[index] = colorR; index++;
		vertices[index] = colorG; index++;
		vertices[index] = colorB; index++;
		vertices[index] = colorA; index++;

		vertices[index] = centerX - size;; index++;
		vertices[index] = centerY + size;; index++;
		vertices[index] = 0.f; index++; //tri 1
		vertices[index] = startTime; index++;
		vertices[index] = vx; index++;
		vertices[index] = vy; index++;
		vertices[index] = vz; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = amp; index++;
		vertices[index] = period; index++;
		vertices[index] = value; index++;
		vertices[index] = colorR; index++;
		vertices[index] = colorG; index++;
		vertices[index] = colorB; index++;
		vertices[index] = colorA; index++;

		vertices[index] = centerX - size; index++;
		vertices[index] = centerY - size;; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = startTime; index++;
		vertices[index] = vx; index++;
		vertices[index] = vy; index++;
		vertices[index] = vz; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = amp; index++;
		vertices[index] = period; index++;
		vertices[index] = value; index++;
		vertices[index] = colorR; index++;
		vertices[index] = colorG; index++;
		vertices[index] = colorB; index++;
		vertices[index] = colorA; index++;

		vertices[index] = centerX + size; index++;
		vertices[index] = centerY - size;; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = startTime; index++;
		vertices[index] = vx; index++;
		vertices[index] = vy; index++;
		vertices[index] = vz; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = amp; index++;
		vertices[index] = period; index++;
		vertices[index] = value; index++;
		vertices[index] = colorR; index++;
		vertices[index] = colorG; index++;
		vertices[index] = colorB; index++;
		vertices[index] = colorA; index++;

		vertices[index] = centerX + size;; index++;
		vertices[index] = centerY + size;; index++;
		vertices[index] = 0.f; index++; //tri 2
		vertices[index] = startTime; index++;
		vertices[index] = vx; index++;
		vertices[index] = vy; index++;
		vertices[index] = vz; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = amp; index++;
		vertices[index] = period; index++;
		vertices[index] = value; index++;
		vertices[index] = colorR; index++;
		vertices[index] = colorG; index++;
		vertices[index] = colorB; index++;
		vertices[index] = colorA; index++;

	}

	glGenBuffers(1, &m_ParticleCloudVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, vertices.get(), GL_STATIC_DRAW);

	m_ParticleCloudVertexCount = verticesCount;


}


void Renderer::CreateGridMesh(int x, int y) {
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = x;
	int pointCountY = y;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX * pointCountY * 2];
	float* vertices = new float[(pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3];
	m_GridMeshVertexCount = (pointCountX - 1) * (pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y * pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y * pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	//Make triangles
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
			//Triangle part 1
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
		}
	}

	glGenBuffers(1, &m_GridMeshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_GridMeshVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);

}

void Renderer::CreateParticleVertexBufferObjects()
{
	float size = 0.05;
	float vertices[]=
	{
		-size,-size,0.f, size,size,0.0, -size,size,0.0
		,-size,-size,0.0, size,size,0.0, size,-size,0.0
	};
	//

	glGenBuffers(1, &m_ParticleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	
}





void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done."<<'\n';

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Renderer::DrawTest()
{
	
	//Program select
	glUseProgram(m_SolidRectShader);


	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	// 셰이더 프로그램에서 정의된 "a_Position" attribute의 위치를 가져옵니다. 
	// 이 위치는 버텍스 데이터의 각 요소가 어떤 attribute에 해당하는지를 결정합니다.
	glEnableVertexAttribArray(attribPosition);
	//이전 단계에서 가져온 attribute의 위치를 활성화합니다. 
	//이렇게 하면 해당 attribute에 대한 데이터가 셰이더로 전달될 수 있습니다.
	glBindBuffer(GL_ARRAY_BUFFER, m_TestVBO);
	//VBO를 바인딩합니다. 이렇게 하면 VBO에 있는 데이터가 현재 GL_ARRAY_BUFFER로 사용될 수 있습니다.
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	//이전 단계에서 활성화한 attribute에 대해 데이터 포인터와 형식을 설정합니다. 이 경우, 
	//attribute는 3D 위치 정보를 담고 있으므로 데이터 형식은 GL_FLOAT이며, 각 요소는 3개의 float 값으로 구성됩니다. 
	//데이터는 VBO의 첫 번째 요소부터 시작하며, 각 정점의 데이터 크기는 sizeof(float) * 3입니다.
	
	int attribColor = glGetAttribLocation(m_SolidRectShader, "a_Color");
	glEnableVertexAttribArray(attribColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_ColorVBO);
	glVertexAttribPointer(attribColor, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	


	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(attribColor);




}

void Renderer::DrawParticle()
{
	//Program select
	GLuint cur_Shader = m_ParticleShader;
	glUseProgram(cur_Shader);



	
	glUniform1f(glGetUniformLocation(cur_Shader, "u_Time"), m_ParticleTime);
	m_ParticleTime += 0.016;
	
	glUniform1f(glGetUniformLocation(cur_Shader, "u_Period"), 2.0);


	//glUniform3f(glGetUniformLocation(cur_Shader, "c_Velocity"), 0.001,0,0);


	int attribPosition = glGetAttribLocation(cur_Shader, "a_Position");

	glEnableVertexAttribArray(attribPosition);

	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	
	glVertexAttribPointer(attribPosition,3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);


	glDrawArrays(GL_TRIANGLES, 0, m_ParticleCloudVertexCount);

	glDisableVertexAttribArray(attribPosition);




}

void Renderer::DrawParticleCloud()
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Program select
	GLuint cur_Shader = m_ParticleCloudShader;
	glUseProgram(cur_Shader);
	GLuint stride = sizeof(float) * 15;

	glUniform1f(glGetUniformLocation(cur_Shader, "u_Time"), m_ParticleTime);
	m_ParticleTime += 0.016;

	glUniform1f(glGetUniformLocation(cur_Shader, "u_Period"), 2.0);

	//glUniform2f(glGetUniformLocation(cur_Shader, "u_WindAcc"), sin(m_ParticleTime), 0.0);
	glUniform2f(glGetUniformLocation(cur_Shader, "u_WindAcc"), cos(m_ParticleTime), sin(m_ParticleTime));

	glUniform2f(glGetUniformLocation(cur_Shader, "u_AttractPos"), 0, 0.0);
	//glUniform3f(glGetUniformLocation(cur_Shader, "c_Velocity"), 0.001,0,0);


	int attribPosition = glGetAttribLocation(cur_Shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, stride, 0);



	int attribStartTime = glGetAttribLocation(cur_Shader, "a_StartTime");
	glEnableVertexAttribArray(attribStartTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribStartTime, 1, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float)*3));


	int attribVelocity = glGetAttribLocation(cur_Shader, "a_Velocity");
	glEnableVertexAttribArray(attribVelocity);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribVelocity, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 4));

	int attribLifeTime = glGetAttribLocation(cur_Shader, "a_LifeTime");
	glEnableVertexAttribArray(attribLifeTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribLifeTime, 1, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 7));

	int attribAmp = glGetAttribLocation(cur_Shader, "a_Amp");
	glEnableVertexAttribArray(attribAmp);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribAmp, 1, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 8));

	int attribPeriod = glGetAttribLocation(cur_Shader, "a_Period");
	glEnableVertexAttribArray(attribPeriod);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribPeriod, 1, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) *9));

	int attribValue = glGetAttribLocation(cur_Shader, "a_Value");
	glEnableVertexAttribArray(attribValue);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribValue, 1, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 10));

	int attribColor = glGetAttribLocation(cur_Shader, "a_Color");
	glEnableVertexAttribArray(attribColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float) * 11));



	glDrawArrays(GL_TRIANGLES, 0, m_ParticleCloudVertexCount);

	glDisableVertexAttribArray(attribStartTime);
	glDisable(GL_BLEND);

}

void Renderer::DrawFSSandBox()
{
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Program select
	GLuint cur_Shader = m_FSSandBoxShader;
	glUseProgram(cur_Shader);
	GLuint stride = sizeof(float) * 3;

	glUniform1f(glGetUniformLocation(cur_Shader, "u_Time"), m_FSSandBoxTime);
	m_FSSandBoxTime += 0.016;

	glUniform1f(glGetUniformLocation(cur_Shader, "u_Period"), 2.0);

	

	int attribPosition = glGetAttribLocation(cur_Shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_FSSandBoxVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, stride, 0);



	


	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	//glDisable(GL_BLEND);


}


void Renderer::DrawGridMesh()
{
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Program select
	GLuint cur_Shader = m_GridMeshShader;
	glUseProgram(cur_Shader);
	GLuint stride = sizeof(float) * 3;

	glUniform1f(glGetUniformLocation(cur_Shader, "u_Time"), m_FSGridMeshTime);
	m_FSGridMeshTime += 0.016;

	glUniform1f(glGetUniformLocation(cur_Shader, "u_Period"), 2.0);



	int attribPosition = glGetAttribLocation(cur_Shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_GridMeshVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, stride, 0);

	glDrawArrays(GL_LINE_STRIP, 0, m_GridMeshVertexCount);
	//glDrawArrays(GL_TRIANGLES, 0, m_GridMeshVertexCount);

	glDisableVertexAttribArray(attribPosition);
	//glDisable(GL_BLEND);


}

void Renderer::DrawTextureSandBox()
{
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Program select
	GLuint cur_Shader = m_TextureSandBoxShader;
	glUseProgram(cur_Shader);
	GLuint stride = sizeof(float) * 5;

	glUniform1f(glGetUniformLocation(cur_Shader, "u_Time"), m_TextureSandBoxTime);
	m_TextureSandBoxTime += 0.016;

	glUniform1f(glGetUniformLocation(cur_Shader, "u_Period"), 2.0);



	int attribPosition = glGetAttribLocation(cur_Shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureSandBoxVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, stride, 0);

	int attribUv = glGetAttribLocation(cur_Shader, "a_TexPos");
	glEnableVertexAttribArray(attribUv);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureSandBoxVBO);
	glVertexAttribPointer(attribUv, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(float)*3));

	int uniformTex = glGetUniformLocation(cur_Shader, "uTexSampler");
	glUniform1i(uniformTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_RGBTexture);


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
	//glDrawArrays(GL_TRIANGLES, 0, m_GridMeshVertexCount);

	glDisableVertexAttribArray(attribPosition);
	//glDisable(GL_BLEND);
}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}


