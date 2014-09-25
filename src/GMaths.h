#pragma once
#include "Utilities.h"

#if defined(_vita_) || defined(_PS3_)
class vector2{
public:
	vector2 operator* (float f){ return vector2(x*f, y*f); }
	vector2 operator+= (vector2 f){ x += f.x; y += f.y; return vector2(x, y); }
	vector2 operator-= (vector2 f){ x -= f.x; y -= f.y; return vector2(x, y); }
	vector2 operator+ (vector2 f){ return vector2(x + f.x, y + f.y); }
	vector2 operator- (vector2 f){ return vector2(x - f.x, y - f.y); }
	float x, y;
	vector2(){
		x = 0;
		y = 0;
	};

	vector2(float Xv, float Yv)
	{
		x = Xv;
		y = Yv;
	}
};
#endif

#if defined(_vita_)
	#include <vectormath.h>
	#define matrix4 sce::Vectormath::Simd::Aos::Matrix4
	#define vector3 sce::Vectormath::Simd::Aos::Vector3
	#define matrix3 sce::Vectormath::Simd::Aos::Matrix3
	#define vector4 sce::Vectormath::Simd::Aos::Vector4
	#define point3 sce::Vectormath::Simd::Aos::Point3
	#ifndef M_PI
		#define M_PI 3.14159265358979323846
	#endif
#elif defined(_PS3_)
	#include <vectormath/cpp/vectormath_aos.h>
	//using namespace Vectormath::Aos;
	#define matrix4 Vectormath::Aos::Matrix4
	#define vector3 Vectormath::Aos::Vector3
	#define matrix3 Vectormath::Aos::Matrix3
	#define vector4 Vectormath::Aos::Vector4
	#define point3 Vectormath::Aos::Point3
	#ifndef FLT_MIN
		#define FLT_MIN 10^(-37)
	#endif
	#ifndef FLT_MAX
		#define FLT_MAX 10^37
	#endif

#elif defined(_XBOX360_) || defined(_WINDOWS_)
	#define _USE_MATH_DEFINES
	#define GLM_FORCE_RADIANS
	#include <cmath>
	#include "glm\glm.hpp"
	//#define uint32_t unsigned int
	//#define uint16_t unsigned int
	//TODO implement generic methods to change values in vectors
	#define matrix4 glm::mat4
	#define matrix3 glm::mat3
	#define vector4 glm::vec4
	#define vector3 glm::vec3
	#define vector2 glm::vec2
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


namespace M4{
	//
	matrix4 rotation(const float angle, const vector3 axis);
	matrix4 rotation(const vector3 rotation);
	matrix4 translation(const vector3 translationVector);
	matrix4 scale(const vector3 scaleVector);
	//
	void transform();
	void rotate();
	//
	matrix4 lookat(const vector3 eyePos, const vector3 targetPos, const vector3 UpVector);
	matrix4 perspective(const float FoV, const float ratio,const float nearClip, const float farClip );
	matrix4 identity();
};

namespace SPACIAL{
	vector2 getFrustrumBoundsAtDepth(float fov, float ratio, float distance);
	vector2 getFrustrumBoundsAtDepth(float depth, float farX, float farY, float farZ, float nearX, float nearY);
	bool withinFrustrum(vector3 position);
}

namespace M3{
	vector3 cross(const vector3 a, const vector3 b);
}

namespace M2{
	float dot(const vector2 a, const vector2 b);
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