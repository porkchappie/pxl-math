#pragma once

#include <cmath>
#include <limits>

namespace pxl {
	namespace mathf {
		inline static constexpr float epsilon{ std::numeric_limits<float>::epsilon() }; // TODO ??
		inline static constexpr float infinity{ std::numeric_limits<float>::infinity() };
		inline static constexpr float pi{ 3.14159265359f };
		inline static constexpr float halfpi{ 1.57079632679f };
		inline static constexpr float deg2rad{ 0.01745329252f };
		inline static constexpr float rad2deg{ 57.295779513f };
		inline static constexpr float rsqrt(const float& x) {
			int i = *(int*)&x;
			i = 0x5f3759df - (i >> 1);
			const float y = *(float*)&i;
			return y * (1.5f - 0.5f * x * y * y);
		}
		inline static constexpr float radian(const float& a) { return a * mathf::deg2rad; }
		inline static constexpr float degree(const float& a) { return a * mathf::rad2deg; }
		inline static constexpr float min(const float& a, const float& b) { return a < b ? a : b; }
		inline static constexpr float max(const float& a, const float& b) { return a > b ? a : b; }
		inline static constexpr float clamp(const float& a, float x, float y) { return x > y ? (a > x ? x : a < y ? y : a) : (a < x ? x : a > y ? y : a); }
		inline static constexpr float clamp01(const float& a) { return a < 0 ? 0 : a > 1 ? 1 : a; }
		inline static constexpr float lerp(const float& x, const float& y, const float& a) { return x * (1 - a) + y * a; }
		inline static constexpr float clamped_lerp(const float& x, const float& y, const float& a) { return lerp(x, y, clamp01(a)); }
	}
}