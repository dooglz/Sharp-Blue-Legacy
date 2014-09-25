#include "GMaths.h"

#if defined(_PS3_)

#elif defined(_XBOX360_) || defined(_WINDOWS_)
	#include "glm\gtc\matrix_transform.hpp"
#endif

#if defined(_vita_)
namespace M4{
	//
	matrix4 rotation(const float angle, const vector3 axis)
	{
		return matrix4::rotation(angle*(M_PI/180), axis);
	}
	matrix4 rotation(const vector3 rotation)
	{
		matrix4 a = matrix4::rotation(float(rotation.getX()*(M_PI / 180.0f)), vector3(1, 0, 0));
		a *= matrix4::rotation(float(rotation.getY()*(M_PI / 180.0f)), vector3(0, 1, 0));
		a *= matrix4::rotation(float(rotation.getZ()*(M_PI / 180.0f)), vector3(0, 0, 1));
		return a;
	}

	matrix4 translation(const vector3 translationVector)
	{
		return matrix4::translation(translationVector);
	}
	matrix4 scale(const vector3 scaleVector)
	{
		return matrix4::scale(scaleVector);
	}
	//
	void transform(){};
	void rotate(){};
	//
	matrix4 lookat(const vector3 eyePos, const vector3 targetPos, const vector3 UpVector)
	{
		return matrix4::lookAt(
			point3(eyePos.getX(),eyePos.getY(),eyePos.getZ()), 
			point3(targetPos.getX(),targetPos.getY(),targetPos.getZ()),
			UpVector
			);
	}

	matrix4 perspective(const float FoV, const float ratio,const float nearClip, const float farClip )
	{
		return matrix4::perspective(FoV*(M_PI/180), ratio, nearClip, farClip);
	}
}

namespace M3{
	vector3 cross(const vector3 a, const vector3 b){
		return  sce::Vectormath::Simd::Aos::cross(a, b);
	}
}
#elif defined(_PS3_)

namespace M4{
	//
	matrix4 rotation(const float angle, const vector3 axis)
	{
		return matrix4::rotation(angle*(M_PI/180), axis);
	}
	matrix4 rotation(const vector3 rotation)
	{
		matrix4 a = matrix4::rotation(float(rotation.getX()*(M_PI / 180.0f)), vector3(1, 0, 0));
		a *= matrix4::rotation(float(rotation.getY()*(M_PI / 180.0f)), vector3(0, 1, 0));
		a *= matrix4::rotation(float(rotation.getZ()*(M_PI / 180.0f)), vector3(0, 0, 1));
		return a;
	}

	matrix4 translation(const vector3 translationVector)
	{
		return matrix4::translation(translationVector);
	}
	matrix4 scale(const vector3 scaleVector)
	{
		return matrix4::scale(scaleVector);
	}
	//
	void transform(){};
	void rotate(){};
	//
	matrix4 lookat(const vector3 eyePos, const vector3 targetPos, const vector3 UpVector)
	{
		return matrix4::lookAt(
			point3(eyePos.getX(),eyePos.getY(),eyePos.getZ()), 
			point3(targetPos.getX(),targetPos.getY(),targetPos.getZ()),
			UpVector
			);
	}

	matrix4 perspective(const float FoV, const float ratio,const float nearClip, const float farClip )
	{
		return matrix4::perspective(FoV*(M_PI/180), ratio, nearClip, farClip);
	}
}

namespace M3{
	vector3 cross(const vector3 a, const vector3 b){
		return Vectormath::Aos::cross(a, b);
	}
}

#elif defined(_XBOX360_) || defined(_WINDOWS_)

namespace M4{
	//
	matrix4 rotation(const float angle, const vector3 axis)
	{
		return glm::rotate(glm::mat4(1.0f), float(angle*(M_PI/180.0f)), axis);
	}
	matrix4 rotation(const vector3 rotation)
	{
		matrix4 a = glm::rotate(glm::mat4(1.0f), float(rotation.x*(M_PI / 180.0f)), vector3(1, 0, 0));
		a *= glm::rotate(glm::mat4(1.0f), float(rotation.y*(M_PI / 180.0f)), vector3(0, 1, 0));
		a *= glm::rotate(glm::mat4(1.0f), float(rotation.z*(M_PI / 180.0f)), vector3(0, 0, 1));
		return a;
	}

	matrix4 translation(const vector3 translationVector)
	{
		return glm::translate(glm::mat4(1.0f), translationVector);
	}
	matrix4 scale(const vector3 scaleVector)
	{
		return glm::scale(glm::mat4(1.0f), scaleVector);
	}

	//
	void transform(){};
	void rotate(){};
	//

	matrix4 lookat(const vector3 eyePos, const vector3 targetPos, const vector3 UpVector)
	{
		return glm::lookAt(eyePos, targetPos, UpVector);
	}

	matrix4 perspective(const float FoV, const float ratio,const float nearClip, const float farClip )
	{
		return glm::perspective(float(FoV*(M_PI/180.0f)), ratio, nearClip, farClip);
	}

	matrix4 identity(){
		return glm::mat4(1.0f);
	}
}

namespace M3{
	vector3 cross(const vector3 a, const vector3 b){
		return glm::cross(a, b);
	}
}

#endif

namespace SPACIAL{
	vector2 getFrustrumBoundsAtDepth(float fov, float ratio, float distance){
		float tang = (float)tan((M_PI / 180.0f) * fov * 0.5);
		float X = distance  * tang;
		float Y = X * ratio;
		return vector2(Y, X);
	}
	vector2 getFrustrumBoundsAtDepth(float depth, float farX, float farY, float farZ, float nearX, float nearY){
		float A=0, X=0, Y=0;
		A = ((farZ - depth) / farZ);
		X = A*farX + ((1 - A)*nearX);
		Y = Y*farY + ((1 - A)*nearY); //posible error here
		return vector2(X, Y);
	}
	bool withinFrustrum(vector3 position){
		return true;
	}
}

namespace M2{
	float dot(const vector2 a, const vector2 b){
		return a.x * b.x + a.y * b.y;
	}
}