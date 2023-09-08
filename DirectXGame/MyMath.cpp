#include "MyMath.h"
#include <cmath>
#include <assert.h>
#include <algorithm>



#pragma region Vector2 

#pragma region Operator OverLoad

inline Vector2 operator+(const Vector2& v1, const Vector2& v2) {
	Vector2 temp;
	temp.x = v1.x + v2.x;
	temp.y = v1.y + v2.y;
	return temp;
}
inline Vector2 operator-(const Vector2& v1, const Vector2& v2) {
	Vector2 temp;
	temp.x = v1.x - v2.x;
	temp.y = v1.y - v2.y;
	return temp;
}
inline Vector2 operator*(const Vector2& v, float scalar) {
	Vector2 temp;
	temp.x = v.x * scalar;
	temp.y = v.y * scalar;
	return temp;
}
inline Vector2 operator*(float scalar, const Vector2& v) {
	Vector2 temp;
	temp.x = v.x * scalar;
	temp.y = v.y * scalar;
	return temp;
}

inline Vector2& operator+=(Vector2& v1, const Vector2& v2) {
	v1 = v1 + v2;
	return v1;
}

inline Vector2& operator-=(Vector2& v1, const Vector2& v2) {
	v1 = v1 - v2;
	return v1;
}

inline Vector2& operator*=(Vector2& v, float scalar) {
	v = v * scalar;
	return v;
}

#pragma endregion

float MyMath::Dot(const Vector2& v1, const Vector2& v2) {
	float result;
	result = v1.x * v2.x + v1.y * v2.y;

	return result;
}

float MyMath::Cross(const Vector2& v1, const Vector2& v2) {
	float result{};
	result = v1.x * v2.y - v1.y * v2.x;

	return result;
}

float MyMath::Length(const Vector2& v) {
	float result;
	result = sqrtf(v.x * v.x + v.y * v.y);

	return result;
}

Vector2 MyMath::Normalize(const Vector2& v) {
	Vector2 result{};
	if (v.x == 0.0f && v.y == 0.0f) {
		result.x = 0.0f;
		result.y = 0.0f;
		return result;
	}

	result.x = v.x / Length(v);
	result.y = v.y / Length(v);

	return result;
}

Vector2 MyMath::TransformCoord(Vector2 vector, Matrix3x3 matrix) {
	Vector2 result{};
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	return result;
}

Vector2 MyMath::TransformNormal(const Vector2& v, const Matrix3x3& m) {
	Vector2 result{
	    v.x * m.m[0][0] + v.y * m.m[1][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1],
	};
	return result;
}

#pragma endregion

#pragma region Vector3

#pragma region Operator OverLoad

inline Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 temp;
	temp.x = v1.x + v2.x;
	temp.y = v1.y + v2.y;
	temp.z = v1.z + v2.z;

	return temp;
}
inline Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	Vector3 temp;
	temp.x = v1.x - v2.x;
	temp.y = v1.y - v2.y;
	temp.z = v1.z - v2.z;
	return temp;
}
inline Vector3 operator*(const Vector3& v, float scalar) {
	Vector3 temp;
	temp.x = v.x * scalar;
	temp.y = v.y * scalar;
	temp.z = v.z * scalar;
	return temp;
}
inline Vector3 operator*(float scalar, const Vector3& v) {
	Vector3 temp;
	temp.x = v.x * scalar;
	temp.y = v.y * scalar;
	temp.z = v.z * scalar;
	return temp;
}

inline Vector3& operator+=(Vector3& v1, const Vector3& v2) {
	v1 = v1 + v2;
	return v1;
}

inline Vector3& operator-=(Vector3& v1, const Vector3& v2) {
	v1 = v1 - v2;
	return v1;
}

inline Vector3& operator*=(Vector3& v, float scalar) {
	v = v * scalar;
	return v;
}

#pragma endregion

float MyMath::Dot(const Vector3& v1, const Vector3& v2) {
	float result;
	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return result;
}
float MyMath::Length(const Vector3& v) {
	float result;
	result = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

	return result;
}
Vector3 MyMath::Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}
Vector3 MyMath::Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;

	return result;
}
Vector3 MyMath::Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}
Vector3 MyMath::Multiply(const float& v1, const Vector3& v2) {
	Vector3 result{};
	result.x = v1 * v2.x;
	result.y = v1 * v2.y;
	result.z = v1 * v2.z;
	return result;
}
Vector3 MyMath::TransformCoord(Vector3 vector, Matrix4x4 matrix) {
	Vector3 result{};
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
	           1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
	           1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
	           1.0f * matrix.m[3][2];

	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
	          1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}
Vector3 MyMath::Project(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};

	result = Normalize(v2);
	float a = Dot(v1, result);
	result.x *= a;
	result.y *= a;

	return result;
}
Vector3 MyMath::Normalize(const Vector3& v) {
	Vector3 result{};
	if (v.x == 0.0f && v.y == 0.0f && v.z == 0.0f) {
		result.x = 0.0f;
		result.y = 0.0f;
		result.z = 0.0f;
		return result;
	}

	result.x = v.x / Length(v);
	result.y = v.y / Length(v);
	result.z = v.z / Length(v);

	return result;
}
Vector3 MyMath::Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return {-vector.y, vector.x, 0.0f};
	}
	return {0.0f, -vector.z, vector.y};
}

Vector3 MyMath::TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};
	return result;
}

#pragma endregion

#pragma region Matrix3x3
Matrix3x3 MyMath::Multiply(const Matrix3x3& m1, const Matrix3x3& m2) {
	Matrix3x3 result{};
	for (int row = 0; row < 3; row++) {
		for (int column = 0; column < 3; column++) {
			result.m[row][column] = m1.m[row][0] * m2.m[0][column] +
			                        m1.m[row][1] * m2.m[1][column] + m1.m[row][2] * m2.m[2][column];
		}
	}

	return result;
}

Matrix3x3 MyMath::MakeTranslateMatrix(const Vector2 translate) {

	Matrix3x3 result{};

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result.m[i][j] = 0;
		}
	}

	result.m[0][0] = 1;
	result.m[1][1] = 1;
	result.m[2][2] = 1;

	result.m[2][0] = translate.x;
	result.m[2][1] = translate.y;

	return result;
}

Matrix3x3 MyMath::MakeScaleMatrix(const Vector2 scale) {
	Matrix3x3 result{};

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result.m[i][j] = 0;
		}
	}
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = 1;

	return result;
}

Matrix3x3 MyMath::MakeRotateMatrix(float radian) {
	Matrix3x3 result{};

	result.m[0][0] = std::cos(radian);
	result.m[1][0] = -(std::sin(radian));
	result.m[0][1] = std::sin(radian);
	result.m[1][1] = std::cos(radian);
	result.m[2][2] = 1;

	return result;
}

Matrix3x3
    MyMath::MakeAffineMatrix(const Vector2 scale, const float rotate, const Vector2 translate) {
	Matrix3x3 result{};

	Matrix3x3 scaleMat = MakeScaleMatrix(scale);
	Matrix3x3 rotateMat = MakeRotateMatrix(rotate);
	Matrix3x3 translateMat = MakeTranslateMatrix(translate);

	result = Multiply(Multiply(scaleMat, rotateMat), translateMat);

	return result;
}

#pragma endregion

#pragma region Matrix4x4
Matrix4x4 MyMath::Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result{};

	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			result.m[row][column] = m1.m[row][0] * m2.m[0][column] +
			                        m1.m[row][1] * m2.m[1][column] +
			                        m1.m[row][2] * m2.m[2][column] + m1.m[row][3] * m2.m[3][column];
		}
	}

	/*result.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] +
	m1.m[0][3] * m2.m[3][0]; result.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] +
	m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1]; result.m[0][2] = m1.m[0][0] * m2.m[0][2] +
	m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2]; result.m[0][3] =
	m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] *
	m2.m[3][3];

	result.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] +
	m1.m[1][3] * m2.m[3][0]; result.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] +
	m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1]; result.m[1][2] = m1.m[1][0] * m2.m[0][2] +
	m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2]; result.m[1][3] =
	m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] *
	m2.m[3][3];

	result.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] +
	m1.m[2][3] * m2.m[3][0]; result.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] +
	m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1]; result.m[2][2] = m1.m[2][0] * m2.m[0][2] +
	m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2]; result.m[2][3] =
	m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] *
	m2.m[3][3];

	result.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] +
	m1.m[3][3] * m2.m[3][0]; result.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] +
	m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1]; result.m[3][2] = m1.m[3][0] * m2.m[0][2] +
	m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2]; result.m[3][3] =
	m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] *
	m2.m[3][3];*/
	return result;
}

Matrix4x4 MyMath::MakeTranslateMatrix(const Vector3 translate) {
	Matrix4x4 result{};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = 0;
		}
	}

	result.m[0][0] = 1;
	result.m[1][1] = 1;
	result.m[2][2] = 1;
	result.m[3][3] = 1;

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	return result;
}

Matrix4x4 MyMath::MakeScaleMatrix(const Vector3 scale) {
	Matrix4x4 result{};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = 0;
		}
	}
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 MyMath::MakeRotateXMatrix(float radian) {
	Matrix4x4 result{};

	result.m[0][0] = 1;
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[2][1] = -(std::sin(radian));
	result.m[2][2] = std::cos(radian);
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 MyMath::MakeRotateYMatrix(float radian) {
	Matrix4x4 result{};

	result.m[0][0] = std::cos(radian);
	result.m[0][2] = -(std::sin(radian));
	result.m[1][1] = 1;
	result.m[2][0] = std::sin(radian);
	result.m[2][2] = std::cos(radian);
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 MyMath::MakeRotateZMatrix(float radian) {
	Matrix4x4 result{};

	result.m[0][0] = std::cos(radian);
	result.m[1][0] = -(std::sin(radian));
	result.m[0][1] = std::sin(radian);
	result.m[1][1] = std::cos(radian);
	result.m[2][2] = 1;
	result.m[3][3] = 1;

	return result;
}

Matrix4x4
    MyMath::MakeAffineMatrix(const Vector3 scale, const Vector3 rotate, const Vector3 translate) {
	Matrix4x4 result{};

	/*Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);*/
	Matrix4x4 rotateXYZMatrix = Multiply(
	    MakeRotateXMatrix(rotate.x),
	    Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));

	/*result2 = Multiply(rotateXYZMatrix, translateMatrix);
	result = Multiply(rotateXYZMatrix, scaleMatrix);*/
	result.m[0][0] = rotateXYZMatrix.m[0][0] * scale.x;
	result.m[0][1] = rotateXYZMatrix.m[0][1] * scale.x;
	result.m[0][2] = rotateXYZMatrix.m[0][2] * scale.x;
	result.m[0][3] = 0;

	result.m[1][0] = rotateXYZMatrix.m[1][0] * scale.y;
	result.m[1][1] = rotateXYZMatrix.m[1][1] * scale.y;
	result.m[1][2] = rotateXYZMatrix.m[1][2] * scale.y;
	result.m[1][3] = 0;

	result.m[2][0] = rotateXYZMatrix.m[2][0] * scale.z;
	result.m[2][1] = rotateXYZMatrix.m[2][1] * scale.z;
	result.m[2][2] = rotateXYZMatrix.m[2][2] * scale.z;
	result.m[2][3] = 0;

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1;

	return result;
}

Matrix4x4
    MyMath::MakePerspectiveFovMatrix(float fovY, float aspectRetio, float nearClip, float farClip) {
	Matrix4x4 result{};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = 0;
		}
	}

	result.m[0][0] = (1 / aspectRetio) * (1 / tan(fovY / 2));
	result.m[1][1] = (1 / tan(fovY / 2));
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1;
	result.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);

	return result;
}

Matrix4x4 MyMath::MakeOrthographicMatrix(
    float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result{};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = 0;
		}
	}

	result.m[0][0] = 2 / (right - left);
	result.m[1][1] = 2 / (top - bottom);
	result.m[2][2] = 1 / (farClip - nearClip);
	result.m[3][3] = 1;

	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = (nearClip) / (nearClip - farClip);

	return result;
}

Matrix4x4 MyMath::MakeViewPortMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result{};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = 0;
		}
	}
	result.m[0][0] = width / 2;
	result.m[1][1] = -(height / 2);
	result.m[2][2] = maxDepth - minDepth;
	result.m[3][0] = left + (width / 2);
	result.m[3][1] = top + (height / 2);
	result.m[3][2] = minDepth;
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 MyMath::Inverse(const Matrix4x4& m) {
	float a = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] +
	          m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
	          m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]

	          - m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
	          m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] -
	          m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]

	          - m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] -
	          m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
	          m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]

	          + m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] +
	          m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] +
	          m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]

	          + m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] +
	          m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] +
	          m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]

	          - m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
	          m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] -
	          m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]

	          - m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] -
	          m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
	          m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]

	          + m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] +
	          m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] +
	          m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	Matrix4x4 result{};
	result.m[0][0] = (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +
	                  m.m[1][3] * m.m[2][1] * m.m[3][2]

	                  - m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][3] -
	                  m.m[1][1] * m.m[2][3] * m.m[3][2]) /
	                 a;

	result.m[0][1] = (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] -
	                  m.m[0][3] * m.m[2][1] * m.m[3][2]

	                  + m.m[0][3] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[2][1] * m.m[3][3] +
	                  m.m[0][1] * m.m[2][3] * m.m[3][2]) /
	                 a;

	result.m[0][2] = (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] +
	                  m.m[0][3] * m.m[1][1] * m.m[3][2]

	                  - m.m[0][3] * m.m[1][2] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][3] -
	                  m.m[0][1] * m.m[1][3] * m.m[3][2]) /
	                 a;

	result.m[0][3] = (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] -
	                  m.m[0][3] * m.m[1][1] * m.m[2][2]

	                  + m.m[0][3] * m.m[1][2] * m.m[2][1] + m.m[0][2] * m.m[1][1] * m.m[2][3] +
	                  m.m[0][1] * m.m[1][3] * m.m[2][2]) /
	                 a;

	result.m[1][0] = (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] -
	                  m.m[1][3] * m.m[2][0] * m.m[3][2]

	                  + m.m[1][3] * m.m[2][2] * m.m[3][0] + m.m[1][2] * m.m[2][0] * m.m[3][3] +
	                  m.m[1][0] * m.m[2][3] * m.m[3][2]) /
	                 a;

	result.m[1][1] = (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] +
	                  m.m[0][3] * m.m[2][0] * m.m[3][2]

	                  - m.m[0][3] * m.m[2][2] * m.m[3][0] - m.m[0][2] * m.m[2][0] * m.m[3][3] -
	                  m.m[0][0] * m.m[2][3] * m.m[3][2]) /
	                 a;

	result.m[1][2] = (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] -
	                  m.m[0][3] * m.m[1][0] * m.m[3][2]

	                  + m.m[0][3] * m.m[1][2] * m.m[3][0] + m.m[0][2] * m.m[1][0] * m.m[3][3] +
	                  m.m[0][0] * m.m[1][3] * m.m[3][2]) /
	                 a;

	result.m[1][3] = (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] +
	                  m.m[0][3] * m.m[1][0] * m.m[2][2]

	                  - m.m[0][3] * m.m[1][2] * m.m[2][0] - m.m[0][2] * m.m[1][0] * m.m[2][3] -
	                  m.m[0][0] * m.m[1][3] * m.m[2][2]) /
	                 a;

	result.m[2][0] = (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] +
	                  m.m[1][3] * m.m[2][0] * m.m[3][1]

	                  - m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[1][1] * m.m[2][0] * m.m[3][3] -
	                  m.m[1][0] * m.m[2][3] * m.m[3][1]) /
	                 a;

	result.m[2][1] = (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] -
	                  m.m[0][3] * m.m[2][0] * m.m[3][1]

	                  + m.m[0][3] * m.m[2][1] * m.m[3][0] + m.m[0][1] * m.m[2][0] * m.m[3][3] +
	                  m.m[0][0] * m.m[2][3] * m.m[3][1]) /
	                 a;

	result.m[2][2] = (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] +
	                  m.m[0][3] * m.m[1][0] * m.m[3][1]

	                  - m.m[0][3] * m.m[1][1] * m.m[3][0] - m.m[0][1] * m.m[1][0] * m.m[3][3] -
	                  m.m[0][0] * m.m[1][3] * m.m[3][1]) /
	                 a;

	result.m[2][3] = (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] -
	                  m.m[0][3] * m.m[1][0] * m.m[2][1]

	                  + m.m[0][3] * m.m[1][1] * m.m[2][0] + m.m[0][1] * m.m[1][0] * m.m[2][3] +
	                  m.m[0][0] * m.m[1][3] * m.m[2][1]) /
	                 a;

	result.m[3][0] = (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] -
	                  m.m[1][2] * m.m[2][0] * m.m[3][1]

	                  + m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[1][1] * m.m[2][0] * m.m[3][2] +
	                  m.m[1][0] * m.m[2][2] * m.m[3][1]) /
	                 a;

	result.m[3][1] = (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] +
	                  m.m[0][2] * m.m[2][0] * m.m[3][1]

	                  - m.m[0][2] * m.m[2][1] * m.m[3][0] - m.m[0][1] * m.m[2][0] * m.m[3][2] -
	                  m.m[0][0] * m.m[2][2] * m.m[3][1]) /
	                 a;

	result.m[3][2] = (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] -
	                  m.m[0][2] * m.m[1][0] * m.m[3][1]

	                  + m.m[0][2] * m.m[1][1] * m.m[3][0] + m.m[0][1] * m.m[1][0] * m.m[3][2] +
	                  m.m[0][0] * m.m[1][2] * m.m[3][1]) /
	                 a;

	result.m[3][3] = (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
	                  m.m[0][2] * m.m[1][0] * m.m[2][1]

	                  - m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] -
	                  m.m[0][0] * m.m[1][2] * m.m[2][1]) /
	                 a;

	return result;
}

Matrix4x4 MyMath::Transpose(const Matrix4x4& m) {
	Matrix4x4 result{};

	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			result.m[row][column] = m.m[column][row];
		}
	}

	// result.m[0][0] = m.m[0][0];
	// result.m[0][1] = m.m[1][0];
	// result.m[0][2] = m.m[2][0];
	// result.m[0][3] = m.m[3][0];

	// result.m[1][0] = m.m[0][1];
	// result.m[1][1] = m.m[1][1];
	// result.m[1][2] = m.m[2][1];
	// result.m[1][3] = m.m[3][1];

	return result;
}

Matrix4x4 MyMath::MakeIdentity4x4() {
	Matrix4x4 result{};

	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			result.m[row][column] = 0;
		}
	}
	result.m[0][0] = 1;
	result.m[1][1] = 1;
	result.m[2][2] = 1;
	result.m[3][3] = 1;

	return result;
}

#pragma endregion

#pragma region 便利系

void MyMath::CountT(float& t, const float endT, bool& flag, const bool setFlag, float offset) {
	if (t >= 1.0f) {
		t = endT;
		flag = setFlag;
	} else {
		t += offset;
	}
}

float MyMath::Mymax(float a, float b) {
	if (a > b) {
		return a;
	}
	else if (a < b) {
		return b;
	}
	return a;
}

float MyMath::Mymin(float a, float b) {
	if (a < b) {
		return a;
	}
	else if (a > b) {
		return b;
	}
	return a;
}
float MyMath::gcd(float a, float b) {
	//
	if (a == 0 || b == 0) {
		return 0;
	}

	while (a != b) {
		if (a < b) {
			b -= a;
		} else {
			a -= b;
		}
	}
	return a;
}

#pragma endregion

#pragma region catmullrom

Vector2 MyMath::CatmullRom(
    const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3, float t) {
	Vector2 pt = (1 / 2.0f) * ((Vector2(-p0.x, -p0.y) + 3 * p1 - 3 * p2 + p3) * powf(t, 3) +
	                           (2 * p0 - 5 * p1 + 4 * p2 - p3) * powf(t, 2) +
	                           (Vector2(-p0.x, -p0.y) + p2) * t + 2 * p1);
	return pt;
}
#pragma endregion

#pragma region lerp


//
float MyMath::lerp(float t, float start, float end) { 
	return (1.0f - t) * start + t * end;
}

Vector2 MyMath::lerp(float t, Vector2 start, Vector2 end) { 
	return Vector2(
		(1.0f - t) * start.x + t * end.x, 
		(1.0f - t) * start.y + t * end.y);
}

Vector3 MyMath::lerp(float t, Vector3 start, Vector3 end) {
	return Vector3(
	    (1.0f - t) * start.x + t * end.x, 
		(1.0f - t) * start.y + t * end.y,
	    (1.0f - t) * start.z + t * end.z);
}
#pragma endregion

#pragma region float Easing

float MyMath::EaseInQuadF(float t, float start, float end) {
	float easeT = t * t;
	return (1.0f - easeT) * start + easeT * end;
}

float MyMath::EaseOutQuadF(float t, float start, float end) {
	float easeT = 1.0f - powf(1.0f - t, 2.0f);
	return (1.0f - easeT) * start + easeT * end;
}

float MyMath::EaseInOutQuadF(float t, float start, float end) {
	float easeT = t > 0.5f ? 2.0f * t * t
	                         : 1.0f - powf(-2.0f * t + 2.0f, 2.0f) / 2.0f;

	return (1.0f - easeT) * start + easeT * end;
}

float MyMath::EaseInCubicF(float t, float start, float end) {
	float easeT = t * t * t;
	return (1.0f - easeT) * start + easeT * end;
}

float MyMath::EaseOutCubicF(float t, float start, float end) {
	float easeT = 1.0f - powf(1.0f - t, 3.0f);
	return (1.0f - easeT) * start + easeT * end;
}

float MyMath::EaseInOutCubicF(float t, float start, float end) {
	float easeT = t > 0.5f ? 4.0f *t *t * t
	                                : 1.0f - powf(-2.0f * t + 2, 3.0f) / 2.0f;

	return (1.0f - easeT) * start + easeT * end;
}
#pragma endregion

#pragma region Vector2 Easing
Vector2 MyMath::EaseInQuadF(float t, Vector2 start, Vector2 end) {
	float easeT = t * t;
	return Vector2(
	    (1.0f - easeT) * start.x + easeT * end.x, 
		(1.0f - easeT) * start.y + easeT * end.y);
}
Vector2 MyMath::EaseOutQuadF(float t, Vector2 start, Vector2 end) {
	float easeT = 1.0f - powf(1.0f - t, 2.0f);
	return Vector2(
		(1.0f - easeT) * start.x + easeT * end.x, 
		(1.0f - easeT) * start.y + easeT * end.y);
}
Vector2 MyMath::EaseInOutQuadF(float t, Vector2 start, Vector2 end) {
	float easeT = t > 0.5f ? 2.0f * t * t : 1.0f - powf(-2.0f * t + 2.0f, 2.0f) / 2.0f;

	return Vector2(
		(1.0f - easeT) * start.x + easeT * end.x,
		(1.0f - easeT) * start.y + easeT * end.y);
}

Vector2 MyMath::EaseInCubicF(float t, Vector2 start, Vector2 end) {
	float easeT = t * t * t;
	return Vector2(
	    (1.0f - easeT) * start.x + easeT * end.x,
		(1.0f - easeT) * start.y + easeT * end.y);
}
Vector2 MyMath::EaseOutCubicF(float t, Vector2 start, Vector2 end) {
	float easeT = 1.0f - powf(1.0f - t, 3.0f);
	return Vector2(
		(1.0f - easeT) * start.x + easeT * end.x,
		(1.0f - easeT) * start.y + easeT * end.y);
}
Vector2 MyMath::EaseInOutCubicF(float t, Vector2 start, Vector2 end) {
	float easeT = t > 0.5f ? 4.0f * t * t * t : 1.0f - powf(-2.0f * t + 2, 3.0f) / 2.0f;

	return Vector2(
	    (1.0f - easeT) * start.x + easeT * end.x,
		(1.0f - easeT) * start.y + easeT * end.y);
}


#pragma endregion

#pragma region 使わないやつ

// float MyMath::Clamp(float num, float max, float min) {
//	if (num > max) {
//		return max;
//	}
//	else if (num < min) {
//		return min;
//	}
//	else {
//		return num;
//	}
// }

// Vector3 MyMath::ClosestPoint(const Vector3& point, const Segment& segment) {
//	float t = Dot(Subtract(point, segment.origin), segment.diff) /
// std::powf(Length(segment.diff), 2.0f); 	Vector3 result = Add(segment.origin, Multiply(t,
// segment.diff));
//
//	t = std::clamp(t, 1.0f, 0.0f);
//
//
//	return result;
// }

// bool MyMath::IsCollision(const Sphere& s1, const Sphere& s2) {
//	float distance = Length(Subtract(s2.center, s1.center));
//
//	if (distance <= s1.radius + s2.radius) {
//		return	true;
//	}
//
//	return false;
//
// }
// bool MyMath::IsCollision(const Sphere& s1, const Plane& plane) {
//	float k = std::abs(Dot(plane.normal, s1.center) - plane.distance);
//
//	if (s1.radius > k) {
//		return true;
//	}
//	return false;
// }
//
//
// bool MyMath::IsCollision(const Segment& line, const Plane& plane) {
//	float dot = Dot(plane.normal, line.diff);
//
//	if (dot == 0.0f) {
//		return false;
//	}
//
//	//
//	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;
//
//	if (0.0f < t && t < 1.0f) {
//		return true;
//	}
//	return false;
// }
// bool MyMath::IsCollision(const Ray& line, const Plane& plane) {
//	float dot = Dot(plane.normal, line.diff);
//
//	if (dot == 0.0f) {
//		return false;
//	}
//
//	//
//	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;
//
//	if (0.0f < t) {
//		return true;
//	}
//	return false;
// }
// bool MyMath::IsCollision(const Line& line, const Plane& plane) {
//	float dot = Dot(plane.normal, line.diff);
//
//	if (dot == 0.0f) {
//		return false;
//	}
//
//	//
//	//float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;
//
//
//	return true;
// }
//
////bool MyMath::IsCollision(const Plane& plane, Segment* segment, Line* line, Ray* ray) {
////	bool result = false;
////	if (segment) {
////		result = IsCollision(*segment, plane);
////	}
////	else if (ray) {
////		result = IsCollision(*ray, plane);
////	}
////	else if (line) {
////		result = IsCollision(*line, plane);
////	}
////
////
////	return result;
////}
//
// bool MyMath::IsCollision(const Triangle& triangle, const Segment& segment) {
//	Plane plane{};
//	plane.normal =
//		MyMath::Normalize(
//			MyMath::Cross(
//				MyMath::Subtract(triangle.vertices[1], triangle.vertices[0]),
//				MyMath::Subtract(triangle.vertices[2], triangle.vertices[1])
//			)
//		);
//
//	plane.distance = MyMath::Dot(triangle.vertices[0], plane.normal);
//
//	float dot = Dot(plane.normal, segment.diff);
//
//	if (dot == 0.0f) {
//		return false;
//	}
//	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;
//
//	if (0.0f < t && t < 1.0f) {
//		Vector3 p = MyMath::Add(segment.origin, MyMath::Multiply(t, segment.diff));
//
//
//		Vector3 cross01 = MyMath::Cross(
//			MyMath::Subtract(triangle.vertices[1], triangle.vertices[0]),
//			MyMath::Subtract(p, triangle.vertices[1])
//		);
//		Vector3 cross12 = MyMath::Cross(
//			MyMath::Subtract(triangle.vertices[2], triangle.vertices[1]),
//			MyMath::Subtract(p, triangle.vertices[2])
//		);
//		Vector3 cross20 = MyMath::Cross(
//			MyMath::Subtract(triangle.vertices[0], triangle.vertices[2]),
//			MyMath::Subtract(p, triangle.vertices[0])
//		);
//
//
//		if (MyMath::Dot(cross01, plane.normal) >= 0.0f &&
//			MyMath::Dot(cross12, plane.normal) >= 0.0f &&
//			MyMath::Dot(cross20, plane.normal) >= 0.0f) {
//			return true;
//		}
//
//	}
//
//	return false;
//
//}
//
//
//
//
// bool MyMath::IsCollision(const AABB& aabb1, const AABB& aabb2) {
//	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
//		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
//		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)
//		) {
//
//		return true;
//
//	}
//
//	return false;
//
//}
//
// bool MyMath::IsCollision(const AABB& aabb, const Sphere& sphere) {
//	Vector3 clossestPoint{
//		std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
//		std::clamp(sphere.center.y, aabb.min.y, aabb.max.y),
//		std::clamp(sphere.center.z, aabb.min.z, aabb.max.z)
//	};
//
//	float distance = Length(Subtract(clossestPoint, sphere.center));
//	if (distance <= sphere.radius) {
//		return true;
//	}
//
//	return false;
//
//}
//
// bool MyMath::IsCollision(const AABB& aabb, const Segment& segment) {
//	Plane xMin;
//	Plane xMAx;
//
//	Plane yMin;
//	Plane yMax;
//
//	Plane zMin;
//	Plane zMax;
//
//	xMin.normal = { 1, 0, 0 };
//	xMin.distance = aabb.min.x;
//	xMAx.normal = { 1, 0, 0 };
//	xMAx.distance = aabb.max.x;
//
//	yMin.normal = { 0, 1, 0 };
//	yMin.distance = aabb.min.y;
//
//	yMax.normal = { 0, 1, 0 };
//	yMax.distance = aabb.max.y;
//
//	zMin.normal = { 0, 0, 1 };
//	zMin.distance = aabb.min.z;
//
//	zMax.normal = { 0, 0, 1 };
//	zMax.distance = aabb.max.z;
//
//	float dot = Dot(xMin.normal, segment.diff);
//	float tMin = (xMin.distance - Dot(segment.origin, xMin.normal)) / dot;
//	dot = Dot(xMAx.normal, segment.diff);
//	float tMax = (xMAx.distance - Dot(segment.origin, xMAx.normal)) / dot;
//
//	float tNearX = std::min(tMin, tMax);
//	float tFarX = std::max(tMin, tMax);
//
//
//	dot = Dot(yMin.normal, segment.diff);
//	tMin = (yMin.distance - Dot(segment.origin, yMin.normal)) / dot;
//	dot = Dot(yMax.normal, segment.diff);
//	tMax = (yMax.distance - Dot(segment.origin, yMax.normal)) / dot;
//
//	float tNearY = std::min(tMin, tMax);
//	float tFarY = std::max(tMin, tMax);
//
//	dot = Dot(zMin.normal, segment.diff);
//	tMin = (zMin.distance - Dot(segment.origin, zMin.normal)) / dot;
//	dot = Dot(zMax.normal, segment.diff);
//	tMax = (zMax.distance - Dot(segment.origin, zMax.normal)) / dot;
//
//	float tNearZ = std::min(tMin, tMax);
//	float tFarZ = std::max(tMin, tMax);
//
//
//	float tNear = std::max(std::max(tNearX, tNearY), tNearZ);
//	float tFar = std::min(std::min(tFarX, tFarY), tFarZ);
//
//	//float tmin = tNearX;
//	//float tmax = tFarX;
//	if (tNear <= tFar) {
//		if ((tNear < 1.0f) && (0.0f < tFar)) {
//			return true;
//		}
//	}
//
//	return false;
//}
//
// bool MyMath::IsCollision(const AABB& aabb, const Line& line) {
//	/*enum {
//		NEAR,
//		FAR
//	};
//
//	enum {
//
//	};*/
//
//	Plane xMin;
//	Plane xMAx;
//
//	Plane yMin;
//	Plane yMax;
//
//	Plane zMin;
//	Plane zMax;
//
//
//	xMin.normal = { 1, 0, 0 };
//	xMin.distance = aabb.min.x;
//	xMAx.normal = { 1, 0, 0 };
//	xMAx.distance = aabb.max.x;
//
//	yMin.normal = { 0, 1, 0 };
//	yMin.distance = aabb.min.y;
//
//	yMax.normal = { 0, 1, 0 };
//	yMax.distance = aabb.max.y;
//
//	zMin.normal = { 0, 0, 1 };
//	zMin.distance = aabb.min.z;
//
//	zMax.normal = { 0, 0, 1 };
//	zMax.distance = aabb.max.z;
//
//	float dot = Dot(xMin.normal, line.diff);
//	float tMin = (xMin.distance - Dot(line.origin, xMin.normal)) / dot;
//	dot = Dot(xMAx.normal, line.diff);
//	float tMax = (xMAx.distance - Dot(line.origin, xMAx.normal)) / dot;
//
//	float tNearX = std::min(tMin, tMax);
//	float tFarX = std::max(tMin, tMax);
//
//
//	dot = Dot(yMin.normal, line.diff);
//	tMin = (yMin.distance - Dot(line.origin, yMin.normal)) / dot;
//	dot = Dot(yMax.normal, line.diff);
//	tMax = (yMax.distance - Dot(line.origin, yMax.normal)) / dot;
//
//	float tNearY = std::min(tMin, tMax);
//	float tFarY = std::max(tMin, tMax);
//
//	dot = Dot(zMin.normal, line.diff);
//	tMin = (zMin.distance - Dot(line.origin, zMin.normal)) / dot;
//	dot = Dot(zMax.normal, line.diff);
//	tMax = (zMax.distance - Dot(line.origin, zMax.normal)) / dot;
//
//	float tNearZ = std::min(tMin, tMax);
//	float tFarZ = std::max(tMin, tMax);
//
//
//	float tNear = std::max(std::max(tNearX, tNearY), tNearZ);
//	float tFar = std::min(std::min(tFarX, tFarY), tFarZ);
//
//	//float tmin = tNearX;
//	//float tmax = tFarX;
//	if (tNear <= tFar) {
//		return true;
//	}
//
//	return false;
//}
//
// bool MyMath::IsCollision(const AABB& aabb, const Ray& ray) {
//	/*enum {
//		NEAR,
//		FAR
//	};
//
//	enum {
//
//	};*/
//
//	Plane xMin;
//	Plane xMAx;
//
//	Plane yMin;
//	Plane yMax;
//
//	Plane zMin;
//	Plane zMax;
//
//
//	xMin.normal = { 1, 0, 0 };
//	xMin.distance = aabb.min.x;
//	xMAx.normal = { 1, 0, 0 };
//	xMAx.distance = aabb.max.x;
//
//	yMin.normal = { 0, 1, 0 };
//	yMin.distance = aabb.min.y;
//
//	yMax.normal = { 0, 1, 0 };
//	yMax.distance = aabb.max.y;
//
//	zMin.normal = { 0, 0, 1 };
//	zMin.distance = aabb.min.z;
//
//	zMax.normal = { 0, 0, 1 };
//	zMax.distance = aabb.max.z;
//
//	float dot = Dot(xMin.normal, ray.diff);
//	float tMin = (xMin.distance - Dot(ray.origin, xMin.normal)) / dot;
//	dot = Dot(xMAx.normal, ray.diff);
//	float tMax = (xMAx.distance - Dot(ray.origin, xMAx.normal)) / dot;
//
//	float tNearX = std::min(tMin, tMax);
//	float tFarX = std::max(tMin, tMax);
//
//
//	dot = Dot(yMin.normal, ray.diff);
//	tMin = (yMin.distance - Dot(ray.origin, yMin.normal)) / dot;
//	dot = Dot(yMax.normal, ray.diff);
//	tMax = (yMax.distance - Dot(ray.origin, yMax.normal)) / dot;
//
//	float tNearY = std::min(tMin, tMax);
//	float tFarY = std::max(tMin, tMax);
//
//	dot = Dot(zMin.normal, ray.diff);
//	tMin = (zMin.distance - Dot(ray.origin, zMin.normal)) / dot;
//	dot = Dot(zMax.normal, ray.diff);
//	tMax = (zMax.distance - Dot(ray.origin, zMax.normal)) / dot;
//
//	float tNearZ = std::min(tMin, tMax);
//	float tFarZ = std::max(tMin, tMax);
//
//
//	float tNear = std::max(std::max(tNearX, tNearY), tNearZ);
//	float tFar = std::min(std::min(tFarX, tFarY), tFarZ);
//
//	//float tmin = tNearX;
//	//float tmax = tFarX;
//	if (tNear <= tFar) {
//		if (0.0f < tFar) {
//			return true;
//		}
//	}
//
//	return false;
//}

#pragma endregion














