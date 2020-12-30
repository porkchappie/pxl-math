#pragma once

#include "vec2.h"

namespace pxl {
	union vec3 {
		float data[3];
		struct {
			union {
				struct { float x, y; };
				vec2 xy;
			};
			float z;
		};

		inline float dot() const { return x * x + y * y + z * z; }
		inline float magnitude() const { const auto d = dot(); return d < mathf::epsilon ? 0 : sqrtf(d); }
		inline vec3& normalize() { const auto d = dot(); return d < mathf::epsilon ? *this = 0 : *this *= mathf::rsqrt(d); }
		inline vec3 normalized() const { const auto d = dot(); return d < mathf::epsilon ? zero() : *this * mathf::rsqrt(d); }
		inline vec3& clamp_magnitude(const float& a) { 
			const auto d = dot(); 
			if(a * a < d) 
				return *this *= a * mathf::rsqrt(d) 
			else if(d < mathf::epsilon) 
				return *this = 0; 
			return *this; 
		}
		inline vec3 clamped_magnitude(const float& a) const { const auto d = dot(); return a * a < d ? *this * a * mathf::rsqrt(d) : d < mathf::epsilon ? zero() : a; }
		inline vec3& move_towards(const vec3& o, const float& a) {
			const auto b = o - *this;
			const auto d = b.dot();
			if(d) return a * a < d ? *this += b * mathf::rsqrt(d) * a : *this = o;
			return *this;
		}
		inline vec3 moved_towards(const vec3& o, const float& a) const {
			const auto b = o - *this;
			const auto d = b.dot();
			return d && a * a < d ? *this + b * mathf::rsqrt(d) * a : o;
		}
		// UNARY
		inline vec3 operator+() const { return *this; }
		inline vec3 operator-() const { return (*this) * -1; }
		inline vec3& operator++() { return (*this) += 1; }
		inline vec3& operator--() { return (*this) -= 1; }
		inline vec3 operator++(int) { const auto ret = *this; ++(*this); return ret; }
		inline vec3 operator--(int) { const auto ret = *this; --(*this); return ret; }
		// ARITHMETIC
		inline vec3 operator+(const float& o) const { return { x + o, y + o, z + o }; }
		inline friend vec3 operator+(const float& l, const vec3& r) { return { l + r.x, l + r.y, l + r.z }; }
		inline vec3 operator+(const vec3& o) const { return { x + o.x, y + o.y, z + o.z }; }
		inline vec3 operator-(const float& o) const { return { x - o, y - o, z - o }; }
		inline friend vec3 operator-(const float& l, const vec3& r) { return { l - r.x, l - r.y, l - r.z }; }
		inline vec3 operator-(const vec3& o) const { return { x - o.x, y - o.y, z - o.z }; }
		inline vec3 operator*(const float& o) const { return { x * o, y * o, z * o }; }
		inline friend vec3 operator*(const float& l, const vec3& r) { return { l * r.x, l * r.y, l * r.z }; }
		inline vec3 operator*(const vec3& o) const { return { x * o.x, y * o.y, z * o.z }; }
		inline vec3 operator/(const float& o) const { return { x / o, y / o, z / o }; }
		inline friend vec3 operator/(const float& l, const vec3& r) { return { l / r.x, l / r.y, l / r.z }; }
		inline vec3 operator/(const vec3& o) const { return { x / o.x, y / o.y, z / o.z }; }
		// ASSIGN
		inline vec3& operator+=(const float& o) { x += o, y += o, z += o; return *this; }
		inline vec3& operator+=(const vec3& o) { x += o.x, y += o.y, z += o.z; return *this; }
		inline vec3& operator-=(const float& o) { x -= o, y -= o, z -= o; return *this; }
		inline vec3& operator-=(const vec3& o) { x -= o.x, y -= o.y, z -= o.z; return *this; }
		inline vec3& operator*=(const float& o) { x *= o, y *= o, z *= o; return *this; }
		inline vec3& operator*=(const vec3& o) { x *= o.x, y *= o.y, z *= o.z; return *this; }
		inline vec3& operator/=(const float& o) { x /= o, y /= o, z /= o; return *this; }
		inline vec3& operator/=(const vec3& o) { x /= o.x, y /= o.y, z /= o.z; return *this; }
		inline vec3& operator=(const float& o) { x = o, y = o, z = o; return *this; }
		// COMPARE
		inline bool operator==(const vec3& o) const { return x == o.x && y == o.y && z == o.z; }
		inline bool operator!=(const vec3& o) const { return x != o.x || y != o.y || z != o.z; }
		// SUBSCRIPT
		inline const float& operator[](const size_t& i) const { return data[i]; }
		inline float& operator[](const size_t& i) { return data[i]; }
		// CONSTANTS
		inline static constexpr vec3 zero() { return { 0, 0, 0 }; }
		inline static constexpr vec3 one() { return { 1, 1, 1 }; }
		inline static constexpr vec3 left() { return { -1, 0, 0 }; }
		inline static constexpr vec3 right() { return { 1, 0, 0 }; }
		inline static constexpr vec3 up() { return { 0, 1, 0 }; }
		inline static constexpr vec3 down() { return { 0, -1, 0 }; }
		inline static constexpr vec3 forward() { return { 0, 0, 1 }; }
		inline static constexpr vec3 back() { return { 0, 0, -1 }; }
		inline static constexpr vec3 infinity() { return { mathf::infinity, mathf::infinity, mathf::infinity }; }
		// STATIC FUNCTIONS
		inline static float angle(const vec3& a, const vec3& b) { const auto d = a.dot() * b.dot(); return d < mathf::epsilon ? mathf::halfpi : acosf(dot(a, b) / sqrtf(d)); }
		inline static float signed_angle(const vec3& a, const vec3& b, const vec3& c) { return dot(cross(a, b), c) < 0 ? -angle(a, b) : angle(a, b); }
		inline static vec3 min(const vec3& a, const vec3& b) { return { mathf::min(a.x, b.x), mathf::min(a.y, b.y), mathf::min(a.z, b.z) }; }
		inline static vec3 max(const vec3& a, const vec3& b) { return { mathf::max(a.x, b.x), mathf::max(a.y, b.y), mathf::max(a.z, b.z) }; }
		inline static float dot(const vec3& a, const vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
		inline static float distance(const vec3& a, const vec3& b) { return (a - b).magnitude(); }
		inline static vec3 cross(const vec3& a, const vec3& b) { return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x }; }
		inline static vec3 reflect(const vec3& a, const vec3& b) { return a - b * dot(a, b) * 2; }
		inline static vec3 project(const vec3& a, const vec3& b) { const auto c = b.dot(); return c < mathf::epsilon ? zero() : dot(a, b) * b / c; }
		inline static vec3 project_on_plane(const vec3& a, const vec3& b) { return a - project(a, b); }
		inline static vec3 lerp(const vec3& x, const vec3& y, const float& a) { return x * (1 - a) + y * a; }
		inline static vec3 clamped_lerp(const vec3& x, const vec3& y, const float& a) { return lerp(x, y, mathf::clamp01(a)); }
	};
}