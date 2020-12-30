#pragma once

#include "vec4.h"

namespace pxl {
	union quat {
		float data[4];
		struct {
			union {
				struct { float x, y, z; };
				vec3 xyz;
			};
			float w;
		};
		vec4 xyzw;
		inline float dot() const { return xyzw.dot(); }
		inline quat& normalize() {
			const auto d = xyzw.dot();
			if (d < mathf::epsilon) xyz = 0, w = 1;
			else xyzw *= mathf::rsqrt(d);
			return *this;
		}
		inline quat normalized() const {
			const auto d = xyzw.dot();
			if (d < mathf::epsilon) return identity();
			const float& rmag = mathf::rsqrt(d);
			return { x * rmag, y * rmag, z * rmag, w * rmag };
		}
		inline quat& conjugate() { xyz *= -1; return *this; }
		inline quat conjugated() const { return { -x, -y, -z, w }; }
		inline quat& inverse() {
			auto d = xyzw.dot();
			if (d == 1) return conjugate();
			d = -1.0f / d;
			x *= d, y *= d, z *= d, w *= -d;
			return *this;
		}
		inline quat inversed() const {
			auto d = xyzw.dot();
			if (d == 1) return conjugated();
			d = -1.0f / d;
			return { x * d, y * d, z * d, -w * d };
		}
		// UNARY
		inline quat operator+() const { return *this; }
		inline quat operator-() const { return (*this) * -1; }
		inline quat& operator++() { return (*this) += 1; }
		inline quat& operator--() { return (*this) -= 1; }
		inline quat operator++(int) { const auto ret = *this; ++(*this); return ret; }
		inline quat operator--(int) { const auto ret = *this; --(*this); return ret; }
		// ARITHMETIC
		inline quat operator+(const float& o) const { return { x + o, y + o, z + o, w + o }; }
		inline friend quat operator+(const float& l, const quat& r) { return { l + r.x, l + r.y, l + r.z, l + r.w }; }
		inline quat operator+(const quat& o) const { return { x + o.x, y + o.y, z + o.z, w + o.w }; }
		inline quat operator-(const float& o) const { return { x - o, y - o, z - o, w - o }; }
		inline friend quat operator-(const float& l, const quat& r) { return { l - r.x, l - r.y, l - r.z, l - r.w }; }
		inline quat operator-(const quat& o) const { return { x - o.x, y - o.y, z - o.z, w - o.w }; }
		inline quat operator*(const float& o) const { return { x * o, y * o, z * o, w * o }; }
		inline friend quat operator*(const float& l, const quat& r) { return { l * r.x, l * r.y, l * r.z, l * r.w }; }
		inline quat operator*(const quat& o) const {
			return {
				w * o.x + x * o.w + y * o.z - z * o.y,
				w * o.y + y * o.w + z * o.x - x * o.z,
				w * o.z + z * o.w + x * o.y - y * o.x,
				w * o.w - x * o.x - y * o.y - z * o.z };
		}
		inline quat operator/(const float& o) const { return { x / o, y / o, z / o, w / o }; }
		inline friend quat operator/(const float& l, const quat& r) { return { l / r.x, l / r.y, l / r.z, l / r.w }; }
		inline quat operator/(const quat& o) const {
			const auto a = 1.0f / o.xyzw.dot();
			return {
				(-w * o.x + x * o.w - y * o.z + z * o.y) * a,
				(-w * o.y + x * o.z + y * o.w - z * o.x) * a,
				(-w * o.z - x * o.y + y * o.x + z * o.w) * a,
				(w * o.w + x * o.x + y * o.y + z * o.z) * a };
		}
		// ASSIGN
		inline quat& operator+=(const float& o) { x += o, y += o, z += o, w += o; return *this; }
		inline quat& operator+=(const quat& o) { x += o.x, y += o.y, z += o.z, w += o.w; return *this; }
		inline quat& operator-=(const float& o) { x -= o, y -= o, z -= o, w -= o; return *this; }
		inline quat& operator-=(const quat& o) { x -= o.x, y -= o.y, z -= o.z, w -= o.w; return *this; }
		inline quat& operator*=(const float& o) { x *= o, y *= o, z *= o, w *= o; return *this; }
		inline quat& operator*=(const quat& o) { return *this = *this * o; }
		inline quat& operator/=(const float& o) { x /= o, y /= o, z /= o, w /= o; return *this; }
		inline quat& operator/=(const quat& o) { return *this = *this / o; }
		// COMPARE
		inline bool operator==(const quat& o) const { return xyzw == o.xyzw; }
		inline bool operator!=(const quat& o) const { return xyzw != o.xyzw; }
		// SUBSCRIPT
		inline const float& operator[](const size_t& i) const { return data[i]; }
		inline float& operator[](const size_t& i) { return data[i]; }
		// CONSTANTS
		inline static constexpr quat identity() { return { 0, 0, 0, 1 }; }
		// STATIC FUNCTIONS
		inline static quat rotate(vec3 x, vec3 y, const vec3& a = vec3::up()) { // TODO OPTIMIZE
			const auto d = vec3::dot(x.normalize(), y.normalize());
			if (d >= 1 - mathf::epsilon) return identity();
			if (d <= mathf::epsilon - 1) return { a.x, a.y, a.z, 0 };
			quat ret;
			ret.xyz = vec3::cross(x, y);
			ret.w = 1;
			return ret.normalize();
		}
		inline static vec3 to_euler(const quat& q) {
			const float x2 = q.x * q.x, y2 = q.y * q.y, z2 = q.z * q.z, w2 = q.w * q.w,
				u = x2 + y2 + z2 + w2, t = q.x * q.y + q.z * q.w;
			if (t > 0.499f * u)
				return { 0, mathf::halfpi, 2 * atan2f(q.x, q.w) };
			if (t < -0.499f * u)
				return { 0, -mathf::halfpi, -2 * atan2f(q.x, q.w) };
			return { atan2f(2 * q.x * q.w - 2 * q.y * q.z, -x2 + y2 - z2 + w2),
					atan2f(2 * q.y * q.w - 2 * q.x * q.z, x2 - y2 - z2 + w2),
					asinf(2 * t / u) };
		}
		inline static quat from_euler(const vec3& r) {
			const auto hr{ r * 0.5f };
			const float x0 = cosf(hr.x), x1 = sinf(hr.x), y0 = cosf(hr.y),
				y1 = sinf(hr.y), z0 = cosf(hr.z), z1 = sinf(hr.z),
				y0z0 = y0 * z0, y0z1 = y0 * z1, y1z0 = y1 * z0, y1z1 = y1 * z1;
			return {
				y0z0 * x1 + y1z1 * x0,
				y1z0 * x0 + y0z1 * x1,
				y0z1 * x0 - y1z0 * x1,
				y0z0 * x0 - y1z1 * x1 };
		}
		inline static float to_angle_axis(const quat& a, vec3& b) {
			const float d = a.xyz.dot();
			b = d ? a.xyz.normalized() : vec3::up();
			return 2 * atan2f(sqrtf(d), a.w);
		}
		inline static quat from_angle_axis(const float& a, const vec3& b) { const auto ha = a * 0.5f, c = sinf(ha); return { b.x * c, b.y * c, b.z * c, cosf(ha) }; }
		inline static quat lerp(const quat& x, const quat& y, const float& a) { return x * (1 - a) + y * a; }
		inline static quat clamped_lerp(const quat& x, const quat& y, const float& a) { return lerp(x, y, mathf::clamp01(a)); }
		inline static quat slerp(const quat& x, const quat& y, const float& a) {
			const auto d = vec4::dot(x.xyzw, y.xyzw),
				t = acosf(d), s = mathf::rsqrt(1 - d * d),
				sy = sinf(t * a) * s, sx = sinf((1 - a) * t) * s;
			return x * sx + y * sy;
		}
	};
}