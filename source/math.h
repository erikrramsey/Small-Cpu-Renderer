#pragma once
#include <cmath>
#include <array>

template<class T>
struct vec3t {
	T x, y, z;

	vec3t<T>() { x = 0; y = 0; z = 0; }
	vec3t<T>(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
	vec3t<T>(T _v) : x(_v), y(_v), z(_v) {}

	template<class U>
	vec3t<T>(const vec3t<U>& other);

	const T& operator[](const size_t i) const {
		switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		}
	}

	vec3t<T> operator+(const vec3t<T>& rhs) {
		vec3t<T> val(x, y, z);
		val.x += rhs.x;
		val.y += rhs.y;
		val.z += rhs.z;
		return val;
	}

	vec3t<T> operator-(const vec3t<T>& rhs) {
		vec3t<T> val(x, y, z);
		val.x -= rhs.x;
		val.y -= rhs.y;
		val.z -= rhs.z;
		return val;
	}

	vec3t<T> operator*(const float rhs) {
		vec3t<T> val(x, y, z);
		val.x *= rhs;
		val.y *= rhs;
		val.z *= rhs;
		return val;
	}

	vec3t<T> operator/(const float rhs) {
		vec3t<T> val(x, y, z);
		val.x /= rhs;
		val.y /= rhs;
		val.z /= rhs;
		return val;
	}

	void operator/=(const float rhs) {
		x /= rhs;
		y /= rhs;
		z /= rhs;
	}

	float operator*(const vec3t<T>& rhs) {
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}

	bool operator==(const vec3t<T>& rhs) {
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}

	vec3t<T> operator-() {
		return vec3t<T>(-x, -y, -z);
	}

	vec3t<T> cross(const vec3t<T>& rhs) {
		return vec3t<T>(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);
	}

	float norm() { return std::sqrt(x * x + y * y + z * z); }

	vec3t<T>& normalize() {
		*this = (*this) * (1.0 / norm());
		return *this;
	}

};


template<class T>
struct vec2t {
	T x, y;

	vec2t<T>() { x = 0; y = 0; }
	vec2t<T>(T _x, T _y) : x(_x), y(_y) {}
	vec2t<T>(T _v)       : x(_v), y(_v) {}

	const T& operator[](const size_t i) const {
		switch (i) {
		case 0: return x;
		case 1: return y;
		}
	}

	T& operator[](const size_t i) { switch (i) {
		case 0: return x;
		case 1: return y;
		}
	}

	vec2t<T> operator+(const vec2t<T>& rhs) {
		vec2t<T> val;
		for (size_t i = 0; i < 2; i++) val[i] = (*this)[i] + rhs[i];
		return val;
	}

	vec2t<T> operator-(const vec2t<T>& rhs) {
		vec2t<T> val;
		for (size_t i = 0; i < 2; i++) val[i] = (*this)[i] - rhs[i];
		return val;
	}

	vec2t<T> operator*(const float rhs) {
		vec2t<T> val;
		for (size_t i = 0; i < 2; i++) val[i] = (*this)[i] * rhs;
		return val;
	}

	float operator*(const vec2t<T>& rhs) {
		float val = 0;
		for (size_t i = 0; i < 2; i++) val += (*this)[i] * rhs[i];
		return val;
	}

	bool operator==(const vec2t<T>& rhs) {
		bool val = true;
		for (size_t i = 0; i < 2 && val; i++) val = (*this)[i] == rhs[i];
		return val;
	}

	float norm() { return std::sqrt(x * x + y * y); }

	vec2t<T>& normalize() {
		*this = (*this) * (1.0 / norm());
		return *this;
	}

};

typedef vec3t<float> vec3;
typedef vec3t<int> vec3i;
typedef vec2t<float> vec2;
typedef vec2t<int> vec2i;

template<class T, size_t size>
struct mat {
	std::array<float, size*size> contents;

	mat() {
		for (int i = 0; i < size * size; i++)
			contents[i] = 0;
		for (int i = 0; i < size * size; i+= (size + 1))
			contents[i] = 1;
	}

	mat(T val) : mat() {
		*this *= val;
	}
	
	T& at(size_t index) { return contents[index]; }

	mat operator*(float rhs) { for (auto& i : contents) i *= rhs; return *this; }
	mat operator*=(float rhs) { for (auto& i : contents) i *= rhs; return *this; }
	T& operator[](size_t index) { return contents[index]; }

	vec3 operator*(vec3 rhs) {
		vec3 val;
		float w = 1.0f;

		val.x = rhs.x * at(0) + rhs.y * at(1) +  rhs.z * at(2)  + w * at(3);
		val.y = rhs.x * at(4) + rhs.y * at(5) +  rhs.z * at(6)  + w * at(7);
		val.z = rhs.x * at(8) + rhs.y * at(9) +  rhs.z * at(10) + w * at(11);
		w =    rhs.x * at(12) + rhs.y * at(13) + rhs.z * at(14) + w * at(15);
		return (val / w);
	}


	mat<float, 4> operator*(mat<float, 4>& other) {
		mat<float, 4> val;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				val.at(j + i * 4) = rowCol(i * 4, other, j);

		return val;
	}

	static mat<float, 4> rotateY(float angle) {
		mat<float, 4> rotation;
		rotation[0] = std::cos(angle);
		rotation[2] = -std::sin(angle);
		rotation[8] = std::sin(angle);
		rotation[10] = std::cos(angle);
		return rotation;
	}
private:
	float rowCol(int r, mat<float, 4>& m, int c) {
		return at(r) * m.at(c) + at(r + 1) * m.at(c + 4) + at(r + 2) * m.at(c + 8) + at(r + 3) * m.at(c + 12);
	}
};


typedef mat<float, 4> mat4;
