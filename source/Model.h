#pragma once

#include "RendererHeaders.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

class Model {
public:
	Model(std::string path);

	std::vector<vec3>& vertices() { return m_vertices; }
	std::vector<vec3>& normals() { return m_normals; }
	std::vector<vec2>& texCoords() { return m_texCoords; }
	IndexBuffer& indices() { return m_indices; }
	IndexBuffer& nIndices() { return m_nIndices; }
	IndexBuffer& uvIndices() { return m_uvIndices; }
private:
	std::vector<vec3> m_vertices;
	std::vector<vec3> m_normals;
	IndexBuffer m_indices;
	IndexBuffer m_nIndices;
	IndexBuffer m_uvIndices;
	std::vector<vec2> m_texCoords;
};
