#pragma once
#include "RendererHeaders.h"

#include <vector>
#include <iostream>

const int WIDTH  = 1000;
const int HEIGHT = 1000;

const std::vector<vec2> lineVertices = {
	{20.0f, 20.0f},
	{20.0f, 80.0f}, 
	{80.0f, 80.0f}, 
	{80.0f, 20.0f}, 
};

const std::vector<vec2> triVertices = {
	{20.0f, 120.0f},
	{310.0f, 600.0f},
	{600.0f, 20.0f},
	{300.0f, 420.0f},
	{60.0f, 450.0f},
	{900.0f, 420.0f},
};

class Renderer {
private:
	std::vector<Pixel> m_pixels;
public:
	Renderer();
	~Renderer();

	void init();
	void saveFile(const char* name);
	void setPix(int x, int y, Color& p);
	void trySetPix(int x, int y, Color& p);
	void drawLine(vec2 p0, vec2 p1, Color& color);
	void drawTri(vec2 p0, vec2 p1, vec2 p2, Color& color);
private:
	inline int ind(int x, int y) { return y * WIDTH + x; }
};

