#pragma once
#include "Vector3.h"

class Matrix33 {
public:
	Matrix33() {
		xVector3 = Vector3(1, 0, 0);
		yVector3 = Vector3(0, 1, 0);
		zVector3 = Vector3(0, 0, 1);
	}
	Matrix33(const Vector3 xxVector3, const Vector3 yyVector3, const Vector3 zzVector3) {
		xVector3 = xxVector3;
		yVector3 = yyVector3;
		zVector3 = zzVector3;
	}
	Matrix33(const Matrix33& otherMatrix33) {
		xVector3 = otherMatrix33.xVector3;
		yVector3 = otherMatrix33.yVector3;
		zVector3 = otherMatrix33.zVector3;
	}
	~Matrix33() {}
	Matrix33 operator+(const Matrix33& otherMatrix33)const {
		return Matrix33(xVector3 + otherMatrix33.getXvector3(),
			yVector3 + otherMatrix33.getYvector3(),
			zVector3 + otherMatrix33.getZvector3());
	}
	Matrix33 operator-(const Matrix33& otherMatrix33)const {
		return Matrix33(xVector3 - otherMatrix33.getXvector3(),
			yVector3 - otherMatrix33.getYvector3(),
			zVector3 - otherMatrix33.getZvector3());
	}
	bool operator==(const Matrix33& otherMatrix33)const {
		if (xVector3 == otherMatrix33.getXvector3() &&
			yVector3 == otherMatrix33.getYvector3() &&
			zVector3 == otherMatrix33.getZvector3()) {
			return true;
		}
		return false;
	}
	void transposition() {
		Vector3 tempxVector3 = xVector3;
		Vector3 tempyVector3 = yVector3;
		Vector3 tempzVector3 = zVector3;
		xVector3 = Vector3(tempxVector3.getX(), tempyVector3.getX(), tempzVector3.getX());
		yVector3 = Vector3(tempxVector3.getY(), tempyVector3.getY(), tempzVector3.getY());
		zVector3 = Vector3(tempxVector3.getZ(), tempyVector3.getZ(), tempzVector3.getZ());
	}

	Matrix33 friend findExcessMatrix(Matrix33 A, Matrix33 B);

	Vector3 getXvector3()const { return xVector3; }
	Vector3 getYvector3()const { return yVector3; }
	Vector3 getZvector3()const { return zVector3; }

private:
	Vector3 xVector3;
	Vector3 yVector3;
	Vector3 zVector3;
};


