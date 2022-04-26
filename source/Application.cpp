#include "Application.h"

Application::Application() {
	m_renderer = new Renderer();
}

Application::~Application() {
	delete m_renderer;
}

void Application::run() {
	std::cout << "Save file to:" << std::endl;
	std::string outfileName;
//	std::cin >> std::noskipws >> outfileName;
	if (outfileName == "")
		outfileName = "output.tga";

	m_renderer->init();
	testDrawingTriangle();
	m_renderer->saveFile(outfileName.c_str());

}

void Application::testDrawingLine() {
	const int LINES_TO_DRAW = 1000;
	for (int i = 0; i < LINES_TO_DRAW; i++) {
		Color color = { i / 4, 255, 255 ,255 };
		m_renderer->drawLine( {(float)i, 999.0f }, { 0.0f, 0.0f },color);
	}
}

void Application::testDrawingTriangle() {
	Color col = { 255, 0, 0, 255 };
	m_renderer->drawTri(triVertices[0], triVertices[1], triVertices[2], col);
	col = {0, 255, 0, 255 };
	m_renderer->drawTri(triVertices[3], triVertices[4], triVertices[5], col);
}