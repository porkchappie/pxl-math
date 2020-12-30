#pragma once

#include "utils.h"

namespace pxl {
	union vec2 {
		float data[2];
		struct { float x, y; };

		inline float dot() const { return x * x + y * y; }
		inline float magnitude() const { const auto d = dot(); return d < mathf::epsilon ? 0 : sqrtf(d); }
		inline vec2& normalize() { const auto d = dot(); return d < mathf::epsilon ? *this = 0 : *this *= mathf::rsqrt(d); }
		inline vec2 normalized() const { const auto d = dot(); return d < mathf::epsilon ? zero() : *this * mathf::rsqrt(d); }
		inline vec2& clamp_magnitude(const float& a) { 
			const auto d = dot(); 
			if(a * a < d) 
				return *this *= a * mathf::rsqrt(d) 
			else if(d < mathf::epsilon) 
				return *this = 0; 
			return *this; 
		}
		inline vec2 clamped_magnitude(const float& a) const { const auto d = dot(); return a * a < d ? *this * a * mathf::rsqrt(d) : d < mathf::epsilon ? zero() : a; }
		inline vec2& move_towards(const vec2& o, const float& a) {
			const auto b = o - *this;
			const auto d = b.dot();
			if(d) return a * a < d ? *this += b * mathf::rsqrt(d) * a : *this = o;
			return *this;
		}
		inline vec2 moved_towards(const vec2& o, const float& a) const {
			const auto b = o - *this;
			const auto d = b.dot();
			return d && a * a < d ? *this + b * mathf::rsqrt(d) * a : o;
		}
		// UNARY
		inline vec2 operator+() const { return *this; }
		inline vec2 operator-() const { return (*this) * -1; }
		inline vec2& operator++() { return (*this) += 1; }
		inline vec2& operator--() { return (*this) -= 1; }
		inline vec2 operator++(int) { const auto ret = *this; ++(*this); return ret; }
		inline vec2 operator--(int) { const auto ret = *this; --(*this); return ret; }
		// ARITHMETIC
		inline vec2 operator+(const float& o) const { return { x + o, y + o }; }
		inline friend vec2 operator+(const float& l, const vec2& r) { return { l + r.x, l + r.y }; }
		inline vec2 operator+(const vec2& o) const { return { x + o.x, y + o.y }; }
		inline vec2 operator-(const float& o) const { return { x - o, y - o }; }
		inline friend vec2 operator-(const float& l, const vec2& r) { return { l - r.x, l - r.y }; }
		inline vec2 operator-(const vec2& o) const { return { x - o.x, y - o.y }; }
		inline vec2 operator*(const float& o) const { return { x * o, y * o }; }
		inline friend vec2 operator*(const float& l, const vec2& r) { return { l * r.x, l * r.y }; }
		inline vec2 operator*(const vec2& o) const { return { x * o.x, y * o.y }; }
		inline vec2 operator/(const float& o) const { return { x / o, y / o }; }
		inline friend vec2 operator/(const float& l, const vec2& r) { return { l / r.x, l / r.y }; }
		inline vec2 operator/(const vec2& o) const { return { x / o.x, y / o.y }; }
		// ASSIGN
		inline vec2& operator+=(const float& o) { x += o, y += o; return *this; }
		inline vec2& operator+=(const vec2& o) { x += o.x, y += o.y; return *this; }
		inline vec2& operator-=(const float& o) { x -= o, y -= o; return *this; }
		inline vec2& operator-=(const vec2& o) { x -= o.x, y -= o.y; return *this; }
		inline vec2& operator*=(const float& o) { x *= o, y *= o; return *this; }
		inline vec2& operator*=(const vec2& o) { x *= o.x, y *= o.y; return *this; }
		inline vec2& operator/=(const float& o) { x /= o, y /= o; return *this; }
		inline vec2& operator/=(const vec2& o) { x /= o.x, y /= o.y; return *this; }
		inline vec2& operator=(const float& o) { x = o, y = o; return *this; }
		// COMPARE
		inline bool operator==(const vec2& o) const { return x == o.x && y == o.y; }
		inline bool operator!=(const vec2& o) const { return x != o.x || y != o.y; }
		// SUBSCRIPT
		inline const float& operator[](const size_t& i) const { return data[i]; }
		inline float& operator[](const size_t& i) { return data[i]; }
		// CONSTANTS
		inline static constexpr vec2 zero() { return { 0, 0 }; }
		inline static constexpr vec2 one() { return { 1, 1 }; }
		inline static constexpr vec2 left() { return { -1, 0 }; }
		inline static constexpr vec2 right() { return { 1, 0 }; }
		inline static constexpr vec2 up() { return { 0, 1 }; }
		inline static constexpr vec2 down() { return { 0, -1 }; }
		inline static constexpr vec2 infinity() { return { mathf::infinity, mathf::infinity }; }
		// STATIC FUNCTIONS
		inline static float angle(const vec2& a, const vec2& b) { const auto d = a.dot() * b.dot(); return d < mathf::epsilon ? mathf::halfpi : acosf(dot(a, b) / sqrtf(d)); }
		inline static float signed_angle(const vec2& a, const vec2& b) { return cross(a, b) < 0 ? -angle(a, b) : angle(a, b); }
		inline static vec2 min(const vec2& a, const vec2& b) { return { mathf::min(a.x, b.x), mathf::min(a.y, b.y) }; }
		inline static vec2 max(const vec2& a, const vec2& b) { return { mathf::max(a.x, b.x), mathf::max(a.y, b.y) }; }
		inline static float dot(const vec2& a, const vec2& b) { return a.x * b.x + a.y * b.y; }
		inline static float cross(const vec2& a, const vec2& b) { return a.x * b.y - a.y * b.x; }
		inline static float distance(const vec2& a, const vec2& b) { return (a - b).magnitude(); }
		inline static vec2 reflect(const vec2& a, const vec2& b) { return a - b * dot(a, b) * 2; }
		inline static vec2 lerp(const vec2& x, const vec2& y, const float& a) { return x * (1 - a) + y * a; }
		inline static vec2 clamped_lerp(const vec2& x, const vec2& y, const float& a) { return lerp(x, y, mathf::clamp01(a)); }
	};
}