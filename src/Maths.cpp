#include "Maths.h"
#include "Utilities.h"
#include <stdio.h>

#if defined(_PC_)
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtc/matrix_transform.hpp"

void print(Vector3 v) { printf("V3(%f,%f,%f)\n", v.x, v.y, v.z); }
void print(const Vector4& v) {
	printf("V4(%f,%f,%f,%f)\n", v.x, v.y, v.z, v.w);
}
void print(const Quaternion& v) {
	printf("Qu(%f,%f,%f,%f)\n", v.x, v.y, v.z, v.w);
}

void print(const Matrix3& v) {
	printf("M3(%f,%f,%f\n%f,%f,%f\n%f,%f,%f)\n", v[0][0], v[0][1], v[0][2],
		v[1][0], v[1][1], v[1][2], v[2][0], v[2][1], v[2][2]);
}

void print(const Matrix4& v) {
	printf("M4(%f,%f,%f,%f\n%f,%f,%f,%f\n,%f,%f,%f,%f\n,%f,%f,%f,%f)\n", v[0][0],
		v[0][1], v[0][2], v[0][3], v[1][0], v[1][1], v[1][2], v[1][3], v[2][0],
		v[2][1], v[2][2], v[2][3], v[3][0], v[3][1], v[3][2], v[3][3]);
}

void EulerToAngleAxis(const Vector3 euler, Vector3& axis, float& angle) {
	Quaternion q = EulerToQuat(euler);
	axis = glm::axis(q);
	angle = glm::angle(q);
}

void QuatToAngleAxis(const Quaternion q, Vector3& axis, float& angle) {
	axis = glm::axis(q);
	angle = glm::angle(q);
}
void MatrixToAngleAxis(const Matrix4 m, Vector3& axis, float& angle) {
	Quaternion q = MatrixToQuat(m);
	axis = glm::axis(q);
	angle = glm::angle(q);
}

//--
Vector3 Normalize(Vector3 v) {
	v = glm::normalize(v);
	if (isnan(v.x) || isnan(v.y) || isnan(v.z)) {
		return Vector3(0, 0, 0);
	}
	return v;
}
Vector4 Normalize(Vector4 v) {
	v = glm::normalize(v);
	if (isnan(v.x) || isnan(v.y) || isnan(v.z)) {
		return Vector4(0, 0, 0, 0);
	}
	return v;
}

Quaternion Normalize(Quaternion q) {
	q = glm::normalize(q);
	if (isnan(q.x) || isnan(q.y) || isnan(q.z) || isnan(q.w)) {
		return Quaternion(0, 0, 0, 0);
	}
	return q;
}

Vector3 QuatToEuler(const Quaternion q) { return glm::eulerAngles(q); }

Vector3 AngleAxisToEuler(const Vector3 axis, const float angle) {
	return QuatToEuler(AngleAxisToQuat(axis, angle));
}
// only translate

Vector3 MatrixToEuler(const Matrix4 m) { return QuatToEuler(MatrixToQuat(m)); }

Vector3 GetTransaltion(const Matrix4 m) { return Vector3(m[3]); }

//--
Quaternion EulerToQuat(const Vector3 euler) { return Quaternion(euler); }

Quaternion AngleAxisToQuat(const Vector3 axis, const float angle) {
	return glm::angleAxis(angle, axis);
}

Quaternion MatrixToQuat(const Matrix4 m) { return glm::quat_cast(m); }

//--
Matrix4 EulerToMatrix(const Vector3 euler) {
	return glm::eulerAngleYXZ(euler.y, euler.x, euler.z);
}

Matrix4 QuatToMatrix(const Quaternion q) { return glm::mat4_cast(q); }

Matrix3 QuatToMatrix3(const Quaternion q) { return glm::mat3_cast(q); }

// Angle in Radians
Matrix4 AngleAxisToMatrix(const Vector3 axis, const float angle) {
	return glm::rotate(glm::mat4(1.0f), angle, axis);
}

bool isZero(const Vector3 v3) {
	return (abs(v3.x) == 0 && abs(v3.y) == 0 && abs(v3.z) == 0);
}

Matrix4 Translation(const Vector3 translationVector) {
	return glm::translate(glm::mat4(1.0f), translationVector);
}

Matrix4 Scale(const Vector3 scaleVector) {
	return glm::scale(glm::mat4(1.0f), scaleVector);
}

Vector3 V4toV3(Vector4 v) { return Vector3(v); }

Vector4 Column(const Matrix4& m, const int a) { return glm::column(m, a); }

Matrix4 lookat(const Vector3 eyePos, const Vector3 targetPos,
	const Vector3 UpVector) {
	return Matrix4(1.0f);
}

#elif defined(_PS3_)

void print(Vector3 v) {
	printf("V3(%f,%f,%f)\n", v.getX().getAsFloat(), v.getY().getAsFloat(),
		v.getZ().getAsFloat());
}
void print(const Vector4& v) {
	printf("V4(%f,%f,%f,%f)\n", v.getX().getAsFloat(), v.getY().getAsFloat(),
		v.getZ().getAsFloat(), v.getW().getAsFloat());
}
void print(const Quaternion& v) {
	printf("Qu(%f,%f,%f,%f)\n", v.getX().getAsFloat(), v.getY().getAsFloat(),
		v.getZ().getAsFloat(), v.getW().getAsFloat());
}

void print(const Matrix3& v) {
	printf("M3(%f,%f,%f\n%f,%f,%f\n%f,%f,%f)\n", v.getElem(0, 0).getAsFloat(),
		v.getElem(0, 1).getAsFloat(), v.getElem(0, 2).getAsFloat(),
		v.getElem(1, 0).getAsFloat(), v.getElem(1, 1).getAsFloat(),
		v.getElem(1, 2).getAsFloat(), v.getElem(2, 0).getAsFloat(),
		v.getElem(2, 1).getAsFloat(), v.getElem(2, 2).getAsFloat());
}

void print(const Matrix4& v) {
	printf("M4(%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f)\n",
		v.getElem(0, 0).getAsFloat(), v.getElem(0, 1).getAsFloat(),
		v.getElem(0, 2).getAsFloat(), v.getElem(0, 3).getAsFloat(),
		v.getElem(1, 0).getAsFloat(), v.getElem(1, 1).getAsFloat(),
		v.getElem(1, 2).getAsFloat(), v.getElem(1, 3).getAsFloat(),
		v.getElem(2, 0).getAsFloat(), v.getElem(2, 1).getAsFloat(),
		v.getElem(2, 2).getAsFloat(), v.getElem(2, 3).getAsFloat(),
		v.getElem(3, 0).getAsFloat(), v.getElem(3, 1).getAsFloat(),
		v.getElem(3, 2).getAsFloat(), v.getElem(3, 3).getAsFloat());
}

void EulerToAngleAxis(const Vector3 euler, Vector3& axis, float& angle) {
	Quaternion q = EulerToQuat(euler);
	QuatToAngleAxis(q, axis, angle);
}

void QuatToAngleAxis(const Quaternion q, Vector3& axis, float& angle) {
	Quaternion q1 = q;
	if (q1.getW() > 1) {
		Normalize(q1);
	};  // if w>1 acos and sqrt will produce errors, this cant happen if
	// quaternion is normalised
	angle = 2 * acos(q1.getW());
	double s = sqrt(1 - q1.getW() * q1.getW());  // assuming quaternion normalised
	// then w is less than 1, so term
	// always positive.
	if (s <
		0.001)  // test to avoid divide by zero, s is always positive due to sqrt
	{
		// if s close to zero then direction of axis not important
		axis.setX(q1.getX());  // if it is important that axis is normalised then
		// replace with x=1; y=z=0;
		axis.setY(q1.getY());
		axis.setZ(q1.getZ());
	}
	else {
		axis.setX(q1.getX() / s);  // normalise axis
		axis.setY(q1.getY() / s);
		axis.setZ(q1.getZ() / s);
	}
}
void MatrixToAngleAxis(const Matrix4 m, Vector3& axis, float& angle) {
	Quaternion q = MatrixToQuat(m);
	QuatToAngleAxis(q, axis, angle);
}

//--
Vector3 Normalize(Vector3 v) {
	v = Vectormath::Aos::normalize(v);
	if (isnan(v.getX()) || isnan(v.getY()) || isnan(v.getZ())) {
		return Vector3(0, 0, 0);
	}
	return v;
}
Vector4 Normalize(Vector4 v) {
	v = Vectormath::Aos::normalize(v);
	if (isnan(v.getX()) || isnan(v.getY()) || isnan(v.getZ()) ||
		isnan(v.getW())) {
		return Vector4(0, 0, 0, 0);
	}
	return v;
}

Quaternion Normalize(Quaternion q) {
	q = Vectormath::Aos::normalize(q);
	if (isnan(q.getX()) || isnan(q.getY()) || isnan(q.getZ()) ||
		isnan(q.getW())) {
		return Quaternion(0, 0, 0, 0);
	}
	return q;
}

Vector3 QuatToEuler(const Quaternion q) {
	// This isn't right
	Quaternion q1 = Normalize(q);
	Vector3 v;
	double test = q1.getX() * q1.getY() + q1.getZ() * q1.getW();
	if (test > 0.499) {  // singularity at north pole
		v.setX(2 * atan2(q1.getX(), q1.getW()));
		v.setY(M_PI / 2);
		v.setZ(0);
		return v;
	}
	else if (test < -0.499) {  // singularity at south pole
		v.setX(-2 * atan2(q1.getX(), q1.getW()));
		v.setY(-M_PI / 2);
		v.setZ(0);
		return v;
	}
	float sqx = q1.getX() * q1.getX();
	float sqy = q1.getY() * q1.getY();
	float sqz = q1.getZ() * q1.getZ();
	v.setX(atan2(2 * q1.getY() * q1.getW() - 2 * q1.getX() * q1.getZ(),
		1 - 2 * sqy - 2 * sqz));
	v.setY(asin(2 * test));
	v.setZ(atan2(2 * q1.getX() * q1.getW() - 2 * q1.getY() * q1.getZ(),
		1 - 2 * sqx - 2 * sqz));
	return v;
}

Vector3 AngleAxisToEuler(const Vector3 axis, const float angle) {
	return QuatToEuler(AngleAxisToQuat(axis, angle));
}
// only translate

Vector3 MatrixToEuler(const Matrix4 m) { return QuatToEuler(MatrixToQuat(m)); }

Vector3 GetTransaltion(const Matrix4 m) {
	return Vector3(m.getCol3().getXYZ());
}

//--
Quaternion EulerToQuat(const Vector3 euler) {
	Quaternion q;
	Vector3 c = Vector3(cos(euler.getX() * 0.5f), cos(euler.getY() * 0.5f),
		cos(euler.getZ() * 0.5f));
	Vector3 s = Vector3(sin(euler.getX() * 0.5f), sin(euler.getY() * 0.5f),
		sin(euler.getZ() * 0.5f));
	q.setW(c.getX() * c.getY() * c.getZ() + s.getX() * s.getY() * s.getZ());
	q.setX(s.getX() * c.getY() * c.getZ() - c.getX() * s.getY() * s.getZ());
	q.setY(c.getX() * s.getY() * c.getZ() + s.getX() * c.getY() * s.getZ());
	q.setZ(c.getX() * c.getY() * s.getZ() - s.getX() * s.getY() * c.getZ());
	return q;
}

Quaternion AngleAxisToQuat(const Vector3 axis, const float angle) {
	const double s = sin(angle / 2);
	return Quaternion(axis.getX() * s, axis.getY() * s, axis.getZ() * s,
		cos(angle / 2));
}

Quaternion MatrixToQuat(const Matrix4 m) { return Quaternion(m.getUpper3x3()); }

//--
Matrix4 EulerToMatrix(const Vector3 euler) {
	// TODO: is this right?
	Matrix4 m = Matrix4::identity();
	return m.rotationZYX(euler);
}

Matrix4 QuatToMatrix(const Quaternion q) {
	return Matrix4(q, Vector3(0, 0, 0));
}

Matrix3 QuatToMatrix3(const Quaternion q) { return Matrix3(q); }

// Angle in Radians
Matrix4 AngleAxisToMatrix(const Vector3 axis, const float angle) {
	// TODO: is this right?
	Matrix4 m = Matrix4::identity();
	return m.rotationZYX(axis * angle);
}

bool isZero(const Vector3 v3) {
	return (abs(v3.getX()) == 0 && abs(v3.getY()) == 0 && abs(v3.getZ()) == 0);
}

Matrix4 Translation(const Vector3 translationVector) {
	return Matrix4::translation(translationVector);
}

Matrix4 Scale(const Vector3 scaleVector) { return Matrix4::scale(scaleVector); }

Vector3 V4toV3(Vector4 v) { return v.getXYZ(); }

Vector4 Column(const Matrix4& m, const int a) { return m.getCol(a); }

Matrix4 lookat(const Vector3 eyePos, const Vector3 targetPos,
	const Vector3 UpVector) {
	return Matrix4::lookAt(
		Point3(eyePos.getX(), eyePos.getY(), eyePos.getZ()),
		Point3(targetPos.getX(), targetPos.getY(), targetPos.getZ()), UpVector);
}

Matrix4 Inverse(const Matrix4 m) { return Vectormath::Aos::inverse(m); }
Matrix3 Inverse(const Matrix3 m) { return Vectormath::Aos::inverse(m); }
// TODO check if this is right
Quaternion Inverse(const Quaternion q) {
	return (Vectormath::Aos::conj(q) / dot(q, q));
}

#endif

Vector3 GetForwardVector(Quaternion q) {
	/*
	Matrix4 view = QuatToMatrix(q);
	Vector3 m_xaxis = glm::vec3(view[0][0], view[1][0], view[2][0]);
	Vector3 m_yaxis = glm::vec3(view[0][1], view[1][1], view[2][1]);
	Vector3 m_zaxis = Vector3(view[2][0], view[2][1], view[2][2]);
	return -m_zaxis;
	*/

	return Vector3(2.0f * (q.getX() * q.getZ() + q.getW() * q.getY()),
		2.0f * (q.getY() * q.getZ() - q.getW() * q.getX()),
		1.0f - 2.0f * (q.getX() * q.getX() + q.getY() * q.getY()));
}

Vector3 GetUpVector(Quaternion q) {
	return Vector3(2 * (q.getX() * q.getY() - q.getW() * q.getZ()),
		1.0f - 2.0f * (q.getX() * q.getX() + q.getZ() * q.getZ()),
		2.0f * (q.getY() * q.getZ() + q.getW() * q.getX()));
}

Vector3 GetRightVector(Quaternion q) {
	return Vector3(1.0f - 2.0f * (q.getY() * q.getY() + q.getZ() * q.getZ()),
		2.0f * (q.getX() * q.getY() + q.getW() * q.getZ()),
		2.0f * (q.getX() * q.getZ() - q.getW() * q.getY()));
}

float Clamp(const float a, const float max) {
	if (a > max) {
		return max;
	}
	return a;
}

bool IsIdentical(const Vector3& a, const Vector3& b) {
	return ((a.getX() == b.getX()) && (a.getY() == b.getY()) &&
		(a.getZ() == b.getZ()));
}

bool IsIdentical(const Quaternion& a, const Quaternion& b) {
	return ((a.getX() == b.getX()) && (a.getY() == b.getY()) &&
		(a.getZ() == b.getZ()) && (a.getW() == b.getW()));
}

float lerp(float a, float b, float t) { return a + t * (b - a); }

// create a color ramp
unsigned int ColorRamp(float t) {
	const int ncolors = 7;
	float c[ncolors][3] = {
		{
			1.0, 0.0, 0.0,
		},
		{
			1.0, 0.5, 0.0,
		},
		{
			1.0, 1.0, 0.0,
		},
		{
			0.0, 1.0, 0.0,
		},
		{
			0.0, 1.0, 1.0,
		},
		{
			0.0, 0.0, 1.0,
		},
		{
			1.0, 0.0, 1.0,
		},
	};
	t = t * (ncolors - 1);
	int i = (int)t;
	float u = t - floor(t);

	unsigned char r = (unsigned char)(lerp(c[i][0], c[i + 1][0], u) * 255);
	unsigned char g = (unsigned char)(lerp(c[i][1], c[i + 1][1], u) * 255);
	unsigned char b = (unsigned char)(lerp(c[i][2], c[i + 1][2], u) * 255);
	unsigned int col = ((r << 0) | (g << 8) | (b << 16) | (255 << 24));
	return col;
}