#include "stdafx.h"
#include "Renderer.h"
#include "Dependencies\freeglut.h"
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
	m_FSSandboxShader = CompileShaders("./Shaders/FSSandbox.vs", "./Shaders/FSSandbox.fs");
	m_GridMeshShader = CompileShaders("./Shaders/GridMesh.vs", "./Shaders/GridMesh.fs");
	m_TextureSandboxShader = CompileShaders("./Shaders/TextureSandbox.vs", "./Shaders/TextureSandbox.fs");

	//Create VBOs
	CreateVertexBufferObjects();

	//Create Particle Cloud
	CreateParticleCloud(30000);

	//Create Grid Mesh
	CreateGridMesh(32, 32);

	m_RGBTexture = CreatePngTexture("rgb.png", GL_NEAREST);
	m_NumberTexture[0] = CreatePngTexture("./textures/0.png", GL_NEAREST);
	m_NumberTexture[1] = CreatePngTexture("./textures/1.png", GL_NEAREST);
	m_NumberTexture[2] = CreatePngTexture("./textures/2.png", GL_NEAREST);
	m_NumberTexture[3] = CreatePngTexture("./textures/3.png", GL_NEAREST);
	m_NumberTexture[4] = CreatePngTexture("./textures/4.png", GL_NEAREST);
	m_NumberTexture[5] = CreatePngTexture("./textures/5.png", GL_NEAREST);
	m_NumberTexture[6] = CreatePngTexture("./textures/6.png", GL_NEAREST);
	m_NumberTexture[7] = CreatePngTexture("./textures/7.png", GL_NEAREST);
	m_NumberTexture[8] = CreatePngTexture("./textures/8.png", GL_NEAREST);
	m_NumberTexture[9] = CreatePngTexture("./textures/9.png", GL_NEAREST);
	m_numbersTexture = CreatePngTexture("./textures/numbers.png", GL_NEAREST);

	if (m_SolidRectShader > 0 && m_ParticleVBO > 0)
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
	float size = 0.05f;
	float particleVerts[]
		=
	{
		// 삼각형1
		-size, -size, 0.f,
		size, -size, 0.f,
		size, size, 0.f,

		// 삼각형2
		-size, -size, 0.f,
		size, size, 0.f,
		-size, size, 0.f
	};

	glGenBuffers(1, &m_ParticleVBO);	// VBO 1개 생성, VBO의 ID를 m_ParticleVBO에 저장
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);	// VBO를 바인딩(이 데이터는 array 형식이다 라고 알려줌)
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleVerts), particleVerts, GL_STATIC_DRAW);	// Bind된 VBO에 데이터를 복사(GPU)

	// CPU 메모리에 정점 데이터를 저장
	float vertices[]
		=
	{
		0.f, 0.f, 0.f, 
		1.f, 0.f, 0.f, 
		1.f, 1.f, 0.f
	};

	// GPU 메모리에 정점 데이터를 저장
	glGenBuffers(1, &m_TestVBO);	// VBO 생성, VBO의 ID를 m_TestVBO에 저장
	glBindBuffer(GL_ARRAY_BUFFER, m_TestVBO);	// VBO를 바인딩(이 데이터는 array 형식이다 라고 알려줌)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// Bind된 VBO에 데이터를 복사(GPU)

	size = 0.5f;
	float FSsandboxVerts[]
		=
	{
		// 삼각형1
		-size, -size, 0.f,
		size, -size, 0.f,
		size, size, 0.f,

		// 삼각형2
		-size, -size, 0.f,
		size, size, 0.f,
		-size, size, 0.f
	};

	glGenBuffers(1, &m_FSSandboxVBO);	// VBO 1개 생성, VBO의 ID를 m_ParticleVBO에 저장
	glBindBuffer(GL_ARRAY_BUFFER, m_FSSandboxVBO);	// VBO를 바인딩(이 데이터는 array 형식이다 라고 알려줌)
	glBufferData(GL_ARRAY_BUFFER, sizeof(FSsandboxVerts), FSsandboxVerts, GL_STATIC_DRAW);	// Bind된 VBO에 데이터를 복사(GPU)

	size = 0.5f;
	float TextureVerts[]
		=
	{
		// 삼각형1
		-size, -size, 0.f, 0, 1,
		size,  -size, 0.f, 1, 1,
		size,   size, 0.f, 1, 0,

		-size, -size, 0.f, 0, 1,
		size,   size, 0.f, 1, 0,
		-size,  size, 0.f, 0, 0
	};


	glGenBuffers(1, &m_TextureSandboxVBO);	// VBO 1개 생성, VBO의 ID를 m_ParticleVBO에 저장
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureSandboxVBO);	// VBO를 바인딩(이 데이터는 array 형식이다 라고 알려줌)
	glBufferData(GL_ARRAY_BUFFER, sizeof(TextureVerts), TextureVerts, GL_STATIC_DRAW);	// Bind된 VBO에 데이터를 복사(GPU)
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
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}

void Renderer::DrawTest()
{
	//Program select
	glUseProgram(m_SolidRectShader);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_TestVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::DrawParticle()
{
	GLuint shader = m_ParticleShader;
	glUseProgram(shader);

	GLint timeLocation = glGetUniformLocation(shader, "u_Time");
	glUniform1f(timeLocation, m_ParticleTime);
	m_ParticleTime += 0.0064;

	GLint periodLocation = glGetUniformLocation(shader, "u_Period");
	glUniform1f(periodLocation, 1.0f);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::DrawParticleCloud()
{
	glEnable(GL_BLEND);	// 블렌딩 활성화
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// 블렌딩 함수 설정 뒤에 있는 물체의 알파값을 사용, 소스의 알파값을 1에서 뺀 값으로 계산

	GLuint shader = m_ParticleCloudShader;
	glUseProgram(shader);
	GLuint stride = sizeof(float) * 15;

	GLint timeLocation = glGetUniformLocation(shader, "u_Time");
	glUniform1f(timeLocation, m_ParticleTime);
	m_ParticleTime += 0.016;

	GLint periodLocation = glGetUniformLocation(shader, "u_Period");
	glUniform1f(periodLocation, 1.0f);

	GLint accLocation = glGetUniformLocation(shader, "u_Acc");
	glUniform2f(accLocation, cos(m_ParticleTime / 10.f), sin(m_ParticleTime / 10.f));

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, stride, 0);

	int attribVelocity = glGetAttribLocation(shader, "a_Velocity");
	glEnableVertexAttribArray(attribVelocity);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribVelocity, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3));

	int attribStartTime = glGetAttribLocation(shader, "a_StartTime");
	glEnableVertexAttribArray(attribStartTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribStartTime, 1, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 6));

	int attribLifeTime = glGetAttribLocation(shader, "a_LifeTime");
	glEnableVertexAttribArray(attribLifeTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribLifeTime, 1, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 7));

	int attribAmplitude = glGetAttribLocation(shader, "a_Amplitude");
	glEnableVertexAttribArray(attribAmplitude);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribAmplitude, 1, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 8));

	int attribPeriod = glGetAttribLocation(shader, "a_Period");
	glEnableVertexAttribArray(attribPeriod);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribPeriod, 1, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 9));

	int attribValue = glGetAttribLocation(shader, "a_Value");
	glEnableVertexAttribArray(attribValue);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribValue, 1, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 10));

	int attribColor = glGetAttribLocation(shader, "a_Color");
	glEnableVertexAttribArray(attribColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 11));

	glDrawArrays(GL_TRIANGLES, 0, m_ParticleCloudVertexCount);

	glDisableVertexAttribArray(attribPosition);

	glDisable(GL_BLEND);	// 블렌딩 비활성화
}

void Renderer::DrawFSSandbox()
{
	//glEnable(GL_BLEND);	// 블렌딩 활성화
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// 블렌딩 함수 설정 뒤에 있는 물체의 알파값을 사용, 소스의 알파값을 1에서 뺀 값으로 계산

	GLuint shader = m_FSSandboxShader;
	glUseProgram(shader);
	GLuint stride = sizeof(float) * 3;

	GLint timeLocation = glGetUniformLocation(shader, "u_Time");
	glUniform1f(timeLocation, m_FSSandboxTime);
	m_FSSandboxTime += 0.016;

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_FSSandboxVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, stride, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
	//glDisable(GL_BLEND);	// 블렌딩 비활성화
}

void Renderer::DrawGridMesh()
{
	GLuint shader = m_GridMeshShader;
	glUseProgram(shader);
	GLuint stride = sizeof(float) * 3;

	GLint timeLocation = glGetUniformLocation(shader, "u_Time");
	glUniform1f(timeLocation, m_GridMeshTime);
	m_GridMeshTime += 0.016;

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_GridMeshVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, stride, 0);

	glDrawArrays(GL_LINE_STRIP, 0, m_GridMeshVertexCount);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::DrawTextureSandbox()
{

	GLuint shader = m_TextureSandboxShader;
	//Program select
	glUseProgram(shader);
	float stride = sizeof(float) * 5;
	//glViewport(0, 0, 256, 256);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureSandboxVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, stride, 0);


	int attribUv = glGetAttribLocation(shader, "a_Texture");
	glEnableVertexAttribArray(attribUv);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureSandboxVBO);
	glVertexAttribPointer(attribUv, 2, GL_FLOAT, GL_FALSE, stride, (GLuint*)(sizeof(float) * 3));


	int uniformTex = glGetUniformLocation(shader, "uTexSampler");
	glUniform1i(uniformTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_RGBTexture);

	int usampler1 = glGetUniformLocation(shader, "u_NumberTexture");
	int textures[] = { 1,2,3,4,5,6,7,8,9,10 };
	glUniform1iv(usampler1, 10, textures);


	int usampler2 = glGetUniformLocation(shader, "u_NumbersTexture");
	glUniform1i(usampler2, 11);



	for (int i = 0; i < 10; i++) {
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, m_NumberTexture[i]);
	}
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, m_numbersTexture);

	GLuint ul_Time = glGetUniformLocation(shader, "u_Time");
	glUniform1f(ul_Time, m_TextureSandboxTime);
	m_TextureSandboxTime += 0.016;

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

void Renderer::CreateParticleCloud(int numParticles)
{
	float centerX, centerY;
	centerX = 0.f;
	centerY = 0.f;

	float size = 0.005f;
	int particleCount = numParticles;
	int vertexCount = particleCount * 6;
	int floatCount = vertexCount * (3 + 3 + 1 + 1 + 1 + 1 + 1 + 4); // (x, y, z), (vx, vy, vz), startTime, lifeTime, amplitude, period, value, (r, g, b, a)

	float* vertices = nullptr;
	vertices = new float[floatCount];

	float vx, vy, vz;
	float startTime;
	float lifeTime;
	float amplitude, period;
	float value;
	float r, g, b, a;

	int index = 0;
	for (int i = 0; i < particleCount; ++i) {
		float velocityScale = 0.2f;

		/*centerX = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		centerY = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;*/
		centerX = 0.f;
		centerY = 0.f;

		//vx = (((float)rand() / (float)RAND_MAX) * 2.f - 1.f) * velocityScale;
		vx = 0.f;
		vy = (((float)rand() / (float)RAND_MAX) * 2.f - 1.f) * velocityScale - 0.5f;
		vz = (((float)rand() / (float)RAND_MAX) * 2.f - 1.f) * velocityScale;

		startTime = 10.f * ((float)rand() / (float)RAND_MAX);
		lifeTime = 0.2f * ((float)rand() / (float)RAND_MAX) + 1.f;

		amplitude = (((float)rand() / (float)RAND_MAX) - 0.5f) * 2.f;
		period	= ((float)rand() / (float)RAND_MAX);
		value = ((float)rand() / (float)RAND_MAX);

		r = ((float)rand() / (float)RAND_MAX);
		g = ((float)rand() / (float)RAND_MAX);
		b = ((float)rand() / (float)RAND_MAX);
		a = 1.f;

		// Triangle 1
		vertices[index++] = centerX - size;
		vertices[index++] = centerY - size;
		vertices[index++] = 0.f;
		vertices[index++] = vx;
		vertices[index++] = vy;
		vertices[index++] = vz;
		vertices[index++] = startTime;
		vertices[index++] = lifeTime;
		vertices[index++] = amplitude;
		vertices[index++] = period;
		vertices[index++] = value;
		vertices[index++] = r;
		vertices[index++] = g;
		vertices[index++] = b;
		vertices[index++] = a;

		vertices[index++] = centerX + size;
		vertices[index++] = centerY + size;
		vertices[index++] = 0.f;
		vertices[index++] = vx;
		vertices[index++] = vy;
		vertices[index++] = vz;
		vertices[index++] = startTime;
		vertices[index++] = lifeTime;
		vertices[index++] = amplitude;
		vertices[index++] = period;
		vertices[index++] = value;
		vertices[index++] = r;
		vertices[index++] = g;
		vertices[index++] = b;
		vertices[index++] = a;

		vertices[index++] = centerX - size;
		vertices[index++] = centerY + size;
		vertices[index++] = 0.f;
		vertices[index++] = vx;
		vertices[index++] = vy;
		vertices[index++] = vz;
		vertices[index++] = startTime;
		vertices[index++] = lifeTime;
		vertices[index++] = amplitude;
		vertices[index++] = period;
		vertices[index++] = value;
		vertices[index++] = r;
		vertices[index++] = g;
		vertices[index++] = b;
		vertices[index++] = a;

		// Triangle 2
		vertices[index++] = centerX - size;
		vertices[index++] = centerY - size;
		vertices[index++] = 0.f;
		vertices[index++] = vx;
		vertices[index++] = vy;
		vertices[index++] = vz;
		vertices[index++] = startTime;
		vertices[index++] = lifeTime;
		vertices[index++] = amplitude;
		vertices[index++] = period;
		vertices[index++] = value;
		vertices[index++] = r;
		vertices[index++] = g;
		vertices[index++] = b;
		vertices[index++] = a;

		vertices[index++] = centerX + size;
		vertices[index++] = centerY - size;
		vertices[index++] = 0.f;
		vertices[index++] = vx;
		vertices[index++] = vy;
		vertices[index++] = vz;
		vertices[index++] = startTime;
		vertices[index++] = lifeTime;
		vertices[index++] = amplitude;
		vertices[index++] = period;
		vertices[index++] = value;
		vertices[index++] = r;
		vertices[index++] = g;
		vertices[index++] = b;
		vertices[index++] = a;

		vertices[index++] = centerX + size;
		vertices[index++] = centerY + size;
		vertices[index++] = 0.f;
		vertices[index++] = vx;
		vertices[index++] = vy;
		vertices[index++] = vz;
		vertices[index++] = startTime;
		vertices[index++] = lifeTime;
		vertices[index++] = amplitude;
		vertices[index++] = period;
		vertices[index++] = value;
		vertices[index++] = r;
		vertices[index++] = g;
		vertices[index++] = b;
		vertices[index++] = a;
	}

	glGenBuffers(1, &m_ParticleCloudVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, vertices, GL_STATIC_DRAW);
	m_ParticleCloudVertexCount = vertexCount;

	delete[] vertices;
}

void Renderer::CreateGridMesh(int xCount, int yCount)
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = xCount;
	int pointCountY = yCount;

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
GLuint Renderer::CreatePngTexture(char* filePath, GLuint samplingMethod)
{
		//Load Png
		std::vector<unsigned char> image;
		unsigned width, height;
		unsigned error = lodepng::decode(image, width, height, filePath);
		if (error != 0)
		{
			std::cout << "PNG image loading failed:" << filePath << std::endl;
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
