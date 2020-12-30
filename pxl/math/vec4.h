#pragma once

#include "vec3.h"

namespace pxl {
	union vec4 {
		float data[4];
		struct {
			union {
				struct {
					union {
						struct { float x, y; };
						vec2 xy;
					};
					float z;
				};
				vec3 xyz;
			};
			float w;
		};

		inline float dot() const { return x * x + y * y + z * z + w * w; }
		inline float magnitude() const { const auto d = dot(); return d < mathf::epsilon ? 0 : sqrtf(d); }
		inline vec4& normalize() { const auto d = dot(); return d < mathf::epsilon ? *this = 0 : *this *= mathf::rsqrt(d); }
		inline vec4 normalized() const { const auto d = dot(); return d < mathf::epsilon ? zero() : *this * mathf::rsqrt(d); }
		inline vec4& clamp_magnitude(const float& a) { 
			const auto d = dot(); 
			if(a * a < d) 
				return *this *= a * mathf::rsqrt(d) 
			else if(d < mathf::epsilon) 
				return *this = 0; 
			return *this; 
		}
		inline vec4 clamped_magnitude(const float& a) const { const auto d = dot(); return a * a < d ? *this * a * mathf::rsqrt(d) : d < mathf::epsilon ? zero() : a; }
		inline vec4& move_towards(const vec4& o, const float& a) {
			const auto b = o - *this;
			const auto d = b.dot();
			if(d) return a * a < d ? *this += b * mathf::rsqrt(d) * a : *this = o;
			return *this;
		}
		inline vec4 moved_towards(const vec4& o, const float& a) const {
			const auto b = o - *this;
			const auto d = b.dot();
			return d && a * a < d ? *this + b * mathf::rsqrt(d) * a : o;
		}
		// UNARY
		inline vec4 operator+() const { return *this; }
		inline vec4 operator-() const { return (*this) * -1; }
		inline vec4& operator++() { return (*this) += 1; }
		inline vec4& operator--() { return (*this) -= 1; }
		inline vec4 operator++(int) { const auto ret = *this; ++(*this); return ret; }
		inline vec4 operator--(int) { const auto ret = *this; --(*this); return ret; }
		// ARITHMETIC
		inline vec4 operator+(const float& o) const { return { x + o, y + o, z + o, w + o }; }
		inline friend vec4 operator+(const float& l, const vec4& r) { return { l + r.x, l + r.y, l + r.z, l + r.w }; }
		inline vec4 operator+(const vec4& o) const { return { x + o.x, y + o.y, z + o.z, w + o.w }; }
		inline vec4 operator-(const float& o) const { return { x - o, y - o, z - o, w - o }; }
		inline friend vec4 operator-(const float& l, const vec4& r) { return { l - r.x, l - r.y, l - r.z, l - r.w }; }
		inline vec4 operator-(const vec4& o) const { return { x - o.x, y - o.y, z - o.z, w - o.w }; }
		inline vec4 operator*(const float& o) const { return { x * o, y * o, z * o, w * o }; }
		inline friend vec4 operator*(const float& l, const vec4& r) { return { l * r.x, l * r.y, l * r.z, l * r.w }; }
		inline vec4 operator*(const vec4& o) const { return { x * o.x, y * o.y, z * o.z, w * o.w }; }
		inline vec4 operator/(const float& o) const { return { x / o, y / o, z / o, w / o }; }
		inline friend vec4 operator/(const float& l, const vec4& r) { return { l / r.x, l / r.y, l / r.z, l / r.w }; }
		inline vec4 operator/(const vec4& o) const { return { x / o.x, y / o.y, z / o.z, w / o.w }; }
		// ASSIGN
		inline vec4& operator+=(const float& o) { x += o, y += o, z += o, w += o; return *this; }
		inline vec4& operator+=(const vec4& o) { x += o.x, y += o.y, z += o.z, w += o.w; return *this; }
		inline vec4& operator-=(const float& o) { x -= o, y -= o, z -= o, w -= o; return *this; }
		inline vec4& operator-=(const vec4& o) { x -= o.x, y -= o.y, z -= o.z, w -= o.w; return *this; }
		inline vec4& operator*=(const float& o) { x *= o, y *= o, z *= o, w *= o; return *this; }
		inline vec4& operator*=(const vec4& o) { x *= o.x, y *= o.y, z *= o.z, w *= o.w; return *this; }
		inline vec4& operator/=(const float& o) { x /= o, y /= o, z /= o, w /= o; return *this; }
		inline vec4& operator/=(const vec4& o) { x /= o.x, y /= o.y, z /= o.z, w /= o.w; return *this; }
		inline vec4& operator=(const float& o) { x = o, y = o, z = o, w = o; return *this; }
		// COMPARE
		inline bool operator==(const vec4& o) const { return x == o.x && y == o.y && z == o.z && w == o.w; }
		inline bool operator!=(const vec4& o) const { return x != o.x || y != o.y || z != o.z || w != o.w; }
		// SUBSCRIPT
		inline const float& operator[](const size_t& i) const { return data[i]; }
		inline float& operator[](const size_t& i) { return data[i]; }
		// CONSTANTS
		inline static constexpr vec4 zero() { return { 0, 0, 0, 0 }; }
		inline static constexpr vec4 one() { return { 1, 1, 1, 1 }; }
		inline static constexpr vec4 infinity() { return { mathf::infinity, mathf::infinity, mathf::infinity, mathf::infinity }; }
		// STATIC FUNCTIONS
		inline static vec4 min(const vec4& a, const vec4& b) { return { mathf::min(a.x, b.x), mathf::min(a.y, b.y), mathf::min(a.z, b.z), mathf::min(a.w, b.w) }; }
		inline static vec4 max(const vec4& a, const vec4& b) { return { mathf::max(a.x, b.x), mathf::max(a.y, b.y), mathf::max(a.z, b.z), mathf::max(a.w, b.w) }; }
		inline static float dot(const vec4& a, const vec4& b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w + b.w; }
		inline static float distance(const vec4& a, const vec4& b) { return (a - b).magnitude(); }
		inline static vec4 cross(const vec4& a, const vec4& b) { return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x, 1 }; }
		inline static vec4 reflect(const vec4& a, const vec4& b) { return a - b * dot(a, b) * 2; }
		inline static vec4 project(const vec4& a, const vec4& b) { const auto c = b.dot(); return c < mathf::epsilon ? zero() : dot(a, b) * b / c; }
		inline static vec4 lerp(const vec4& x, const vec4& y, const float& a) { return x * (1 - a) + y * a; }
		inline static vec4 clamped_lerp(const vec4& x, const vec4& y, const float& a) { return lerp(x, y, mathf::clamp01(a)); }
	};
}