#pragma once
#include "Utilities.h"

unsigned int ColorRamp(float t);
float lerp(float a, float b, float t);

#if defined(_PC_)
#define _USE_MATH_DEFINES
#define GLM_FORCE_RADIANS
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtx/norm.hpp"
// TODO implement generic methods to change values in vectors
#define Rotate(a, b) b* a
#define Matrix4 glm::mat4
#define Matrix3 glm::mat3
#define Vector4 glm::vec4
#define Vector3 glm::vec3
#define Vector2 glm::vec2
#define identity tquat
#define Quaternion glm::quat
#define Length glm::length
#define Distance glm::distance
#define Distance2 glm::distance2
#define Transpose glm::transpose
#define Dot glm::dot
#define Cross glm::cross
#define Perspective glm::perspective
#define Inverse glm::inverse
#define Lookat glm::lookAt
// HAX
#define getX() x
#define getY() y
#define getZ() z
#define getW() w
#define setX(a) x = a;
#define setY(a) y = a;
#define setZ(a) z = a;
#define setW(a) w = a;
#elif defined(_PS3_)
#include <vectormath/cpp/vectormath_aos.h>
#include <stdlib.h>

#define Matrix4 Vectormath::Aos::Matrix4
#define Matrix3 Vectormath::Aos::Matrix3
#define Vector2 Vectormath::Aos::Vector2
#define Vector3 Vectormath::Aos::Vector3
#define Vector4 Vectormath::Aos::Vector4
#define Point3 Vectormath::Aos::Point3
#define Quaternion Vectormath::Aos::Quat
//
#define Rotate Vectormath::Aos::rotate
#define Perspective Matrix4::perspective
#define Length Vectormath::Aos::length
#define Distance Vectormath::Aos::distance
#define Distance2(a, b) Vectormath::Aos::distSqr(Point3(a), Point3(b))
#define Transpose Vectormath::Aos::transpose
#define Dot Vectormath::Aos::dot
#define Cross Vectormath::Aos::cross

Matrix4 Inverse(const Matrix4 m);
Matrix3 Inverse(const Matrix3 m);

// TODO check if this is right
Quaternion Inverse(const Quaternion q);

#define Lookat Vectormath::Aos::Matrix4::lookAt
//#define .getXYZ() .getXYZ()
//#define isnan isNaN
//#define rand rand_int32_TT800

#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void EulerToAngleAxis(const Vector3 euler, Vector3& axis, float& angle);
void QuatToAngleAxis(const Quaternion q, Vector3& axis, float& angle);
void MatrixToAngleAxis(const Matrix4 m, Vector3& axis, float& angle);

Vector3 QuatToEuler(const Quaternion q);
Vector3 AngleAxisToEuler(const Vector3 axis, const float angle);
Vector3 MatrixToEuler(const Matrix4 m);
Vector3 GetTransaltion(const Matrix4 m);

Quaternion EulerToQuat(const Vector3 euler);
Quaternion AngleAxisToQuat(const Vector3 axis, const float angle);
Quaternion MatrixToQuat(const Matrix4 m);

Matrix4 EulerToMatrix(const Vector3 euler);
Matrix4 QuatToMatrix(const Quaternion q);
Matrix3 QuatToMatrix3(const Quaternion q);
Matrix4 AngleAxisToMatrix(const Vector3 axis, const float angle);

Matrix4 Translation(const Vector3 translationVector);
Matrix4 Scale(const Vector3 scaleVector);

Vector3 Normalize(Vector3 v);
Vector4 Normalize(Vector4 v);
Quaternion Normalize(Quaternion q);

Vector3 GetForwardVector(Quaternion q);
Vector3 GetUpVector(Quaternion q);
Vector3 GetRightVector(Quaternion q);

Vector3 V4toV3(Vector4 v);

float Clamp(const float a, const float max);
bool IsIdentical(const Vector3& a, const Vector3& b);
bool IsIdentical(const Quaternion& a, const Quaternion& b);

Vector4 Column(const Matrix4& m, const int a);
void print(Vector3 v);
void print(const Vector4& v);
void print(const Quaternion& v);
void print(const Matrix3& v);

void print(const Matrix4& v);

Matrix4 lookat(const Vector3 eyePos, const Vector3 targetPos,
  const Vector3 UpVector);

inline int randomColor() {
  int x = rand() & 0xff;
  x |= (rand() & 0xff) << 8;
  x |= (rand() & 0xff) << 16;
  x |= (0xff) << 24;

  return x;
}