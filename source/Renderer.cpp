#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc//matrix_transform.hpp"
Renderer::Renderer() = default;

Renderer::~Renderer() = default;

void Renderer::init() {
	m_frags.resize(WIDTH * HEIGHT);
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			auto index = y * WIDTH + x;
			m_frags[index].color = { 0, 0, 0, 255 };
		}
	}

	m_sampler = new Sampler("assets/test_head_diffuse.tga");
}

void Renderer::saveFile(const char* name) {
	TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			auto index = y * WIDTH + x;
			image.set(x, y, m_frags[index].color.tga());
		}
	}

	image.write_tga_file(name);
}
void Renderer::setPix(int x, int y, Color& c) {
	m_frags[ind(x, y)].color = c;
}

void Renderer::trySetPix(int x, int y, Color& c) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
		return;
	}
	m_frags[ind(x, y)].color = c;
}

void Renderer::trySetZ(int x, int y, float z) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
		return;
	}
	m_frags[ind(x, y)].z = z;
}

float Renderer::tryGetZ(int x, int y) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
		return 0;
	}
	return m_frags[ind(x, y)].z;
}

void Renderer::trySetNorm(int x, int y, vec3& norm) {
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
		return;
	}
	m_frags[ind(x, y)].norm = norm;
}

void Renderer::drawLine(vec2i p0, vec2i p1, Color& color) {
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
			float t = (x - p0.x) / (float)(p1.x - p0.x);
			int y = p0.y * (1.0 - t) + (p1.y * (double)t);

			trySetPix(x, y, color);
		}
	}
}

vec3 barycentric(vec2 A, vec2 B, vec2 C, vec2i P) {
	vec3 s[2];
	for (int i = 2; i--; ) {
		s[i].x = C[i] - A[i];
		s[i].y = B[i] - A[i];
		s[i].z = A[i] - P[i];
	}
	vec3 u = s[0].cross(s[1]);
	if (std::abs(u[2]) > 1e-2)
		return vec3(1.0f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	return vec3(-1, 1, 1);
}

float interp(float y1, float y2, float ity1, float ity2, float p) {
	auto v = (y2 - p) / (y2 - y1);
	auto ity = ity2 - (v * (ity2 - ity1));
	return ity;
}

void Renderer::triangle(vec3* pts, vec2* uvs, vec3* norms) {
	vec2 bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	vec2 bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	vec3 lightDir = { 3.0f, 0.5, 0.5 };
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			bboxmin[j] = std::min(bboxmin[j], pts[i][j]);
			bboxmax[j] = std::max(bboxmax[j], pts[i][j]);
		}
	}
	vec2i P;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) {
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) {
			vec3 c = barycentric({ pts[0].x, pts[0].y }, { pts[1].x, pts[1].y }, { pts[2].x, pts[2].y }, P);
			float z = pts[0].z * c.x + pts[1].z * c.y + pts[2].z * c.z;
			if (c.x < 0 || c.y < 0 || c.z<0 || tryGetZ(P.x, P.y) > z) continue;

			auto uv = uvs[0] * c.x + uvs[1] * c.y + uvs[2] * c.z;

			auto normal = (norms[0] * c.x) + (norms[1] * c.y) + (norms[2] * c.z);
			auto diffuse = std::max((lightDir.normalize() * normal.normalize()), 0.0f);

			auto& reflect = (lightDir - (normal * 2 * (lightDir * normal))).normalize();
			float spec = std::pow(std::max(reflect * -cameraPos.normalize(), 0.0f), 8.0f);

			auto ambient = 0.3f;
			auto intensity = spec + diffuse + ambient;

			Color col(m_sampler->at(1 - uv.x, 1 - uv.y));
			auto bgra = col.m_color.bgra;
			int r = (int)std::min(bgra[2] * intensity, 255.0f);
			int g = (int)std::min(bgra[1] * intensity, 255.0f);
			int b = (int)std::min(bgra[0] * intensity, 255.0f);
			Color color = { r, g, b, bgra[3] };
			trySetPix(P.x, P.y, color);
			trySetZ(P.x, P.y, z);
		}
	}
}

void Renderer::submitVBuffer(std::vector<vec3>& vBuffer, std::vector<vec3>& nBuffer, std::vector<vec2>& uvs, IndexBuffer& iBuffer, IndexBuffer& inBuffer, IndexBuffer& uvBuffer) {
	m_vBuffers.push_back(vBuffer);
	m_nBuffers.push_back(nBuffer);
	m_uvs.push_back(uvs);
	m_iBuffers.push_back(iBuffer);
	m_inBuffers.push_back(inBuffer);
	m_uviBuffers.push_back(uvBuffer);
}

void Renderer::draw() {
	std::vector<std::vector<vec3>> screenSpaceVerts;
	vertexPass(screenSpaceVerts);
	rasterize(screenSpaceVerts);
	fragmentPass();
}

mat4 lookat(vec3 eye, vec3 center, vec3 up) {
	vec3 z = (eye - center).normalize();
	vec3 x = up.cross(z).normalize();
	vec3 y = z.cross(x).normalize();
	mat4 minv(1);

	minv[0] = x[0];
	minv[1] = x[1];
	minv[2] = x[2];
	minv[3] = -(x * eye);
	minv[4] = y[0];
	minv[5] = y[1];
	minv[6] = y[2];
	minv[7] = -(y * eye);
	minv[8] = z[0];
	minv[9] = z[1];
	minv[10] = z[2];
	minv[11] = -(z * eye);

	return minv;
}

void Renderer::vertexPass(std::vector<std::vector<vec3>>& screenSpaceVerts) {
	mat4 model;

	for (int i = 0; i < m_vBuffers.size(); i++) {
		auto& verts = m_vBuffers[i];
		std::vector<vec3> sverts;
		mat<float, 4> proj(1);
		mat<float, 4> view(1);
		view[0] = WIDTH / 2;
		view[5] = HEIGHT / 2;
		view[3] = WIDTH / 2;
		view[7] = HEIGHT / 2;
		auto dist = -1.0f / 5.0f;

		proj[14] = dist;

		mat4 modelView = lookat(cameraPos, { 0.0, 0, 0.0f }, { 0, 1, 0 });
		for (int j = 0; j < verts.size(); j++) {
			vec3 v = view * proj * modelView * model * verts[j];
			sverts.push_back(v);
		}
		screenSpaceVerts.push_back(sverts);
	}
}

void Renderer::rasterize(std::vector<std::vector<vec3>>& verts) {
	for (int i = 0; i < m_iBuffers.size(); i++) {
		auto& vert = verts[i];
		auto& inds = m_iBuffers[i];
		auto& ninds = m_inBuffers[i];
		auto& wVerts = m_vBuffers[i];
		auto& norms = m_nBuffers[i];
		auto& uvis = m_uviBuffers[i];
		auto& uvs = m_uvs[i];
		for (size_t j = 2; j < inds.size(); j += 3) {
			vec3 norm[3] = {
				norms[ninds[j]],
				norms[ninds[j - 1]],
				norms[ninds[j - 2]],
			};
			vec3 pts[3] = {vert[inds[j]], vert[inds[j - 1]], vert[inds[j - 2]] };

			vec2 uvs3[3] = { uvs[uvis[j]], uvs[uvis[j - 1]], uvs[uvis[j - 2]] };
			triangle(pts, uvs3, norm);
		}
	}
}

void Renderer::fragmentPass() {
	/*
	vec3 lightDir = { 0, 0, -1 };

	for (auto& frag : m_frags) {
		float light = frag.norm * lightDir;
	

		vec3 zero = { 0.0f, 0.0f, 0.0f };
		if (frag.drawnTo == true && frag.norm == zero) {
			std::cerr << "no normal supplied for drawn fragment" << std::endl;
		}
		if (light > 0)
			frag.color = { (int)(light * 255), (int)(light * 255) , (int)(light * 255), 255 };
	}
	*/
}
