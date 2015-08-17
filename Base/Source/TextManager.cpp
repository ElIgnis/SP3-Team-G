#include "TextManager.h"
#include "shader.hpp"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include <iostream>
#include <fstream>
#include "LoadTGA.h"
#include "Mtx44.h"

Text2D::Text2D()
{
	m_shaderID = 0;
	m_uniformTexture = 0;
	m_mesh = NULL;
}

Text2D::~Text2D()
{
}

void Text2D::InitText(const char* fontTexture, const char* fontData)
{
	if(m_shaderID == 0)
	{
		m_shaderID = LoadShaders( "TextVertexShader.vertexshader", "TextVertexShader.fragmentshader" );
		m_uniformTexture = glGetUniformLocation( m_shaderID, "colorTexture" );
		m_uniformTextColor = glGetUniformLocation( m_shaderID, "textColor" );
	}
	if(m_mesh)
	{
		delete m_mesh;
	}
	unsigned numRow = 16, numCol = 16;
	if(fontData != NULL)
	{

	}
	m_mesh = MeshBuilder::GenerateText( fontTexture, numRow, numCol );
	if(m_mesh)
		m_mesh->textureID = LoadTGA(fontTexture);
}

void Text2D::PrintText(const char* text, Color color, float size, float x, float y)
{
	if(!m_mesh || m_mesh->textureID <= 0)
	{
		std::cout << "No text loaded\n";
		return;
	}
	if(!text)
	{
		std::cout << "No text\n";
	}
	glUseProgram(m_shaderID);
	glUniform3fv(m_uniformTextColor, 1, &color.r);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_mesh->textureID);
	glUniform1i(m_uniformTexture, 0);
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 40, 0, 30, -10, 10);
	for(unsigned i = 0; i < strlen(text); ++i)
	{
		Mtx44 translate;
		translate.SetToTranslation(i * 0.5f, 0, i * 0.001f);
		Mtx44 MVP = ortho * translate;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glEnable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
}