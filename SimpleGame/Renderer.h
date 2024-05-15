#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	bool IsInitialized();
	void DrawTest();
	void DrawParticle();
	void DrawParticleCloud();
	void DrawFSSandbox();
	void DrawGridMesh();
	void DrawTextureSandbox();

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects();
	void GetGLPosition(float x, float y, float *newX, float *newY);

	void CreateParticleCloud(int numParticles);
	void CreateGridMesh(int xCount, int yCount);
	GLuint CreatePngTexture(char* filePath, GLuint samplingMethod);


	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_TestVBO = 0;
	GLuint m_SolidRectShader = 0;

	GLuint m_ParticleShader = 0;
	GLuint m_ParticleVBO = 0;
	float m_ParticleTime = 0.0f;

	GLuint m_ParticleCloudShader = 0;
	GLuint m_ParticleCloudVertexCount = 0;
	GLuint m_ParticleCloudVBO = 0;

	GLuint m_FSSandboxShader = 0;
	GLuint m_FSSandboxVBO = 0;
	GLfloat m_FSSandboxTime = 0.0f;

	GLuint m_GridMeshShader = 0;
	GLuint m_GridMeshVBO = 0;
	GLuint m_GridMeshVertexCount = 0;
	GLfloat m_GridMeshTime = 0.0f;
	
	GLuint m_TextureSandboxShader = 0;
	GLuint m_TextureSandboxVBO = 0;
	float m_TextureSandboxTime = 0.0f;
	GLuint m_RGBTexture = 0;

	GLuint m_NumberTexture[10];
	GLuint m_numbersTexture;

	GLuint m_TextureShader = 0;
	GLuint m_TextureVBO = 0;


	GLuint m_A_FBO;
	GLuint m_A_FBOTexture0;

};

