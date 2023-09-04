#pragma once

/// <summary>
/// 2次元ベクトル
/// </summary>
struct Vector2 final {
	float x;
	float y;

	const Vector2 operator+(const Vector2& v1) {
		Vector2 result{};
		result.x = x + v1.x;
		result.y = y + v1.y;
		return result;
	}

	const Vector2 operator+=(const Vector2& v1) {
		Vector2 result{};
		result.x = x + v1.x;
		result.y = y + v1.y;
		return result;
	}

		const Vector2 operator-(const Vector2& v1) {
		Vector2 result{};
		result.x = x - v1.x;
		result.y = y - v1.y;
		return result;
	}

	const Vector2 operator-=(const Vector2& v1) {
		Vector2 result{};
		result.x = x - v1.x;
		result.y = y - v1.y;
		return result;
	}
};

