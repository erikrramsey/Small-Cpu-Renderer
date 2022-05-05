#pragma once

#include "Renderer.h"
#include "RendererHeaders.h"
#include "Model.h"

#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <chrono>

class Application {
public:
	Application();
	~Application();
	void run();

private:
	Renderer* m_renderer;
	std::vector<Model> m_models;
};
