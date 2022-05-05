#include "Application.h"
Application::Application() {
	m_renderer = new Renderer();
	m_models.push_back(Model("assets/test_head.obj"));
}

Application::~Application() {
	delete m_renderer;
}

namespace cr = std::chrono;
void Application::run() {
	m_renderer->init();
	auto start = cr::high_resolution_clock::now();
	m_renderer->submitVBuffer(m_models[0].vertices(), m_models[0].normals(), m_models[0].texCoords(), m_models[0].indices(), m_models[0].nIndices(), m_models[0].uvIndices());
	m_renderer->draw();
	m_renderer->saveFile("output.tga");
	std::cout << "render took " 
		<< cr::duration_cast<cr::milliseconds>(cr::high_resolution_clock::now() - start).count()
		<< " milliseconds" << std::endl;
}
