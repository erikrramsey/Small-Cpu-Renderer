#include "Model.h"

Model::Model(std::string path) {
	std::fstream inFile(path);

	if (inFile.fail()) {
		std::cerr << "failed to laod model at path: " << path << std::endl;
		return;
	}

	std::string line;
	while (!inFile.eof()) {
		std::getline(inFile, line);
		std::istringstream current(line.c_str());
		char dummy;
		if (line.compare(0, 2, "v ") == 0) {
			float v1, v2, v3;
			current >> dummy >> v1 >> v2 >> v3;
			vec3 v = { v1, v2, v3 };
			m_vertices.push_back(v);
		} else if (line.compare(0, 2, "vn") == 0) {
			float v1, v2, v3;
			current >> dummy >> dummy >> v1 >> v2 >> v3;
			vec3 v = {v1, v2, v3};
			m_normals.push_back(v);
		} else if (line.compare(0, 2, "f ") == 0) {
			current >> dummy;
			int index;
			do {
				current >> index;
				index--;
				m_indices.push_back(index);
				current >> dummy >> index;
				index--;
				m_uvIndices.push_back(index);
				current >> dummy >> index;
				index--;
				m_nIndices.push_back(index);
			} while (!current.eof());
		} else if (line.compare(0, 2, "vt") == 0) {
			current >> dummy >> dummy;
			float u, v;
			current >> u >> v;
			m_texCoords.push_back(vec2(u, v));
		}
	}
}