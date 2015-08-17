#ifndef TEXT_2D_H
#define TEXT_2D_H

#include "Mesh.h"
#include "Vertex.h"

class Text2D
{
public:
	Text2D();
	~Text2D();
	void InitText(const char* fontTexture, const char* fontData);
	void PrintText(const char* text, Color color, float size = 1, float x = 0, float y = 0);
private:
	unsigned m_shaderID;
	unsigned m_uniformTexture;
	unsigned m_uniformTextColor;
	Mesh *m_mesh;
};

#endif