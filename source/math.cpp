#include "math.h"

template <> template<> vec3t<int>::vec3t(const vec3t<float>& other) {
	x = other.x + 0.5f;
	y = other.y + 0.5f;
	z = other.z + 0.5f;
}

template<> template<> vec3t<float>::vec3t(const vec3t<int>& other) {
	x = other.x;
	y = other.y;
	z = other.z;
}
