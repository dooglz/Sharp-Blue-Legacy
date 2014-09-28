#include "GMaths.h"

#if defined(_PS3_)

#elif defined(_XBOX360_) || defined(_PC_)
	#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
	#include "glm/gtx/euler_angles.hpp"
	#include "glm/gtc/matrix_transform.hpp"
#endif

#if defined(_vita_)
namespace M4{
	//
	Matrix4 rotation(const float angle, const Vector3 axis)
	{
		return Matrix4::rotation(angle*(M_PI/180), axis);
	}
	Matrix4 rotation(const Vector3 rotation)
	{
		Matrix4 a = Matrix4::rotation(float(rotation.getX()*(M_PI / 180.0f)), Vector3(1, 0, 0));
		a *= Matrix4::rotation(float(rotation.getY()*(M_PI / 180.0f)), Vector3(0, 1, 0));
		a *= Matrix4::rotation(float(rotation.getZ()*(M_PI / 180.0f)), Vector3(0, 0, 1));
		return a;
	}

	Matrix4 translation(const Vector3 translationVector)
	{
		return Matrix4::translation(translationVector);
	}
	Matrix4 scale(const Vector3 scaleVector)
	{
		return Matrix4::scale(scaleVector);
	}
	//
	void transform(){};
	void rotate(){};
	//
	Matrix4 lookat(const Vector3 eyePos, const Vector3 targetPos, const Vector3 UpVector)
	{
		return Matrix4::lookAt(
			point3(eyePos.getX(),eyePos.getY(),eyePos.getZ()), 
			point3(targetPos.getX(),targetPos.getY(),targetPos.getZ()),
			UpVector
			);
	}

	Matrix4 perspective(const float FoV, const float ratio,const float nearClip, const float farClip )
	{
		return Matrix4::perspective(FoV*(M_PI/180), ratio, nearClip, farClip);
	}
}

namespace M3{
	Vector3 cross(const Vector3 a, const Vector3 b){
		return  sce::Vectormath::Simd::Aos::cross(a, b);
	}
}
#elif defined(_PS3_)

namespace M4{
	//
	Matrix4 rotation(const float angle, const Vector3 axis)
	{
		return Matrix4::rotation(angle*(M_PI/180), axis);
	}
	Matrix4 rotation(const Vector3 rotation)
	{
		Matrix4 a = Matrix4::rotation(float(rotation.getX()*(M_PI / 180.0f)), Vector3(1, 0, 0));
		a *= Matrix4::rotation(float(rotation.getY()*(M_PI / 180.0f)), Vector3(0, 1, 0));
		a *= Matrix4::rotation(float(rotation.getZ()*(M_PI / 180.0f)), Vector3(0, 0, 1));
		return a;
	}

	Matrix4 translation(const Vector3 translationVector)
	{
		return Matrix4::translation(translationVector);
	}
	Matrix4 scale(const Vector3 scaleVector)
	{
		return Matrix4::scale(scaleVector);
	}
	//
	void transform(){};
	void rotate(){};
	//
	Matrix4 lookat(const Vector3 eyePos, const Vector3 targetPos, const Vector3 UpVector)
	{
		return Matrix4::lookAt(
			point3(eyePos.getX(),eyePos.getY(),eyePos.getZ()), 
			point3(targetPos.getX(),targetPos.getY(),targetPos.getZ()),
			UpVector
			);
	}

	Matrix4 perspective(const float FoV, const float ratio,const float nearClip, const float farClip )
	{
		return Matrix4::perspective(FoV*(M_PI/180), ratio, nearClip, farClip);
	}
}

namespace M3{
	Vector3 cross(const Vector3 a, const Vector3 b){
		return Vectormath::Aos::cross(a, b);
	}
}

#elif defined(_XBOX360_) || defined(_WINDOWS_)

namespace M4{
	//
	Matrix4 rotation(const float angle, const Vector3 axis)
	{
		return glm::rotate(glm::mat4(1.0f), float(angle*(M_PI/180.0f)), axis);
	}
	Matrix4 rotation(const Vector3 rotation)
	{
		Matrix4 a = glm::rotate(glm::mat4(1.0f), float(rotation.x*(M_PI / 180.0f)), Vector3(1, 0, 0));
		a *= glm::rotate(glm::mat4(1.0f), float(rotation.y*(M_PI / 180.0f)), Vector3(0, 1, 0));
		a *= glm::rotate(glm::mat4(1.0f), float(rotation.z*(M_PI / 180.0f)), Vector3(0, 0, 1));
		return a;
	}

	Matrix4 translation(const Vector3 translationVector)
	{
		return glm::translate(glm::mat4(1.0f), translationVector);
	}
	Matrix4 scale(const Vector3 scaleVector)
	{
		return glm::scale(glm::mat4(1.0f), scaleVector);
	}

	//
	void transform(){};
	void rotate(){};
	//

	Matrix4 lookat(const Vector3 eyePos, const Vector3 targetPos, const Vector3 UpVector)
	{
		return glm::lookAt(eyePos, targetPos, UpVector);
	}

	Matrix4 perspective(const float FoV, const float ratio,const float nearClip, const float farClip )
	{
		return glm::perspective(float(FoV*(M_PI/180.0f)), ratio, nearClip, farClip);
	}

	Matrix4 identity(){
		return glm::mat4(1.0f);
	}

	Vector3 Transform(const Matrix4& mat, const Vector3& p)
	{
		glm::vec4 v4(p, 0);
		glm::vec4 t = mat * v4;
		return Vector3(t.x, t.y, t.z);
		/*
		Vector3 v1 = Vector3(
			mat[0] * p.x + mat[4] * p.y + mat[8] * p.z,
			mat[1] * p.x + mat[5] * p.y + mat[9] * p.z,
			mat[2] * p.x + mat[6] * p.y + mat[10] * p.z
			);
		Vector3 v2 = Vector3(1.0f*mat[12], 1.0f*mat[13], 1.0f* mat[14]);
		return v1 + v2;*/
	}
	Vector3 GetTranslation(const Matrix4& mat)
	{
		//return Vector3(mat[12][0], mat[13][0], mat[14][0]);
		return Vector3(mat[3]);
	}

}

namespace QU{
	Quaternion FromAxisAngle(const Vector3& v, float angle)
	{
		angle *= 0.5f;
		float sinAngle = sin(angle);

		Vector3 normVector = Normalize(v);
		return Quaternion(normVector.x*sinAngle,
			normVector.y*sinAngle,
			normVector.z*sinAngle,
			cos(angle));
	}
	float ToAxisAngle(const Quaternion& q, Vector3& v)
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
	Vector3 cross(const Vector3 a, const Vector3 b){
		return glm::cross(a, b);
	}
}

#endif

namespace SPACIAL{
	Vector2 getFrustrumBoundsAtDepth(float fov, float ratio, float distance){
		float tang = (float)tan((M_PI / 180.0f) * fov * 0.5);
		float X = distance  * tang;
		float Y = X * ratio;
		return Vector2(Y, X);
	}
	Vector2 getFrustrumBoundsAtDepth(float depth, float farX, float farY, float farZ, float nearX, float nearY){
		float A=0, X=0, Y=0;
		A = ((farZ - depth) / farZ);
		X = A*farX + ((1 - A)*nearX);
		Y = Y*farY + ((1 - A)*nearY); //posible error here
		return Vector2(X, Y);
	}
	bool withinFrustrum(Vector3 position){
		return true;
	}
}

namespace M2{
	float dot(const Vector2 a, const Vector2 b){
		return a.x * b.x + a.y * b.y;
	}
}




void EulerToAngleAxis(const Vector3 euler, Vector3 &axis, float &angle)
{
	Quaternion q = EulerToQuat(euler);
	axis = glm::axis(q);
	angle = glm::angle(q);
}

void QuatToAngleAxis(const Quaternion q, Vector3 &axis, float &angle)
{
	axis = glm::axis(q);
	angle = glm::angle(q);
}

void MatrixToAngleAxis(const Matrix4 m, Vector3 &axis, float &angle){
	Quaternion q = MatrixToQuat(m);
	axis = glm::axis(q);
	angle = glm::angle(q);
}

//--

Vector3 QuatToEuler(const Quaternion q)
{
	return glm::eulerAngles(q);
}

Vector3 AngleAxisToEuler(const Vector3 axis, const float angle){
	return QuatToEuler(AngleAxisToQuat(axis, angle));
}
//only translate
Vector3 MatrixToEuler(const Matrix4 m){
	return Vector3(m[3]);
}

//--
Quaternion EulerToQuat(const Vector3 euler){
	return Quaternion(euler);
}

Quaternion AngleAxisToQuat(const Vector3 axis, const float angle){
	return glm::angleAxis(angle, axis);
}

Quaternion MatrixToQuat(const Matrix4 m){
	return glm::quat_cast(m);
}

//--
Matrix4 EulerToMatrix(const Vector3 euler)
{
	return  glm::eulerAngleYXZ(euler.x, euler.y, euler.z);
}

Matrix4 QuatToMatrix(const Quaternion q)
{
	return glm::mat4_cast(q);
}

// Angle in Radians
Matrix4 AngleAxisToMatrix(const Vector3 axis, const float angle)
{
	return glm::rotate(glm::mat4(1.0f), angle, axis);
}