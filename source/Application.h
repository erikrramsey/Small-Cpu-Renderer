#pragma once

#include "Renderer.h"
#include "RendererHeaders.h"

#include <iostream>
#include <string>

class Application {
public:
	Application();
	~Application();
	void run();

	void testDrawingLine();
	void testDrawingTriangle();
private:
	Renderer* m_renderer;
};
