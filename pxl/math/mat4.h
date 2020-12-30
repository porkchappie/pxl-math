#pragma once

#include "quat.h"

//string map[] = {
//		"m00", "m01", "m02", "m03",
//		"m10", "m11", "m12", "m13",
//		"m20", "m21", "m22", "m23",
//		"m30", "m31", "m32", "m33"
//};
//string texts[] = {
//	"m[5] * b5 - m[6] * b4 + m[7] * b3;",
//	"-m[1] * b5 + m[2] * b4 - m[3] * b3;",
//	"m[13] * a5 - m[14] * a4 + m[15] * a3;",
//	"-m[9] * a5 + m[10] * a4 - m[11] * a3;",
//	"-m[4] * b5 + m[6] * b2 - m[7] * b1;",
//	"m[0] * b5 - m[2] * b2 + m[3] * b1;",
//	"-m[12] * a5 + m[14] * a2 - m[15] * a1;",
//	"m[8] * a5 - m[10] * a2 + m[11] * a1;",
//	"m[4] * b4 - m[5] * b2 + m[7] * b0;",
//	"-m[0] * b4 + m[1] * b2 - m[3] * b0;",
//	"m[12] * a4 - m[13] * a2 + m[15] * a0;",
//	"-m[8] * a4 + m[9] * a2 - m[11] * a0;",
//	"-m[4] * b3 + m[5] * b1 - m[6] * b0;",
//	"m[0] * b3 - m[1] * b1 + m[2] * b0;",
//	"-m[12] * a3 + m[13] * a1 - m[14] * a0;",
//	"m[8] * a3 - m[9] * a1 + m[10] * a0;"
//};
//regex e("(.*)m\\[(\\d+)\\](.*)m\\[(\\d+)\\](.*)m\\[(\\d+)\\](.*);");
//for (int i = 0; i < 16; i++) {
//	int a, b, c;
//	sscanf(regex_replace(texts[i], e, "$2.$4.$6", std::regex_constants::format_no_copy).c_str(),
//		"%d.%d.%d", &a, &b, &c);
//	cout << "(" << regex_replace(texts[i], e, "$1", std::regex_constants::format_no_copy) <<
//		map[a] << regex_replace(texts[i], e, "$3", std::regex_constants::format_no_copy) <<
//		map[b] << regex_replace(texts[i], e, "$5", std::regex_constants::format_no_copy) <<
//		map[c] << regex_replace(texts[i], e, "$7", std::regex_constants::format_no_copy) <<
//		") * det," << endl;
//}

namespace pxl {
	union mat4 {
		float data[16];
		vec4 v[4];
		struct { vec4 v0, v1, v2, v3; };
		struct {
			float
				m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33;
		};
		inline mat4& transpose() {
			const auto t01 = m01, t02 = m02, t03 = m03,
				t12 = m12, t13 = m13, t23 = m23;
			m01 = m10, m02 = m20, m03 = m30, m12 = m21, m13 = m31, m23 = m32;
			m10 = t01, m20 = t02, m30 = t03, m21 = t12, m31 = t13, m32 = t23;
			return *this;
		}
		inline mat4 transposed() const {
			return {
				m00, m10, m20, m30,
				m01, m11, m21, m31,
				m02, m12, m22, m32,
				m03, m13, m23, m33 };
		}
		inline float determinant() const {
			const auto
				a0 = m00 * m11 - m01 * m10, a1 = m00 * m12 - m02 * m10, a2 = m00 * m13 - m03 * m10,
				a3 = m01 * m12 - m02 * m11, a4 = m01 * m13 - m03 * m11, a5 = m02 * m13 - m03 * m12,
				b0 = m20 * m31 - m21 * m30, b1 = m20 * m32 - m22 * m30, b2 = m20 * m33 - m23 * m30,
				b3 = m21 * m32 - m22 * m31, b4 = m21 * m33 - m23 * m31, b5 = m22 * m33 - m23 * m32;
			return a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
		}

		inline mat4& inverse() { return *this = inversed(); }
		inline mat4 inversed() const {
			const auto
				a0 = m00 * m11 - m01 * m10, a1 = m00 * m12 - m02 * m10, a2 = m00 * m13 - m03 * m10,
				a3 = m01 * m12 - m02 * m11, a4 = m01 * m13 - m03 * m11, a5 = m02 * m13 - m03 * m12,
				b0 = m20 * m31 - m21 * m30, b1 = m20 * m32 - m22 * m30, b2 = m20 * m33 - m23 * m30,
				b3 = m21 * m32 - m22 * m31, b4 = m21 * m33 - m23 * m31, b5 = m22 * m33 - m23 * m32;
			const auto det = 1.0f / (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);
			return {
				(m11 * b5 - m12 * b4 + m13 * b3) * det,
				(-m01 * b5 + m02 * b4 - m03 * b3) * det,
				(m31 * a5 - m32 * a4 + m33 * a3) * det,
				(-m21 * a5 + m22 * a4 - m23 * a3) * det,
				(-m10 * b5 + m12 * b2 - m13 * b1) * det,
				(m00 * b5 - m02 * b2 + m03 * b1) * det,
				(-m30 * a5 + m32 * a2 - m33 * a1) * det,
				(m20 * a5 - m22 * a2 + m23 * a1) * det,
				(m10 * b4 - m11 * b2 + m13 * b0) * det,
				(-m00 * b4 + m01 * b2 - m03 * b0) * det,
				(m30 * a4 - m31 * a2 + m33 * a0) * det,
				(-m20 * a4 + m21 * a2 - m23 * a0) * det,
				(-m10 * b3 + m11 * b1 - m12 * b0) * det,
				(m00 * b3 - m01 * b1 + m02 * b0) * det,
				(-m30 * a3 + m31 * a1 - m32 * a0) * det,
				(m20 * a3 - m21 * a1 + m22 * a0) * det };
		}

		inline mat4& adjugated() { return *this = adjugate(); }
		inline mat4 adjugate() const {
			const auto a0 = m22 * m33 - m23 * m32, a1 = m12 * m33 - m13 * m32, a2 = m12 * m23 - m13 * m22,
				a3 = m02 * m33 - m03 * m32, a4 = m02 * m23 - m03 * m22, a5 = m02 * m13 - m03 * m12,
				a6 = m21 * m33 - m23 * m31, a7 = m11 * m33 - m13 * m31, a8 = m11 * m23 - m13 * m21,
				a9 = m01 * m33 - m03 * m31, a10 = m01 * m23 - m03 * m21, a11 = m01 * m13 - m03 * m11,
				a12 = m21 * m32 - m22 * m31, a13 = m11 * m32 - m12 * m31, a14 = m11 * m22 - m12 * m21,
				a15 = m01 * m32 - m02 * m31, a16 = m01 * m22 - m02 * m21, a17 = m01 * m12 - m02 * m11;
			return {
				(m11 * a0 - m21 * a1 + m31 * a2),
				-(m01 * a0 - m21 * a3 + m31 * a4),
				(m01 * a1 - m11 * a3 + m31 * a5),
				-(m01 * a2 - m11 * a4 + m21 * a5),
				-(m10 * a0 - m20 * a1 + m30 * a2),
				(m00 * a0 - m20 * a3 + m30 * a4),
				-(m00 * a1 - m10 * a3 + m30 * a5),
				(m00 * a2 - m10 * a4 + m20 * a5),
				(m10 * a6 - m20 * a7 + m30 * a8),
				-(m00 * a6 - m20 * a9 + m30 * a10),
				(m00 * a7 - m10 * a9 + m30 * a11),
				-(m00 * a8 - m10 * a10 + m20 * a11),
				-(m10 * a12 - m20 * a13 + m30 * a14),
				(m00 * a12 - m20 * a15 + m30 * a16),
				-(m00 * a13 - m10 * a15 + m30 * a17),
				(m00 * a14 - m10 * a16 + m20 * a17)
			};
		}
		inline auto position() const { return v3.xyz; }
		inline auto& position(const vec3& p) {
			v2.xyz = p;
			return *this;
		}
		inline quat rotation() const {
			const auto e = m00 + m11 + m22;
			if (e > 0) {
				const auto a = mathf::rsqrt(e + 1.0f) * 0.5f; // S=4*qw
				return { (m12 - m21) * a, (m20 - m02) * a, (m01 - m10) * a, 0.25f / a };
			}
			else if ((m00 > m11) && (m00 > m22)) {
				const auto a = mathf::rsqrt(1.0f + m00 - m11 - m22) * 0.5f; // S=4*qx
				return { 0.25f / a, (m10 + m01) * a, (m20 + m02) * a, (m12 - m21) * a };

			}
			else if (m11 > m22) {
				const auto a = mathf::rsqrt(1.0f + m11 - m00 - m22) * 0.5f; // S=4*qy
				return { (m10 + m01) * a, 0.25f / a, (m21 + m12) * a, (m20 - m02) * a };
			}
			else {
				const auto a = mathf::rsqrt(1.0f + m22 - m00 - m11) * 0.5f; // S=4*qz
				return { (m20 - m02) * a, (m21 + m12) * a, 0.25f / a, (m01 - m10) * a };
			}
		}
		inline auto& rotation(const quat& r) {
			const auto s = scale();
			const float x = r.x, y = r.y, z = r.z, w = r.w, x2 = x * x, y2 = y * y,
				z2 = z * z, xy = x * y, xz = x * z, yz = y * z, wx = w * x,
				wy = w * y, wz = w * z;
			v0.xyz = s.x * vec3{ 1.0f - 2.0f * (y2 + z2), 2.0f * (xy + wz), 2.0f * (xz - wy) };
			v1.xyz = s.y * vec3{ 2.0f * (xy - wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz + wx) };
			v2.xyz = s.z * vec3{ 2.0f * (xz + wy), 2.0f * (yz - wx), 1.0f - 2.0f * (x2 + y2) };
			return *this;
		}
		inline vec3 scale() const { return { v0.xyz.magnitude(), v1.xyz.magnitude(), v2.xyz.magnitude() }; }
		inline auto& scale(const vec3& s) {
			const auto r = rotation();
			const float x = r.x, y = r.y, z = r.z, w = r.w, x2 = x * x, y2 = y * y,
				z2 = z * z, xy = x * y, xz = x * z, yz = y * z, wx = w * x,
				wy = w * y, wz = w * z;
			v0.xyz = s.x * vec3{ 1.0f - 2.0f * (y2 + z2), 2.0f * (xy + wz), 2.0f * (xz - wy) };
			v1.xyz = s.y * vec3{ 2.0f * (xy - wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz + wx) };
			v2.xyz = s.z * vec3{ 2.0f * (xz + wy), 2.0f * (yz - wx), 1.0f - 2.0f * (x2 + y2) };
			return *this;
		}
		// ARITHMETIC
		inline vec2 operator*(const vec2& o) const {
			return {
				o.x * m00 + o.y * m10 + m20 + m30,
				o.x * m01 + o.y * m11 + m21 + m31
			};
		}
		inline friend vec2 operator*(const vec2& l, const mat4& r) {
			return {
				l.x * r.m00 + l.y * r.m01 + r.m02 + r.m03,
				l.x * r.m10 + l.y * r.m11 + r.m12 + r.m13
			};
		}
		inline vec3 operator*(const vec3& o) const {
			return {
				o.x * m00 + o.y * m10 + o.z * m20 + m30,
				o.x * m01 + o.y * m11 + o.z * m21 + m31
			};
		}
		inline friend vec3 operator*(const vec3& l, const mat4& r) {
			return {
				l.x * r.m00 + l.y * r.m01 + l.z * r.m02 + r.m03,
				l.x * r.m10 + l.y * r.m11 + l.z * r.m12 + r.m13
			};
		}
		inline vec4 operator*(const vec4& o) const {
			return {
				o.x * m00 + o.y * m10 + o.z * m20 + o.w * m30,
				o.x * m01 + o.y * m11 + o.z * m21 + o.w * m31
			};
		}
		inline friend vec4 operator*(const vec4& l, const mat4& r) {
			return {
				l.x * r.m00 + l.y * r.m01 + l.z * r.m02 + l.w * r.m03,
				l.x * r.m10 + l.y * r.m11 + l.z * r.m12 + l.w * r.m13
			};
		}
		inline mat4 operator*(const mat4& o) const {
			return {
				m00 * o.m00 + m10 * o.m01 + m20 * o.m02 + m30 * o.m03,
				m01 * o.m00 + m11 * o.m01 + m21 * o.m02 + m31 * o.m03,
				m02 * o.m00 + m12 * o.m01 + m22 * o.m02 + m32 * o.m03,
				m03 * o.m00 + m13 * o.m01 + m23 * o.m02 + m33 * o.m03,

				m00 * o.m10 + m10 * o.m11 + m20 * o.m12 + m30 * o.m13,
				m01 * o.m10 + m11 * o.m11 + m21 * o.m12 + m31 * o.m13,
				m02 * o.m10 + m12 * o.m11 + m22 * o.m12 + m32 * o.m13,
				m03 * o.m10 + m13 * o.m11 + m23 * o.m12 + m33 * o.m13,

				m00 * o.m20 + m10 * o.m21 + m20 * o.m22 + m30 * o.m23,
				m01 * o.m20 + m11 * o.m21 + m21 * o.m22 + m31 * o.m23,
				m02 * o.m20 + m12 * o.m21 + m22 * o.m22 + m32 * o.m23,
				m03 * o.m20 + m13 * o.m21 + m23 * o.m22 + m33 * o.m23,

				m00 * o.m30 + m10 * o.m31 + m20 * o.m32 + m30 * o.m33,
				m01 * o.m30 + m11 * o.m31 + m21 * o.m32 + m31 * o.m33,
				m02 * o.m30 + m12 * o.m31 + m22 * o.m32 + m32 * o.m33,
				m03 * o.m30 + m13 * o.m31 + m23 * o.m32 + m33 * o.m33,
			};
		}
		// ASSIGN
		inline friend vec2& operator*=(vec2& l, const mat4& r) { return l = l * r; }
		inline friend vec3& operator*=(vec3& l, const mat4& r) { return l = l * r; }
		inline friend vec4& operator*=(vec4& l, const mat4& r) { return l = l * r; }
		inline mat4& operator*=(const mat4& o) { return *this = *this * o; }
		// COMPARE
		inline bool operator==(const mat4& o) const { return v0 == o.v0 && v1 == o.v1 && v2 == o.v2 && v3 == o.v3; }
		inline bool operator!=(const mat4& o) const { return v0 != o.v0 || v1 != o.v1 || v2 != o.v2 || v3 != o.v3; }
		// SUBSCRIPT
		inline const float& operator[](const size_t& i) const { return data[i]; }
		inline float& operator[](const size_t& i) { return data[i]; }
		// CONSTANTS
		inline static constexpr mat4 zero() { return { 0 }; }
		inline static constexpr mat4 identity() { return { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }; }
		// STATIC FUNCTIONS
		inline static mat4 from_translation(const vec2& p) {
			return {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				p.x, p.y, 0, 1 };
		}
		inline static mat4 from_rotation(const quat& r) {
			const float x = r.x, y = r.y, z = r.z, w = r.w, x2 = x * x, y2 = y * y,
				z2 = z * z, xy = x * y, xz = x * z, yz = y * z, wx = w * x,
				wy = w * y, wz = w * z;
			return {
				1.0f - 2.0f * (y2 + z2), 2.0f * (xy + wz), 2.0f * (xz - wy), 0,
				2.0f * (xy - wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz + wx), 0,
				2.0f * (xz + wy), 2.0f * (yz - wx), 1.0f - 2.0f * (x2 + y2), 0,
				0, 0, 0, 1 };
		}
		inline static mat4 from_scale(const vec3& s) {
			return {
				s.x, 0, 0, 0,
				0, s.y, 0, 0,
				0, 0, s.z, 0,
				0, 0, 0, 1 };
		}
		inline static mat4 from_trs(const vec3& t, const quat& r, const vec3& s = vec3::one()) {
			const float x = r.x, y = r.y, z = r.z, w = r.w, x2 = x * x, y2 = y * y,
				z2 = z * z, xy = x * y, xz = x * z, yz = y * z, wx = w * x,
				wy = w * y, wz = w * z;
			return {
				(1.0f - 2.0f * (y2 + z2)) * s.x, (2.0f * (xy + wz)) * s.x, (2.0f * (xz - wy)) * s.x, 0,
				(2.0f * (xy - wz)) * s.y, (1.0f - 2.0f * (x2 + z2)) * s.y, (2.0f * (yz + wx)) * s.y, 0,
				(2.0f * (xz + wy)) * s.z, (2.0f * (yz - wx)) * s.z, (1.0f - 2.0f * (x2 + y2)) * s.z, 0,
				t.x, t.y, t.z, 1 };
		}
	};
}