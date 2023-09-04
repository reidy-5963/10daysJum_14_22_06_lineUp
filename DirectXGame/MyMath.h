#pragma once

//#include "MyStruct.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"


/// <summary>
/// 数学の関数系
/// </summary>
class MyMath {
public:
	/// <summary>
	/// ベクトル同士の内積
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <returns></returns>
	static float Dot(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// 長さ(ノルム)
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns></returns>
	static float Length(const Vector3& v);

	/// <summary>
	/// 長さ(ノルム)
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns></returns>
	static float Length(const Vector2& v);


	/// <summary>
	/// 正規化
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns></returns>
	static Vector2 Normalize(const Vector2& v);


	/// <summary>
	/// 
	/// </summary>
	/// <param name="num"></param>
	/// <param name="max"></param>
	/// <param name="min"></param>
	/// <returns></returns>
//	static float Clamp(float num, float max, float min);

	/// <summary>
	/// ベクトル同士の加算
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <returns></returns>
	static Vector3 Add(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// ベクトル同士の外積
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <returns></returns>
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// ベクトル同士の減算
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <returns></returns>
	static Vector3 Subtract(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// ベクトルと数の乗算
	/// </summary>
	/// <param name="v1">数1</param>
	/// <param name="v2">ベクトル2</param>
	/// <returns></returns>
	static Vector3 Multiply(const float& v1, const Vector3& v2);

	/// <summary>
	/// 座標変換
	/// </summary>
	/// <param name="vector">ベクトル</param>
	/// <param name="matrix">行列</param>
	/// <returns></returns>
	static Vector3 TransformCoord(Vector3 vector, Matrix4x4 matrix);


	/// <summary>
	/// 座標変換
	/// </summary>
	/// <param name="vector">ベクトル</param>
	/// <param name="matrix">行列</param>
	/// <returns></returns>
	static Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

	/// <summary>
	/// 正射影ベクトル
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static Vector3 Project(const Vector3& v1, const Vector3& v2);
	/// <summary>
	/// 最近接点
	/// </summary>
	/// <param name="point"></param>
	/// <param name="segment"></param>
	/// <returns></returns>
	//static Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

	/// <summary>
	/// 正規化
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns></returns>
	static Vector3 Normalize(const Vector3& v);

	/// <summary>
	/// 法線と垂直なベクトル
	/// </summary>
	/// <param name="vector"></param>
	/// <returns></returns>
	static Vector3 Perpendicular(const Vector3& vector);

	/// <summary>
	/// 行列同士の乗算
	/// </summary>
	/// <param name="m1">行列1</param>
	/// <param name="m2">行列2</param>
	/// <returns></returns>
	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	/// <summary>
	/// 平行移動行列の作成
	/// </summary>
	/// <param name="translate">ベクトル</param>
	/// <returns></returns>
	static Matrix4x4 MakeTranslateMatrix(const Vector3 translate);

	/// <summary>
	/// 拡大縮小行列の作成
	/// </summary>
	/// <param name="scale">ベクトル</param>
	/// <returns></returns>
	static Matrix4x4 MakeScaleMatrix(const Vector3 scale);

	/// <summary>
	/// X軸回転行列の作成
	/// </summary>
	/// <param name="radian">xラジアン</param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateXMatrix(float radian);

	/// <summary>
	/// Y軸回転行列の作成
	/// </summary>
	/// <param name="radian">yラジアン</param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateYMatrix(float radian);

	/// <summary>
	/// Z軸回転行列の作成
	/// </summary>
	/// <param name="radian">zラジアン</param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateZMatrix(float radian);

	/// <summary>
	/// アフィン変換
	/// </summary>
	/// <param name="scale">スケール</param>
	/// <param name="rotate">ローテ―ト</param>
	/// <param name="translate">トランスレート</param>
	/// <returns></returns>
	static Matrix4x4 MakeAffineMatrix(const Vector3 scale, const Vector3 rotate, const Vector3 translate);

	/// <summary>
	/// 透視投影行列
	/// </summary>
	/// <param name="fovY">縦画角</param>
	/// <param name="aspectRetio">アスペクト比</param>
	/// <param name="nearClip">近平面への距離</param>
	/// <param name="farClip">遠平面への距離</param>
	/// <returns></returns>
	static Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRetio, float nearClip, float farClip);

	/// <summary>
	/// 正射影行列
	/// </summary>
	/// <param name="left">左</param>
	/// <param name="top">上</param>
	/// <param name="right">右</param>
	/// <param name="bottom">下</param>
	/// <param name="nearClip">近平面への距離</param>
	/// <param name="farClip">遠平面への距離</param>
	/// <returns></returns>
	static Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

	/// <summary>
	/// ビューポート変換行列
	/// </summary>
	/// <param name="left">左</param>
	/// <param name="top">上</param>
	/// <param name="width">横幅</param>
	/// <param name="height">縦幅</param>
	/// <param name="minDepth">最少深度値</param>
	/// <param name="maxDepth">最大深度値</param>
	/// <returns></returns>
	static Matrix4x4 MakeViewPortMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

	/// <summary>
	/// 逆行列
	/// </summary>
	/// <param name="m">行列</param>
	/// <returns></returns>
	static Matrix4x4 Inverse(const Matrix4x4& m);

	/// <summary>
	/// 転置行列
	/// </summary>
	/// <param name="m">行列</param>
	/// <returns></returns>
	static Matrix4x4 Transpose(const Matrix4x4& m);

	/// <summary>
	/// 単位行列の作成
	/// </summary>
	/// <returns></returns>
	static Matrix4x4 MakeIdentity4x4();

	/// <summary>
	/// より大きい値を返す
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	static float Mymax(float a, float b);

	/// <summary>
	/// より小さい値を返す
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	static float Mymin(float a, float b);

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="s1">球1</param>
	/// <param name="s2">球2</param>
	/// <returns></returns>
	//static bool IsCollision(const Sphere& s1, const Sphere& s2);

	/// <summary>
	/// 球と平面の当たり判定
	/// </summary>
	/// <param name="s1">球</param>
	/// <param name="plane">平面</param>
	/// <returns></returns>
	//static bool IsCollision(const Sphere& s1, const Plane& plane);

	/// <summary>
	/// 線と平面
	/// </summary>
	/// <param name="plane"></param>
	/// <param name="segment"></param>
	/// <param name="line"></param>
	/// <param name="ray"></param>
	/// <returns></returns>
	//static bool IsCollision(const Plane& plane, Segment* segment = nullptr, Line* line = nullptr, Ray* ray = nullptr);

	/// <summary>
	/// 線分と平面の当たり判定
	/// </summary>
	/// <param name="line">線分</param>
	/// <param name="plane">平面</param>
	/// <returns></returns>
	//static bool IsCollision(const Segment& line, const Plane& plane);

	/// <summary>
	/// 半直線と平面の当たり判定
	/// </summary>
	/// <param name="line">半直線</param>
	/// <param name="plane">平面</param>
	/// <returns></returns>
	//static bool IsCollision(const Ray& line, const Plane& plane);

	/// <summary>
	/// 直線と平面の当たり判定
	/// </summary>
	/// <param name="line">直線</param>
	/// <param name="plane">平面</param>
	/// <returns></returns>
	//static bool IsCollision(const Line& line, const Plane& plane);

	/// <summary>
	/// 三角形と線分の当たり判定
	/// </summary>
	/// <param name="triangle">三角形</param>
	/// <param name="segment">線分</param>
	/// <returns></returns>
	//static bool IsCollision(const Triangle& triangle, const Segment& segment);

	/// <summary>
	/// AABBとAABBの当たり判定
	/// </summary>
	/// <param name="aabb1">AABB1</param>
	/// <param name="aabb2">AABB2</param>
	/// <returns></returns>
	//static bool IsCollision(const AABB& aabb1, const AABB& aabb2);


	/// <summary>
	/// AABBと球の当たり判定
	/// </summary>
	/// <param name="aabb">AABB</param>
	/// <param name="sphere">球</param>
	/// <returns></returns>
	//static bool IsCollision(const AABB& aabb, const Sphere& sphere);


	/// <summary>
	///  AABBと線分の当たり判定
	/// </summary>
	/// <param name="aabb">AABB</param>
	/// <param name="segment">線分</param>
	/// <returns></returns>
	//static bool IsCollision(const AABB& aabb, const Segment& segment);

	/// <summary>
	///  AABBと直線の当たり判定
	/// </summary>
	/// <param name="aabb">AABB</param>
	/// <param name="line">直線</param>
	/// <returns></returns>
	//static bool IsCollision(const AABB& aabb, const Line& line);

	/// <summary>
	///  AABBと半直線の当たり判定
	/// </summary>
	/// <param name="aabb">AABB</param>
	/// <param name="ray">半直線</param>
	/// <returns></returns>
	//static bool IsCollision(const AABB& aabb, const Ray& ray);

	static float lerp(float t, float start, float end);


	static float EaseInQuadF(float t, float start, float end);
	static float EaseOutQuadF(float t, float start, float end);
	static float EaseInOutQuadF(float t, float start, float end);


	static float EaseInCubicF(float t, float start, float end);
	static float EaseOutCubicF(float t, float start, float end);
	static float EaseInOutCubicF(float t, float start, float end);

};