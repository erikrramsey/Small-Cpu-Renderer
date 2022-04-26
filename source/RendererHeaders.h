#pragma once
#include "tgaimage.h"

struct Color {
	Color() {
		m_color = TGAColor(255, 255, 255, 255);
	}

	Color(int r, int g, int b, int a) {
		m_color = TGAColor(r, g, b, a);
	}

	TGAColor tga() { return m_color; }
private:
	TGAColor m_color;
};

struct Pixel {
	Pixel() {}

	Color color;
};

struct vec2 {
	vec2(double _x, double _y) : x(_x), y(_y) {}

	double x;
	double y;

	vec2 operator+(const vec2& rhs) {
		return vec2(x + rhs.x, y + rhs.y);
	}

	vec2 operator-(const vec2& rhs) {
		return vec2(x - rhs.x, y - rhs.y);
	}

	vec2 operator*(const double& rhs) {
		return vec2(x * rhs, y * rhs);
	}
};

struct Vertex {
	vec2 pos;
};
