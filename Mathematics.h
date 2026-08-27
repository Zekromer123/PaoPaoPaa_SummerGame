#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;
struct AABB {
	Vector3 min;
	Vector3 max;
};
class Mathematics {
public:
	static inline const float kPi = 3.14159265f;
	static float DegreesToRadians(float degrees) { return degrees * (kPi / 180.0f); }
	static float RadiansToDegrees(float radians) { return radians * (180.0f / kPi); }
	static Matrix4x4 MakeIdentity();
	static Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2);
	static Matrix4x4 MakeTranslateMatrix(Vector3 translate);
	static Matrix4x4 MakeRotateXMatrix(float theta);
	static Matrix4x4 MakeRotateYMatrix(float theta);
	static Matrix4x4 MakeRotateZMatrix(float theta);
	static Matrix4x4 MakeScaleMatrix(Vector3 scale);
	static Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate);
	static bool IsCollision(const AABB& aabb1, const AABB& aabb2);
	static Vector3 Add(const Vector3& v1, const Vector3& v2);
	static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
	static float EaseIn(float start, float end, float t) { return start + (end - start) * t * t; }
	static float EaseOut(float start, float end, float t) { return start + (end - start) * (1.0f - (1.0f - t) * (1.0f - t)); }

	static Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

};
Vector3& operator+=(Vector3& v1, const Vector3& v2);
Vector3& operator-=(Vector3& v1, const Vector3& v2);
Vector3 operator+(const Vector3& v1, const Vector3& v2);
Vector3 operator-(const Vector3& v1, const Vector3& v2);