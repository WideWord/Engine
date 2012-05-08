#include "math3d.h"
#include "math.h"

using namespace math3d;

Vector3::Vector3() {
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3::operator+=(Vector3& that) {
	x += that.x;
	y += that.y;
	z += that.z;
}

void Vector3::operator-=(Vector3& that) {
	x -= that.x;
	y -= that.y;
	z -= that.z;
}

void Vector3::operator*=(float t) {
	x *= t;
	y *= t;
	z *= t;
}

void Vector3::operator/=(float t) {
	x /= t;
	y /= t;
	z /= t;
}

void Vector3::invert () {
	x = -x;
	y = -y;
	z = -z;
}

float Vector3::length () {
	return sqrt(x*x + y*y + z*z);
}

Quaternion::Quaternion() {
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}

void Quaternion::invert () {
	//x = -x;
	//y = -y;
	//z = -z;
}

void Quaternion::normalize () {
	float t = sqrt(x*x+y*y+z*z+w*w);
	w = w / t;
	x = x /  t;
	y = y / t;
	z = z / t;
}

Matrix4::Matrix4 () {
	mtx = new float[16];
	
	mtx[0] = 1;		mtx[1] = 0; 	mtx[2] = 0; 	mtx[3] = 0;
	mtx[4] = 0;		mtx[5] = 1; 	mtx[6] = 0; 	mtx[7] = 0;
	mtx[8] = 0;		mtx[9] = 0; 	mtx[10] = 1; 	mtx[11] = 0;
	mtx[12] = 0;	mtx[13] = 0; 	mtx[14] = 0; 	mtx[15] = 1;
}

Matrix4::~Matrix4 () {
	delete mtx;
}

Matrix4::Matrix4 (const Matrix4& other) {
	mtx = new float[16];
	for (unsigned register char i = 0; i < 16; ++i) {
		mtx[i] = other.mtx[i];
	}
}

float& Matrix4::operator[](int idx) {
	return mtx[idx];
}

void Matrix4::operator*=(Matrix4& other) {
	float M[16];
        M[ 0] = mtx[ 0] * other[ 0] + mtx[ 1] * other[ 4] + mtx[ 2] * other[ 8] + mtx[ 3] * other[12];
        M[ 1] = mtx[ 0] * other[ 1] + mtx[ 1] * other[ 5] + mtx[ 2] * other[ 9] + mtx[ 3] * other[13];
        M[ 2] = mtx[ 0] * other[ 2] + mtx[ 1] * other[ 6] + mtx[ 2] * other[10] + mtx[ 3] * other[14];
        M[ 3] = mtx[ 0] * other[ 3] + mtx[ 1] * other[ 7] + mtx[ 2] * other[11] + mtx[ 3] * other[15];
        M[ 4] = mtx[ 4] * other[ 0] + mtx[ 5] * other[ 4] + mtx[ 6] * other[ 8] + mtx[ 7] * other[12];
        M[ 5] = mtx[ 4] * other[ 1] + mtx[ 5] * other[ 5] + mtx[ 6] * other[ 9] + mtx[ 7] * other[13];
        M[ 6] = mtx[ 4] * other[ 2] + mtx[ 5] * other[ 6] + mtx[ 6] * other[10] + mtx[ 7] * other[14];
        M[ 7] = mtx[ 4] * other[ 3] + mtx[ 5] * other[ 7] + mtx[ 6] * other[11] + mtx[ 7] * other[15];
        M[ 8] = mtx[ 8] * other[ 0] + mtx[ 9] * other[ 4] + mtx[10] * other[ 8] + mtx[11] * other[12];
        M[ 9] = mtx[ 8] * other[ 1] + mtx[ 9] * other[ 5] + mtx[10] * other[ 9] + mtx[11] * other[13];
        M[10] = mtx[ 8] * other[ 2] + mtx[ 9] * other[ 6] + mtx[10] * other[10] + mtx[11] * other[14];
        M[11] = mtx[ 8] * other[ 3] + mtx[ 9] * other[ 7] + mtx[10] * other[11] + mtx[11] * other[15];
        M[12] = mtx[12] * other[ 0] + mtx[13] * other[ 4] + mtx[14] * other[ 8] + mtx[15] * other[12];
        M[13] = mtx[12] * other[ 1] + mtx[13] * other[ 5] + mtx[14] * other[ 9] + mtx[15] * other[13];
        M[14] = mtx[12] * other[ 2] + mtx[13] * other[ 6] + mtx[14] * other[10] + mtx[15] * other[14];
        M[15] = mtx[12] * other[ 3] + mtx[13] * other[ 7] + mtx[14] * other[11] + mtx[15] * other[15];
	for (unsigned register char i = 0; i < 16; ++i) {
		mtx[i] = M[i];
	}
}

void Matrix4::setPerspective ( float fovy, float aspect, float znear, float zfar) {
	    // fovy передается в градусах - сконвертируем его в радианы
        float f = 1.0f / tanf(fovy * M_PI / 360.0f),
              A = (zfar + znear) / (znear - zfar),
              B = (2.0f * zfar * znear) / (znear - zfar);

        mtx[ 0] = f / aspect; mtx[ 1] =  0; mtx[ 2] =  0; mtx[ 3] =  0;
        mtx[ 4] = 0;          mtx[ 5] =  f; mtx[ 6] =  0; mtx[ 7] =  0;
        mtx[ 8] = 0;          mtx[ 9] =  0; mtx[10] =  A; mtx[11] =  B;
        mtx[12] = 0;          mtx[13] =  0; mtx[14] = -1.0f; mtx[15] =  0;
}

void Matrix4::setTranslation(Vector3& vec) {
		mtx[ 0] = 1; mtx[ 1] = 0; mtx[ 2] = 0; mtx[ 3] = vec.x;
        mtx[ 4] = 0; mtx[ 5] = 1; mtx[ 6] = 0; mtx[ 7] = vec.y;
        mtx[ 8] = 0; mtx[ 9] = 0; mtx[10] = 1; mtx[11] = vec.z;
        mtx[12] = 0; mtx[13] = 0; mtx[14] = 0; mtx[15] = 1;
}

void Matrix4::setRotation(Quaternion& q) {
	q.normalize();
	mtx[0] = q.w;	mtx[1] = q.z;	mtx[2] = -q.y;	mtx[3] = q.x;
	mtx[4] = -q.z;	mtx[5] = q.w;	mtx[6] = q.x;	mtx[7] = q.y;
	mtx[8] = q.y;	mtx[9]=-q.x;	mtx[10]=q.w; 	mtx[11]=q.z;
	mtx[12]=-q.x;	mtx[13]=-q.y;	mtx[14]=-q.z;	mtx[15]=q.w;

	Matrix4 m;
	
	
	m[0] = q.w;		m[1] = q.z;		m[2] = -q.y;	m[3] = q.x;
	m[4] = -q.z;	m[5] = q.w;		m[6] = q.x;		m[7] = -q.y;
	m[8] = q.y;		m[9]=-q.x;		m[10]=q.w; 		m[11]=-q.z;
	m[12]=q.x;		m[13]=q.y;		m[14]=q.z;		m[15]=q.w;
	
	(*this)*= m;
}

float* Matrix4::getPtr() {
	return mtx;
}

