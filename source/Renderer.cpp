#include "Renderer.h"

Renderer::Renderer() = default;

Renderer::~Renderer() = default;

void Renderer::init() {
	m_pixels.resize(WIDTH * HEIGHT);
	for (int y = 0; y < WIDTH; y++) {
		for (int x = 0; x < HEIGHT; x++) {
			auto index = y * WIDTH + x;
			m_pixels[index].color = { 0, 0, 0, 255 };
		}
	}
}

void Renderer::saveFile(const char* name) {
	TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
	for (int y = 0; y < WIDTH; y++) {
		for (int x = 0; x < HEIGHT; x++) {
			auto index = y * WIDTH + x;
			image.set(x, y, m_pixels[index].color.tga());
		}
	}

	image.write_tga_file(name);
}
void Renderer::setPix(int x, int y, Color& c) {
	m_pixels[ind(x, y)].color = c;
}

void Renderer::trySetPix(int x, int y, Color& c) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
		std::cerr << "pixel out of bounds x: " << x << "y: " << y << std::endl;
		return;
	}
	m_pixels[ind(x, y)].color = c;
}

void Renderer::drawLine(vec2 p0, vec2 p1, Color& color) {
	bool slopeOverHalf = std::abs(p0.x - p1.x) < std::abs(p0.y - p1.y);
	if (slopeOverHalf) {
		std::swap(p0.x, p0.y);
		std::swap(p1.x, p1.y);
	}
	if (p0.x > p1.x) {
		std::swap(p0, p1);
	}

	if (slopeOverHalf) {
		for (int x = p0.x; x <= p1.x; x++) {
			double t = (x - p0.x) / (double)(p1.x - p0.x);
			int y = p0.y * (1.0 - t) + (p1.y * (double)t);

			trySetPix(y, x, color);
		}
	}
	else {
		for (int x = p0.x; x <= p1.x; x++) {
			double t = (x - p0.x) / (double)(p1.x - p0.x);
			int y = p0.y * (1.0 - t) + (p1.y * (double)t);

			trySetPix(x, y, color);
		}
	}
}

void Renderer::drawTri(vec2 p0, vec2 p1, vec2 p2, Color& color) {
	if (p0.y > p1.y) std::swap(p0, p1);
	if (p0.y > p2.y) std::swap(p0, p2);
	if (p1.y > p2.y) std::swap(p1, p2);
	Color green = { 0, 255, 0, 255 };
	Color red = { 255, 0, 0, 255 };

	int totalHeight = p2.y - p0.y;
	for (int y = p0.y; y <= p1.y; y++) {
		int segmentHeight = p1.y - p0.y + 1;
		double alpha = (double)(y - p0.y) / (double)totalHeight;
		double beta = (double)(y - p0.y) / (double)segmentHeight;
		vec2 A = p0 + (p2 - p0) * alpha;
		vec2 B = p0 + (p1 - p0) * beta;

		drawLine(A, B, color);
	}

	for (int y = p1.y; y <= p2.y; y++) {
		int segmentHeight = p2.y - p1.y + 1;
		double alpha = (double)(y - p0.y) /(double) totalHeight;
		double beta = (double)(y - p1.y) / (double) segmentHeight;
		vec2 A = p0 + (p2 - p0) * alpha;
		vec2 B = p1 + (p2 - p1) * beta;

		drawLine(A, B, color);
	}

}

