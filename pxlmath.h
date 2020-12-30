#pragma once

namespace pxl {
#define PXL_MATH_VEC_IMPL(X) PXL_MATH_VEC_OPS(X) PXL_MATH_VEC_COMP(X) PXL_MATH_VEC_SUBSCRIPT
#define PXL_MATH_VEC_SUBSCRIPT \
	inline const float& operator[](const size_t& i) const { return data[i]; } \
	inline float& operator[](const size_t& i) { return data[i]; }
#define PXL_MATH_VEC_OPS(X) \
	PXL_MATH_VEC_OP(X, +) \
	PXL_MATH_VEC_OP(X, -) \
	PXL_MATH_VEC_OP(X, *) \
	PXL_MATH_VEC_OP(X, /) \
	inline vec##X& operator=(const float& o) { PXL_MATH_VEC##X##_OP_FLOAT_IMPL(=); return *this; }
#define PXL_MATH_VEC_OP(X, OP) \
	inline vec##X operatorOP(const float& o) const { return {PXL_MATH_VEC##X##_OP_FLOAT_IMPL(OP)}; } \
	inline friend vec##X operatorOP(const float& o, const vec##X& v) { return {PXL_MATH_VEC##X##_OP_FLOAT2_IMPL(OP)}; } \
	inline vec##X operatorOP(const vec##X& o) const { return {PXL_MATH_VEC##X##_OP_IMPL(OP)}; } \
	inline vec##X& operatorOP=(const float& o) { PXL_MATH_VEC##X##_OP_FLOAT_IMPL(OP=); return *this; } \
	inline vec##X& operatorOP=(const vec##X& o) { PXL_MATH_VEC##X##_OP_IMPL(OP=); return *this; }
#define PXL_MATH_VEC_COMP(X) \
	inline bool operator==(const vec##X& o) const { return PXL_MATH_VEC## X ##_COMP_IMPL(==, &&); } \
	inline bool operator!=(const vec##X& o) const { return PXL_MATH_VEC## X ##_COMP_IMPL(!=, ||); }
#define PXL_MATH_VEC2_OP_FLOAT_IMPL(X) x X o, y X o
#define PXL_MATH_VEC3_OP_FLOAT_IMPL(X) PXL_MATH_VEC2_OP_FLOAT_IMPL(X), z X o
#define PXL_MATH_VEC4_OP_FLOAT_IMPL(X) PXL_MATH_VEC3_OP_FLOAT_IMPL(X), w X o
#define PXL_MATH_VEC2_OP_FLOAT2_IMPL(X) v.x X o, v.y X o
#define PXL_MATH_VEC3_OP_FLOAT2_IMPL(X) PXL_MATH_VEC2_OP_FLOAT2_IMPL(X), v.z X o
#define PXL_MATH_VEC4_OP_FLOAT2_IMPL(X) PXL_MATH_VEC3_OP_FLOAT2_IMPL(X), v.w X o
#define PXL_MATH_VEC2_OP_IMPL(X) x X o.x, y X o.y
#define PXL_MATH_VEC3_OP_IMPL(X) PXL_MATH_VEC2_OP_IMPL(X), z X o.z
#define PXL_MATH_VEC4_OP_IMPL(X) PXL_MATH_VEC3_OP_IMPL(X), w X o.w
#define PXL_MATH_VEC2_COMP_IMPL(X, Y) x X o.x Y y X o.y
#define PXL_MATH_VEC3_COMP_IMPL(X, Y) PXL_MATH_VEC2_COMP_IMPL(X, Y) Y z X o.z
#define PXL_MATH_VEC4_COMP_IMPL(X, Y) PXL_MATH_VEC3_COMP_IMPL(X, Y) Y w X o.w

	union vec2 {
		float data[2];
		struct { float x, y; };

		PXL_MATH_VEC_IMPL(2)
	};

	union vec3 {
		float data[3];
		struct {
			union {
				struct { float x, y; };
				vec2 xy;
			};
			float z;
		};

		PXL_MATH_VEC_IMPL(3)
	};

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

		PXL_MATH_VEC_IMPL(4)
	};
}