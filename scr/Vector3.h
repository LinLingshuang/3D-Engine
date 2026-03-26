#pragma once
#include <cmath>
//三维向量
class Vector3 {
public:
	Vector3() {
		x = 0;
		y = 0;
		z = 0;
	}
	Vector3(float xx, float yy, float zz) {
		x = xx;
		y = yy;
		z = zz;
	}
	Vector3(const Vector3& otherVector3) {
		x = otherVector3.x;
		y = otherVector3.y;
		z = otherVector3.z;
	}
	~Vector3() {};

	Vector3 operator+(const Vector3& otherVector3)const {
		return Vector3(x + otherVector3.getX(), y + otherVector3.getY(), z + otherVector3.getZ());
	}
	Vector3 operator-(const Vector3& otherVector3)const {
		return Vector3(x - otherVector3.getX(), y - otherVector3.getY(), z - otherVector3.getZ());
	}
	Vector3 operator*(const float num)const {
		return Vector3(x * num, y * num, z * num);
	}
	Vector3 operator/(const float num)const {
		return Vector3(x / num, y / num, z / num);
	}
	bool operator==(const Vector3& otherVector3)const {
		if (x == otherVector3.getX() && y == otherVector3.getY() && z == otherVector3.getZ()) {
			return true;
		}
		return false;
	}
	float operator*(const Vector3& otherVector3)const {
		return (x * otherVector3.getX() + y * otherVector3.getY() + z * otherVector3.getZ());
	}

	float length()const {
		return sqrtf(x * x + y * y + z * z);
	}
	float cosAngel(const Vector3& otherVector3)const {
		return ((*this) * otherVector3) / ((*this).length() * otherVector3.length());
	}

	float getX()const { return x; }
	float getY()const { return y; }
	float getZ()const { return z; }

private:
	float x;
	float y;
	float z;

};
