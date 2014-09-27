#pragma once
#include "Utilities.h"

#if defined(_vita_) || defined(_PS3_)
class Vector2{
public:
	Vector2 operator* (float f){ return Vector2(x*f, y*f); }
	Vector2 operator+= (Vector2 f){ x += f.x; y += f.y; return Vector2(x, y); }
	Vector2 operator-= (Vector2 f){ x -= f.x; y -= f.y; return Vector2(x, y); }
	Vector2 operator+ (Vector2 f){ return Vector2(x + f.x, y + f.y); }
	Vector2 operator- (Vector2 f){ return Vector2(x - f.x, y - f.y); }
	float x, y;
	Vector2(){
		x = 0;
		y = 0;
	};

	Vector2(float Xv, float Yv)
	{
		x = Xv;
		y = Yv;
	}
};
#endif

#if defined(_vita_)
	#include <vectormath.h>
	#define Matrix4 sce::Vectormath::Simd::Aos::Matrix4
	#define Vector3 sce::Vectormath::Simd::Aos::Vector3
	#define matrix3 sce::Vectormath::Simd::Aos::Matrix3
	#define vector4 sce::Vectormath::Simd::Aos::Vector4
	#define point3 sce::Vectormath::Simd::Aos::Point3
	#ifndef M_PI
		#define M_PI 3.14159265358979323846
	#endif
#elif defined(_PS3_)
	#include <vectormath/cpp/vectormath_aos.h>
	//using namespace Vectormath::Aos;
	#define Matrix4 Vectormath::Aos::Matrix4
	#define Vector3 Vectormath::Aos::Vector3
	#define matrix3 Vectormath::Aos::Matrix3
	#define vector4 Vectormath::Aos::Vector4
	#define point3 Vectormath::Aos::Point3
	#ifndef FLT_MIN
		#define FLT_MIN 10^(-37)
	#endif
	#ifndef FLT_MAX
		#define FLT_MAX 10^37
	#endif

#elif defined(_XBOX360_) || defined(_PC_)
	#define _USE_MATH_DEFINES
	#define GLM_FORCE_RADIANS
	#include <cmath>
	#include "glm\glm.hpp"
	#include "glm/gtc/quaternion.hpp"
	//TODO implement generic methods to change values in vectors
	#define Matrix4 glm::mat4
	#define Matrix3 glm::mat3
	#define Vector4 glm::vec4
	#define Vector3 glm::vec3
	#define Vector2 glm::vec2
	#define Quaternion glm::quat
	#define Normalize glm::normalize
	#define Dot glm::dot
	#ifndef M_PI
		#define M_PI 3.14159265358979323846
	#endif
	//HAX
	#define getX() x
	#define getY() y
	#define getZ() z
	#define setX(a) x=a;
	#define setY(a) y=a;
	#define setZ(a) z=a;
#endif


void EulerToAngleAxis(const Vector3 euler, Vector3 &axis, float &angle);
void QuatToAngleAxis(const Quaternion q, Vector3 &axis, float &angle);
void MatrixToAngleAxis(const Matrix4 m, Vector3 &axis, float &angle);

Vector3 QuatToEuler(const Quaternion q);
Vector3 AngleAxisToEuler(const Vector3 axis, const float angle);
Vector3 MatrixToEuler(const Matrix4 m);

Quaternion EulerToQuat(const Vector3 euler);
Quaternion AngleAxisToQuat(const Vector3 axis, const float angle);
Quaternion MatrixToQuat(const Matrix4 m);

Matrix4 EulerToMatrix(const Vector3 euler);
Matrix4 QuatToMatrix(const Quaternion q);
Matrix4 AngleAxisToMatrix(const Vector3 axis, const float angle);


namespace M4{
	//
	Matrix4 rotation(const float angle, const Vector3 axis);
	Matrix4 rotation(const Vector3 rotation);
	Matrix4 translation(const Vector3 translationVector);
	Matrix4 scale(const Vector3 scaleVector);
	//
	void transform();
	void rotate();
	//
	Matrix4 lookat(const Vector3 eyePos, const Vector3 targetPos, const Vector3 UpVector);
	Matrix4 perspective(const float FoV, const float ratio,const float nearClip, const float farClip );
	Matrix4 identity();

	Vector3 Transform(const Matrix4& mat, const Vector3& p);
	Vector3 GetTranslation(const Matrix4& mat);
};

namespace SPACIAL{
	Vector2 getFrustrumBoundsAtDepth(float fov, float ratio, float distance);
	Vector2 getFrustrumBoundsAtDepth(float depth, float farX, float farY, float farZ, float nearX, float nearY);
	bool withinFrustrum(Vector3 position);
}

namespace M3{
	Vector3 cross(const Vector3 a, const Vector3 b);
}

namespace M2{
	float dot(const Vector2 a, const Vector2 b);
}

namespace QU{
	Quaternion FromAxisAngle(const Vector3& v, float angle);
	float ToAxisAngle(const Quaternion& q, Vector3& v);
}


struct stVertex 
{
	float x, y, z;
	unsigned int rgba ;
};

struct stvec3 
{
	float x, y, z;
};
struct stvec2 
{
	float x, y;
};

inline 
float lerp(float a, float b, float t)
{
    return a + t*(b-a);
}

// create a color ramp
inline
unsigned int ColorRamp( float t )
{
	const int ncolors = 7;
	float c[ncolors][3] =
	{
		{ 1.0, 0.0, 0.0, },
		{ 1.0, 0.5, 0.0, },
		{ 1.0, 1.0, 0.0, },
		{ 0.0, 1.0, 0.0, },
		{ 0.0, 1.0, 1.0, },
		{ 0.0, 0.0, 1.0, },
		{ 1.0, 0.0, 1.0, },
	};
	t = t * (ncolors-1);
	int i = (int) t;
	float u = t - floor(t);

	unsigned char r = (unsigned char)(lerp(c[i][0], c[i+1][0], u) * 255);
	unsigned char g = (unsigned char)(lerp(c[i][1], c[i+1][1], u) * 255);
	unsigned char b = (unsigned char)(lerp(c[i][2], c[i+1][2], u) * 255);
	unsigned int col = ((r <<0)|(g <<8)|(b <<16)|(255 <<24));
	return col;
}

inline int randomColor()
{
	int x = rand() & 0xff;
	x |= (rand() & 0xff) << 8;
	x |= (rand() & 0xff) << 16;
	x |= (rand() & 0xff) << 24;

	return x;
}