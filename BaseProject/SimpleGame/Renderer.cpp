#include "stdafx.h"
#include "Renderer.h"

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
	
	//Create VBOs
	CreateVertexBufferObjects();
	CreateParticleVertexBufferObjects();
	CreateParticleCloud(10000);

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::CreateVertexBufferObjects()
{
	/*glGenVertexArrays(1, &PointRectVAO);
	glBindVertexArray(PointRectVAO);*/


	//���ؽ� ���� ��ü(Vertex Buffer Object, VBO)�� OpenGL���� ���� �����͸� �����ϴ� �� ���Ǵ� �����Դϴ�. 
	//�̴� �׷��� ���ø����̼ǿ��� ���Ǵ� 3D ���� ���� ��ǥ, �ؽ�ó ��ǥ, ���� ���� ���� �����͸� �����ϴ� ���� �̿�˴ϴ�. 
	//VBO�� GPU���� ���� �����͸� ȿ�������� �����Ͽ� ������ ������ ����ŵ�ϴ�.

	//VBO�� ����ϸ� CPU���� GPU�� �����͸� �����ϴ� ������带 �ּ�ȭ�� �� �ֽ��ϴ�.
	//�̴� �����͸� �� ���� ���ε��ϰ� �� ���Ŀ��� GPU �޸𸮿� �����Ǿ� �������� ����� �� �ֱ� �����Դϴ�.
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
	//�ϳ��� VBO �ĺ��ڸ� �����ϰ�, �� �ĺ��ڸ� m_TestVBO�� �����մϴ�.
	glBindBuffer(GL_ARRAY_BUFFER, m_TestVBO);
	//m_TestVBO�� ���� ���ε��� ���۷� �����մϴ�. ������ ���� ���� �۾��� �� VBO�� ���� �̷�����ϴ�.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//���� ���ε��� ���ۿ� �����͸� �����մϴ�.ù ��° �Ű������� ��� ������ ������ �����ϰ�, 
	//�� ��° �Ű������� �������� ũ�⸦ ����Ʈ ������ �����մϴ�.�� ��° �Ű������� ������ �����͸� ����Ű�� �������̸�, 
	//�� ��° �Ű������� �������� ��� ����� �����մϴ�.���⼭�� ���� �����͸� �� ���� ���ε��ϹǷ� GL_STATIC_DRAW�� ����մϴ�.
	
	// Assimp �Լ� : ��� ������ �̷������ �Լ�
	
	float verticesColor[] = { 1.0f,0.f,0.f,1.f,0.f,1.f,0.f,1.f,0.f,0.f,1.f,1.0f };
	
		glGenBuffers(1, &m_ParticleColorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleColorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesColor), verticesColor, GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//glBindVertexArray(0);
}


void Renderer::CreateParticleCloud(int numParticles)
{
	float centerX, centerY;
	centerX = 0.f;
	centerY = 0.f;
	float size = 0.01f;
	int particleCount = numParticles;
	int verticesCount = particleCount * 6;
	int floatCount = verticesCount * (3+1+3+1+1+1+1);
	// x, y, z , startTime, velocity.xyz, lifetime, amp, period, value

	float vx, vy, vz;
	float lifeTime;

	float amp, period;
	float value;

	std::unique_ptr<float[]> vertices = nullptr;
	vertices = std::make_unique<float[]>(floatCount);
	//vertices = std::make_unique<float[]>(new float[floatCount]);
	
	int index = 0;
	for (int i = 0; i < particleCount; i++) {

		float velocityScale = 0.2f;
		float startTime =  8*((float)rand() / (float)RAND_MAX);
		centerX = 0; //((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		centerY = 0; //((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		
		vx = (((float)rand() / (float)RAND_MAX) * 2.f - 1.f)* velocityScale;
		vy = (((float)rand() / (float)RAND_MAX) * 2.f - 1.f) * velocityScale;
		vz = (((float)rand() / (float)RAND_MAX) * 2.f - 1.f) * velocityScale;
	
		lifeTime =  0.5 * ((float)rand() / (float)RAND_MAX)+1;

		amp = (((float)rand() / (float)RAND_MAX)-0.5f)*2.f;
		period = ((float)rand() / (float)RAND_MAX);
		value = ((float)rand() / (float)RAND_MAX);

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

	}

	glGenBuffers(1, &m_ParticleCloudVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, vertices.get(), GL_STATIC_DRAW);

	m_ParticleCloudVertexCount = verticesCount;


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
	//���̴� ������Ʈ ����
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//���̴� �ڵ带 ���̴� ������Ʈ�� �Ҵ�
	glShaderSource(ShaderObj, 1, p, Lengths);

	//�Ҵ�� ���̴� �ڵ带 ������
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj �� ���������� ������ �Ǿ����� Ȯ��
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL �� shader log �����͸� ������
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram �� attach!!
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
	GLuint ShaderProgram = glCreateProgram(); //�� ���̴� ���α׷� ����

	if (ShaderProgram == 0) { //���̴� ���α׷��� ����������� Ȯ��
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs �� vs ������ �ε���
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs �� fs ������ �ε���
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram �� vs.c_str() ���ؽ� ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram �� fs.c_str() �����׸�Ʈ ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach �Ϸ�� shaderProgram �� ��ŷ��
	glLinkProgram(ShaderProgram);

	//��ũ�� �����ߴ��� Ȯ��
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program �α׸� �޾ƿ�
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
	// ���̴� ���α׷����� ���ǵ� "a_Position" attribute�� ��ġ�� �����ɴϴ�. 
	// �� ��ġ�� ���ؽ� �������� �� ��Ұ� � attribute�� �ش��ϴ����� �����մϴ�.
	glEnableVertexAttribArray(attribPosition);
	//���� �ܰ迡�� ������ attribute�� ��ġ�� Ȱ��ȭ�մϴ�. 
	//�̷��� �ϸ� �ش� attribute�� ���� �����Ͱ� ���̴��� ���޵� �� �ֽ��ϴ�.
	glBindBuffer(GL_ARRAY_BUFFER, m_TestVBO);
	//VBO�� ���ε��մϴ�. �̷��� �ϸ� VBO�� �ִ� �����Ͱ� ���� GL_ARRAY_BUFFER�� ���� �� �ֽ��ϴ�.
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	//���� �ܰ迡�� Ȱ��ȭ�� attribute�� ���� ������ �����Ϳ� ������ �����մϴ�. �� ���, 
	//attribute�� 3D ��ġ ������ ��� �����Ƿ� ������ ������ GL_FLOAT�̸�, �� ��Ҵ� 3���� float ������ �����˴ϴ�. 
	//�����ʹ� VBO�� ù ��° ��Һ��� �����ϸ�, �� ������ ������ ũ��� sizeof(float) * 3�Դϴ�.
	
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
	//Program select
	GLuint cur_Shader = m_ParticleCloudShader;
	glUseProgram(cur_Shader);
	GLuint stride = sizeof(float) * 11;

	glUniform1f(glGetUniformLocation(cur_Shader, "u_Time"), m_ParticleTime);
	m_ParticleTime += 0.016;

	glUniform1f(glGetUniformLocation(cur_Shader, "u_Period"), 2.0);


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

	glDrawArrays(GL_TRIANGLES, 0, m_ParticleCloudVertexCount);

	glDisableVertexAttribArray(attribStartTime);


}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}


