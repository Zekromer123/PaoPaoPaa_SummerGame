#include "Mathematics.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>


Matrix4x4 Mathematics::MakeIdentity() {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = (i == j) ? 1.0f : 0.0f;
		}
	}
	return result;
}
Matrix4x4 Mathematics::Multiply(Matrix4x4 m1, Matrix4x4 m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}
Matrix4x4 Mathematics::MakeTranslateMatrix(Vector3 translate) {
	Matrix4x4 result = MakeIdentity();
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	return result;
}

Matrix4x4 Mathematics::MakeRotateXMatrix(float theta) {
	Matrix4x4 result = MakeIdentity();
	float rad = theta * (float)M_PI / 180.0f;
	result.m[1][1] = cosf(rad);
	result.m[1][2] = sinf(rad);
	result.m[2][1] = -sinf(rad);
	result.m[2][2] = cosf(rad);
	return result;
}

Matrix4x4 Mathematics::MakeRotateYMatrix(float theta) {
	Matrix4x4 result = MakeIdentity();
	float rad = theta;
	result.m[0][0] = cosf(rad);
	result.m[0][2] = -sinf(rad);
	result.m[2][0] = sinf(rad);
	result.m[2][2] = cosf(rad);
	return result;
}

Matrix4x4 Mathematics::MakeRotateZMatrix(float theta) {
	Matrix4x4 result = MakeIdentity();
	float rad = theta * (float)M_PI / 180.0f;
	result.m[0][0] = cosf(rad);
	result.m[0][1] = sinf(rad);
	result.m[1][0] = -sinf(rad);
	result.m[1][1] = cosf(rad);
	return result;
}

Matrix4x4 Mathematics::MakeScaleMatrix(Vector3 scale) {
	Matrix4x4 result{};
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 Mathematics::MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate) {
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	Matrix4x4 result = Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));
	return result;
}

bool Mathematics::IsCollision(const AABB& aabb1, const AABB& aabb2) {
	return (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && // X
	       (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && // Y
	       (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z);
}

Vector3 Mathematics::Add(const Vector3& v1, const Vector3& v2) { return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; }

Vector3 Mathematics::Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3];
	if (w != 0.0f) {
		result.x /= w;
		result.y /= w;
		result.z /= w;
	}
	return result;
}

Vector3 Mathematics::TransformNormal(const Vector3& v, const Matrix4x4& m)
{ 
	Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]
	}; 
	return result;
}


Vector3& operator+=(Vector3& v1, const Vector3& v2) {
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	return v1;
}

Vector3& operator-=(Vector3& v1, const Vector3& v2) {
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
	return v1;
}
Vector3 operator+(const Vector3& v1, const Vector3& v2) { return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; }
Vector3 operator-(const Vector3& v1, const Vector3& v2) { return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z}; }