#pragma once
#include "tgaimage.h"
#include "math.h"

#include <vector>

struct Color {
	Color(TGAColor color) {
		m_color = color;
	}

	Color() {
		m_color = TGAColor(255, 255, 255, 255);
	}

	Color(int r, int g, int b, int a) {
		m_color = TGAColor(r, g, b, a);
	}

	TGAColor tga() { return m_color; }
	TGAColor m_color;
};

struct Fragment {
	Fragment() {}

	Color color;
	vec3 norm;
	float z = -9999999999;
};

struct Vertex {
	vec3 pos;
	vec3 norm;
};

struct Sampler {
	TGAImage image;

	Sampler(std::string path) {	image.read_tga_file(path); }

	TGAColor at(float u, float v) {
		return image.get(u * (float)image.width(), v * (float)image.height());
	}
};

typedef std::vector<int> IndexBuffer;
typedef std::vector<Vertex> VertexBuffer;
