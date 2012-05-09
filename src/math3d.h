#ifndef MATH3D_H
#define MATH3D_H
#include "platform.h"
namespace math3d {
	class ENGINE_API Vector3 {
	public:
		Vector3();
		Vector3(float x, float y, float z);
		float x;
		float y;
		float z;
		void operator+= (Vector3& that);
		void operator-= (Vector3& that);
		void operator*= (float that);
		void operator/= (float that);
		void invert();
		float length();
	};

	class ENGINE_API Quaternion {
	public:
		Quaternion();
		Quaternion(float x, float y, float z, float w);
		float x, y, z, w;
		void invert();
		void normalize();
		void setEuler(float heading, float attitude, float bank);
		void operator*= (Quaternion& s);
	};
	class ENGINE_API Matrix4 {

	private:
		float* mtx;
	public:
		Matrix4();
		~Matrix4();
		Matrix4(const Matrix4& other);
		float& operator[](int idx);
		void operator*=(Matrix4& other); 
		void setPerspective ( float fovy, float aspect, float znear, float zfar);
		void setTranslation(Vector3& vec);
		void setRotation(Quaternion& q);
		float* getPtr();
	};
}

#endif
