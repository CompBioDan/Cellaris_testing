
#include "datastore.h"

//namespace Cellaris {

	myVec3d::myVec3d() {
		this->pos.x = 0;
		this->pos.y = 0;
		this->pos.z = 0;
	}

	myVec3d::myVec3d(double x, double y, double z) {
		this->pos.x = x;
		this->pos.y = y;
		this->pos.z = z;
	}

	myVec3d::myVec3d(const myVec3d& v) {
		this->pos.x = v.pos.x;
		this->pos.y = v.pos.y;
		this->pos.z = v.pos.z;
	}

	myVec3d::~myVec3d() {
	}

	void myVec3d::normalize() {
		double d = length();
		// zero-div may occur.
		pos.x /= d;
		pos.y /= d;
		pos.z /= d;

		if (length() < 0.9)
			pos.x = 2;
	}

	double myVec3d::lengthSquared() {
		return pos.x*pos.x + pos.y*pos.y + pos.z*pos.z;
	}

	double myVec3d::length() {
		return sqrt(lengthSquared());
	}

	void myVec3d::add(const myVec3d& v) {
		pos.x += v.pos.x;
		pos.y += v.pos.y;
		pos.z += v.pos.z;
	}

	void myVec3d::sub(const myVec3d& v) {
		pos.x -= v.pos.x;
		pos.y -= v.pos.y;
		pos.z -= v.pos.z;
	}

	void myVec3d::negate(const myVec3d& v) {
		pos.x = -v.pos.x;
		pos.y = -v.pos.y;
		pos.z = -v.pos.z;
	}

	void myVec3d::scale(double s, const myVec3d& v) {
		pos.x = s * v.pos.x;
		pos.y = s * v.pos.y;
		pos.z = s * v.pos.z;
	}

	void myVec3d::scale(double s) {
		pos.x *= s;
		pos.y *= s;
		pos.z *= s;
	}

	void myVec3d::cross(const myVec3d& v1, const myVec3d& v2) {
		// store on stack once for aliasing-safety
		// i.e. safe when a.cross(a, b)

		double newx = v1.pos.y*v2.pos.z - v1.pos.z*v2.pos.y;
		double newy = v1.pos.z*v2.pos.x - v1.pos.x*v2.pos.z;
		double newz = v1.pos.x*v2.pos.y - v1.pos.y*v2.pos.x;
		pos.x = newx; pos.y = newy; pos.z = newz;
	}

	/**********************************************/
	myVec3f::myVec3f() {
		this->pos.x = 0;
		this->pos.y = 0;
		this->pos.z = 0;
	}

	myVec3f::myVec3f(float x, float y, float z) {
		this->pos.x = x;
		this->pos.y = y;
		this->pos.z = z;
	}

	myVec3f::myVec3f(const myVec3f& v) {
		this->pos.x = v.pos.x;
		this->pos.y = v.pos.y;
		this->pos.z = v.pos.z;
	}

	myVec3f::~myVec3f() {
	}

	void myVec3f::normalize() {
		float d = length();
		// zero-div may occur.
		pos.x /= d;
		pos.y /= d;
		pos.z /= d;

		if (length() < 0.9)
			pos.x = 2;
	}

	float myVec3f::lengthSquared() {
		return pos.x*pos.x + pos.y*pos.y + pos.z*pos.z;
	}

	float myVec3f::length() {
		return sqrt(lengthSquared());
	}

	void myVec3f::add(const myVec3f& v) {
		pos.x += v.pos.x;
		pos.y += v.pos.y;
		pos.z += v.pos.z;
	}

	void myVec3f::sub(const myVec3f& v) {
		pos.x -= v.pos.x;
		pos.y -= v.pos.y;
		pos.z -= v.pos.z;
	}

	void myVec3f::negate(const myVec3f& v) {
		pos.x = -v.pos.x;
		pos.y = -v.pos.y;
		pos.z = -v.pos.z;
	}

	void myVec3f::scale(float s, const myVec3f& v) {
		pos.x = s * v.pos.x;
		pos.y = s * v.pos.y;
		pos.z = s * v.pos.z;
	}

	void myVec3f::scale(float s) {
		pos.x *= s;
		pos.y *= s;
		pos.z *= s;
	}

	void myVec3f::cross(const myVec3f& v1, const myVec3f& v2) {
		// store on stack once for aliasing-safety
		// i.e. safe when a.cross(a, b)

		float newx = v1.pos.y*v2.pos.z - v1.pos.z*v2.pos.y;
		float newy = v1.pos.z*v2.pos.x - v1.pos.x*v2.pos.z;
		float newz = v1.pos.x*v2.pos.y - v1.pos.y*v2.pos.x;
		pos.x = newx; pos.y = newy; pos.z = newz;
	}

//}

