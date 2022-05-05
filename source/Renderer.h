#pragma once
#include "RendererHeaders.h"

#include <vector>
#include <iostream>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <algorithm>

const int WIDTH  = 4000;
const int HEIGHT = 4000;

class Renderer {
private:
	std::vector<std::vector<vec3>> m_vBuffers;
	std::vector<std::vector<vec3>> m_nBuffers;
	std::vector<std::vector<vec2>> m_uvs;
	std::vector<IndexBuffer> m_iBuffers;
	std::vector<IndexBuffer> m_inBuffers;
	std::vector<IndexBuffer> m_uviBuffers;
	std::vector<Fragment> m_frags;
	Sampler* m_sampler;
public:
	Renderer();
	~Renderer();

	void init();
	void submitVBuffer(std::vector<vec3>& vBuffer, std::vector<vec3>& nBuffer, std::vector<vec2>& uvs, IndexBuffer& iBuffer, IndexBuffer& inBuffer, IndexBuffer& uvBuffer);
	void draw();


	void saveFile(const char* name);
	void setPix(int x, int y, Color& p);
	void trySetPix(int x, int y, Color& p);
	void trySetZ(int x, int y, float z);
	float tryGetZ(int x, int y);
	void trySetNorm(int x, int y, vec3& norm);
	void drawLine(vec2i p0, vec2i p1, Color& color);
	void triangle(vec3* pts, vec2* uvs, vec3* norms);
private:
	inline int ind(int x, int y) { return y * WIDTH + x; }

	void vertexPass(std::vector<std::vector<vec3>>& screenSpaceVerts);
	void rasterize(std::vector<std::vector<vec3>>&verts);
	void fragmentPass();

	vec3 cameraPos = { 0.3f, 0.3f, 1.0f };

};

