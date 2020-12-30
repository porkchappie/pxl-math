#pragma once

#include "quat.h"

namespace pxl {
	union mat3 {
		float data[9];
		vec3 v[3];
		struct { vec3 v0, v1, v2; };
		struct {
			float
				m00, m01, m02,
				m10, m11, m12,
				m20, m21, m22;
		};
		inline mat3& transpose() { 
			const auto t01 = m01, t02 = m02, t12 = m12;
			m01 = m10, m02 = m20, m12 = m21;
			m10 = t01, m20 = t02, m21 = t12;
			return *this; 
		}
		inline mat3 transposed() const {
			return {
				m00, m10, m20,
				m01, m11, m21,
				m02, m12, m22 };
		}
		inline float determinant() const { return m00 * (m22 * m11 - m12 * m21) + m01 * (m12 * m20 - m22 * m10) + m02 * (m21 * m10 - m11 * m20); }
		inline mat3& inverse() { return *this = inversed(); }
		inline mat3 inversed() const {
			const auto dx = m22 * m11 - m12 * m21,
				dy = m12 * m20 - m22 * m10,
				dz = m21 * m10 - m11 * m20,
				d = 1.0f / (m00 * dx + m01 * dy + m02 * dz);
			return {
				dx * d, (m02 * m21 - m22 * m02) * d, (m12 * m02 - m02 * m11) * d,
				dy * d, (m22 * m00 - m02 * m20) * d, (m02 * m10 - m12 * m00) * d,
				dz * d, (m01 * m20 - m21 * m00) * d, (m11 * m00 - m01 * m10) * d };
		}
		inline mat3& adjugated() { return *this = adjugate(); }
		inline mat3 adjugate() const {
			return {
				m11 * m22 - m12 * m21,
				m02 * m21 - m01 * m22,
				m01 * m12 - m02 * m11,
				
				m12 * m20 - m10 * m22,
				m00 * m22 - m02 * m20,
				m02 * m10 - m00 * m12,
				
				m10 * m21 - m11 * m20,
				m01 * m20 - m00 * m21,
				m00 * m11 - m01 * m10 };
		}
		inline auto position() const { return v2.xy; }
		inline auto& position(const vec2& p) {
			v2.xy = p;
			return *this;
		}
		inline auto rotation() const { return atan2f(m01, m00); }
		inline auto& rotation(const float& r) {
			const auto rx = cosf(r), ry = sinf(r);
			const auto s = scale();
			v0.x = rx * s.x;
			v0.y = ry * s.x;
			v1.x = -ry * s.y;
			v1.y = rx * s.y;
			return *this;
		}
		inline vec2 scale() const { return { v0.xy.magnitude(), v1.xy.magnitude() }; }
		inline auto& scale(const vec2& s) {
			const auto r = rotation();
			const auto rx = cosf(r), ry = sinf(r);
			v0.x = rx * s.x;
			v0.y = ry * s.x;
			v1.x = -ry * s.y;
			v1.y = rx * s.y;
			return *this;
		}
		inline void decompose(float& rotation, vec2& scale) {
			const auto sx = v0.xy.dot(),
				sy = v1.xy.dot();
			if (sx) {
				scale.x = sqrtf(sx);
				if (v0.x) {
					const auto rx = v0.x / scale.x;
					rotation = acosf(rx);
					scale.y = v1.y / rx;
				}
				else {
					const auto ry = v0.y * scale.x;
					rotation = asinf(ry);
					scale.y = -v1.x / ry;
				}
			}
			else if (sy) {
				scale.y = sqrtf(sy);
				if (v1.y) {
					const auto rx = v1.y / scale.y;
					rotation = acosf(rx);
					scale.x = v0.x / rx;
				}
				else {
					const auto ry = -v1.x / scale.y;
					rotation = asinf(ry);
					scale.x = v0.y / ry;					
				}
			}
			else {
				rotation = 0;
				scale = 0;
			}
		}
		inline void decompose(vec2& position, float& rotation, vec2& scale) {
			position = v2.xy;
			decompose(rotation, scale);
		}			
		// ARITHMETIC
		inline vec2 operator*(const vec2& o) const {
			return {
				o.x * m00 + o.y * m10 + m20,
				o.x * m01 + o.y * m11 + m21
			};
		}
		inline friend vec2 operator*(const vec2& l, const mat3& r) {
			return {
				l.x * r.m00 + l.y * r.m01 + r.m02,
				l.x * r.m10 + l.y * r.m11 + r.m12
			};
		}
		inline vec3 operator*(const vec3& o) const {
			return {
				o.x * m00 + o.y * m10 + o.z * m20,
				o.x * m01 + o.y * m11 + o.z * m21,
				o.x * m02 + o.y * m12 + o.z * m22
			};
		}
		inline friend vec3 operator*(const vec3& l, const mat3& r) {
			return {
				l.x * r.m00 + l.y * r.m01 + l.z * r.m02,
				l.x * r.m10 + l.y * r.m11 + l.z * r.m12,
				l.x * r.m20 + l.y * r.m21 + l.z * r.m22
			};
		}
		inline mat3 operator*(const mat3& o) const {
			return {
				m00 * o.m00 + m10 * o.m01 + m20 * o.m02,
				m01 * o.m00 + m11 * o.m01 + m21 * o.m02,
				m02 * o.m00 + m12 * o.m01 + m22 * o.m02,
				
				m00 * o.m10 + m10 * o.m11 + m20 * o.m12,
				m01 * o.m10 + m11 * o.m11 + m21 * o.m12,
				m02 * o.m10 + m12 * o.m11 + m22 * o.m12,
				
				m00 * o.m20 + m10 * o.m21 + m20 * o.m22,
				m01 * o.m20 + m11 * o.m21 + m21 * o.m22,
				m02 * o.m20 + m12 * o.m21 + m22 * o.m22 };
		}
		// ASSIGN
		inline friend vec2& operator*=(vec2& l, const mat3& r) { return l = l * r; }
		inline friend vec3& operator*=(vec3& l, const mat3& r) { return l = l * r; }
		inline mat3& operator*=(const mat3& o) { return *this = *this * o; }
		// COMPARE
		inline bool operator==(const mat3& o) const { return v0 == o.v0 && v1 == o.v1 && v2 == o.v2; }
		inline bool operator!=(const mat3& o) const { return v0 != o.v0 || v1 != o.v1 || v2 != o.v2; }
		// SUBSCRIPT
		inline const float& operator[](const size_t& i) const { return data[i]; }
		inline float& operator[](const size_t& i) { return data[i]; }
		// CONSTANTS
		inline static constexpr mat3 zero() { return { 0 }; }
		inline static constexpr mat3 identity() { return { 1, 0, 0, 0, 1, 0, 0, 0, 1 }; }
		// STATIC FUNCTIONS
		inline static mat3 from_translation(const vec2& p) {
			return {
				1, 0, 0,
				0, 1, 0,
				p.x, p.y, 1 };
		}
		inline static mat3 from_rotation(const float& r) {
			const auto rx = cosf(r), ry = sinf(r);
			return {
				rx, ry, 0,
				-ry, rx, 0,
				0, 0, 1 };
		}
		inline static mat3 from_quat(const quat& o) {
			const auto x2 = o.x + o.x, y2 = o.y + o.y, z2 = o.z + o.z;
			const auto xx = o.x * x2, yx = o.y * x2, yy = o.y * y2,
				zx = o.z * x2, zy = o.z * y2, zz = o.z * z2,
				wx = o.w * x2, wy = o.w * y2, wz = o.w * z2;
			return {
				1 - yy - zz, yx + wz, zx - wy,
				yx - wz, 1 - xx - zz, zy + wx,
				zx + wy, zy - wx, 1 - xx - yy };
		}
		inline static mat3 from_scale(const vec2& s) {
			return {
				s.x, 0, 0,
				0, s.y, 0,
				0, 0, 1 };
		}
		inline static mat3 from_trs(const vec2& t, const float& r, const vec2& s = vec2::one()) {
			const auto rx = cosf(r), ry = sinf(r);
			return {
				rx * s.x, ry * s.x, 0,
				-ry * s.y, rx * s.y, 0,
				t.x, t.y, 1 };
		}
	};
}