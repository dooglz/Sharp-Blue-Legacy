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

	vector3 Transform(const matrix4& mat, const vector3& p)
	{
		glm::vec4 v4(p, 0);
		glm::vec4 t = mat * v4;
		return vector3(t.x, t.y, t.z);
		/*
		vector3 v1 = vector3(
			mat[0] * p.x + mat[4] * p.y + mat[8] * p.z,
			mat[1] * p.x + mat[5] * p.y + mat[9] * p.z,
			mat[2] * p.x + mat[6] * p.y + mat[10] * p.z
			);
		vector3 v2 = vector3(1.0f*mat[12], 1.0f*mat[13], 1.0f* mat[14]);
		return v1 + v2;*/
	}
	vector3 GetTranslation(const matrix4& mat)
	{
		return vector3(mat[12][0], mat[13][0], mat[14][0]);
	}

}

namespace QU{
	Quaternion FromAxisAngle(const vector3& v, float angle)
	{
		angle *= 0.5f;
		float sinAngle = sin(angle);

		vector3 normVector = Normalize(v);
		return Quaternion(normVector.x*sinAngle,
			normVector.y*sinAngle,
			normVector.z*sinAngle,
			cos(angle));
	}
	float ToAxisAngle(const Quaternion& q, vector3& v)
	{
		// The quaternion representing the rotation is
		//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

		float sqrLength = q.x*q.x + q.y*q.y + q.z*q.z;
		if (sqrLength > 0.0f)
		{
			float invLength = 1.0f / sqrt(sqrLength);

			v.x = q.x*invLength;
			v.y = q.y*invLength;
			v.z = q.z*invLength;

			return 2.f*acos(q.w);
		}
		else
		{
			// angle is 0 (mod 2*pi), so any axis will do.
			v.x = 1.0f;
			v.y = 0.0f;
			v.z = 0.0f;

			return 0.f;
		}
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