#include <iostream>
#include<fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Vector3.h"
#include "Matrix33.h"

using namespace std;

const float Pi = 3.1415926;

struct camera {
	Vector3 xWorldBase = Vector3(1, 0, 0);
	Vector3 yWorldBase = Vector3(0, 1, 0);
	Vector3 zWorldBase = Vector3(0, 0, 1);

	Vector3 xbase = Vector3(1, 0, 0);
	Vector3 ybase = Vector3(0, 1, 0);
	Vector3 zbase = Vector3(0, 0, 1);


	Vector3 position = Vector3();
	void setPosition(float x, float y, float z) {
		position = Vector3(x, y, z);
	}
	void setBase(Vector3 xxbase, Vector3 yybase, Vector3 zzbase) {
		xbase = xxbase;
		ybase = yybase;
		zbase = zzbase;
	}

	float yaw = 0.0f;   // 水平旋转角度（弧度或度）
	float pitch = 0.0f; // 垂直旋转角度
	float sensitivity = 0.05f; // 鼠标灵敏度

	void processMouseMove(float dx, float dy) {
		// 根据鼠标偏移更新角度
		yaw += dx * sensitivity;
		pitch += dy * sensitivity;

		// 限制俯仰角在 -90° 到 90° 之间（避免翻转）
		if (pitch > 90.0f) pitch = 90.0f;
		if (pitch < -90.0f) pitch = -90.0f;
	}

	void move(char key) {
		switch (key)
		{
		case 'W':
			position = position + xbase*3;
			break;
		case 'A':
			position = position + ybase*3;
			break;
		case 'E':
			position = position + zbase*3;
			break;
		case 'S':
			position = position - xbase*3;
			break;
		case 'D':
			position = position - ybase*3;
			break;
		case 'Q':
			position = position - zbase*3;
			break;
		default:
			position = position;
			break;
		}
	}

	enum mouseDirection{UP,DOWN,LEFT,RIGHT,LEFTLEAN,RIGHTLEAN};

	void rotate(mouseDirection mDirction) {
		switch (mDirction)
		{
		case RIGHT:
			setBase(xbase * cosf(Pi/90) + ybase * sinf(Pi / 90),
					xbase * cosf(Pi / 90 + Pi / 2) + ybase * sinf(Pi / 90 + Pi / 2),
					zbase);
			break;
		case LEFT:
			setBase(xbase * cosf(-Pi / 90) + ybase * sinf(-Pi / 90),
				xbase * cosf(-Pi / 90 + Pi / 2) + ybase * sinf(-Pi / 90 + Pi / 2),
				zbase);
			break;
		case UP:
			setBase(xbase * cosf(Pi / 90) + zbase * sinf(Pi / 90),
				ybase,
				xbase * cosf(Pi / 90 + Pi / 2) + zbase * sinf(Pi / 90 + Pi / 2)
				); 
			break;
		case DOWN:
			setBase(xbase * cosf(-Pi / 90) + zbase * sinf(-Pi / 90),
				ybase,
				xbase * cosf(-Pi / 90 + Pi / 2) + zbase * sinf(-Pi / 90 + Pi / 2)
			);
			break;
		case LEFTLEAN:
			setBase(xbase,
				ybase * cosf(Pi / 90) + zbase * sinf(Pi / 90),
				ybase * cosf(Pi / 90 + Pi / 2) + zbase * sinf(Pi / 90 + Pi / 2));
			break;
		case RIGHTLEAN:
			setBase(xbase,
				ybase * cosf(-Pi / 90) + zbase * sinf(-Pi / 90),
				ybase * cosf(-Pi / 90 + Pi / 2) + zbase * sinf(-Pi / 90 + Pi / 2));
			break;
		default:
			break;
		}
		
	}
};

struct block {
	Vector3 xbase = Vector3(1, 0, 0);
	Vector3 ybase = Vector3(0, 1, 0);
	Vector3 zbase = Vector3(0, 0, 1);

	Vector3 position = Vector3();
	Vector3 viewPosition = Vector3();
	void setPosition(float x, float y,float z) {
		position = Vector3(x, y, z);
	}
	void setBase(Vector3 xxbase, Vector3 yybase, Vector3 zzbase) {
		xbase = xxbase;
		ybase = yybase;
		zbase = zzbase;
	}
	void findTransformationPosition(camera c) {
		Vector3 positionVector = position - c.position;
		Matrix33 A(xbase, ybase, zbase);
		Matrix33 B(c.xbase, c.ybase, c.zbase);
		B.transposition();
		Matrix33 C = findExcessMatrix(A, B);

		//std::cout << "Excess Matrix is：" << endl
		//	<< "[" << setw(6) << C.getXvector3().getX() << "," << setw(6) << C.getXvector3().getY() << "," << setw(6) << C.getXvector3().getZ() << "]" << endl
		//	<< "[" << setw(6) << C.getYvector3().getX() << "," << setw(6) << C.getYvector3().getY() << "," << setw(6) << C.getYvector3().getZ() << "]" << endl
		//	<< "[" << setw(6) << C.getZvector3().getX() << "," << setw(6) << C.getZvector3().getY() << "," << setw(6) << C.getZvector3().getZ() << "]" << endl;

		Matrix33 X(Vector3(positionVector.getX(), 0, 0), Vector3(positionVector.getY(), 0, 0), Vector3(positionVector.getZ(), 0, 0));
		Matrix33 Y = findExcessMatrix(C, X);
		viewPosition = Vector3(Y.getXvector3().getX(), Y.getYvector3().getX(), Y.getZvector3().getX());
	}
};
struct HyperbolaParabola {
	Vector3 xbase = Vector3(1, 0, 0);
	Vector3 ybase = Vector3(0, 1, 0);
	Vector3 zbase = Vector3(0, 0, 1);

	vector<Vector3>position;
	vector<Vector3>viewPosition;
	void findPosition(float a,float b) {
		for (int x = -50; x < 50; x++) {
			for (int y = -50; y < 50; y++) {
				for (int z = -50; z < 50; z++) {
					if ((pow(x, 2) / pow(a, 2)) - (pow(y, 2) / pow(b, 2)) <= z+0.5&& (pow(x, 2) / pow(a, 2)) - (pow(y, 2) / pow(b, 2)) >= z - 0.5) {
						position.push_back(Vector3(x, y, z));
					}
				}
			}
		}
		ofstream outPosition;
		outPosition.open("HyperbolaParabolaPosition.txt");
		for (int i = 0; i < position.size(); i++) {
			outPosition << position[i].getX() << " " << position[i].getY() << " " << position[i].getZ() << endl;
		}
		outPosition.close();
	}

	void setPosition() {
		ifstream inPosition;
		inPosition.open("HyperbolaParabolaPosition.txt");
		string temp;
		int num = 0;
		while (getline(inPosition, temp)) {
			num++;
		}
		inPosition.close();
		int x, y, z;
		inPosition.open("HyperbolaParabolaPosition.txt");
		for (int i = 0; i < num; i++) {
			inPosition >> x >> y >> z;
			position.push_back(Vector3(x, y, z));
		}
		inPosition.close();
	}
	void setBase(Vector3 xxbase, Vector3 yybase, Vector3 zzbase) {
		xbase = xxbase;
		ybase = yybase;
		zbase = zzbase;
	}
	void findTransformationPosition(camera c) {
		for(int i=0;i< position.size();i++)
		{
			Vector3 positionVector = position[i] - c.position;
			Matrix33 A(xbase, ybase, zbase);
			Matrix33 B(c.xbase, c.ybase, c.zbase);
			B.transposition();
			Matrix33 C = findExcessMatrix(A, B);

			Matrix33 X(Vector3(positionVector.getX(), 0, 0), Vector3(positionVector.getY(), 0, 0), Vector3(positionVector.getZ(), 0, 0));
			Matrix33 Y = findExcessMatrix(C, X);
			viewPosition.push_back(Vector3(Y.getXvector3().getX(), Y.getYvector3().getX(), Y.getZvector3().getX()));
		}
	}

};
struct Elliptical {
	Vector3 xbase = Vector3(1, 0, 0);
	Vector3 ybase = Vector3(0, 1, 0);
	Vector3 zbase = Vector3(0, 0, 1);

	vector<Vector3>position;
	vector<Vector3>viewPosition;
	void findPosition(float a, float b, float c) {
		for (int x = -50; x < 50; x++) {
			for (int y = -50; y < 50; y++) {
				for (int z = -50; z < 50; z++) {
					if ((pow(x, 2) / pow(a, 2)) + (pow(y, 2) / pow(b, 2)) + (pow(z, 2) / pow(c, 2)) <= 1 + 0.1 &&
						(pow(x, 2) / pow(a, 2)) + (pow(y, 2) / pow(b, 2)) + (pow(z, 2) / pow(c, 2)) >= 1 - 0.1 ) {
						position.push_back(Vector3(x, y, z));
					}
				}
			}
		}
		ofstream outPosition;
		outPosition.open("EllipticalPosition.txt");
		for (int i = 0; i < position.size(); i++) {
			outPosition << position[i].getX() << " " << position[i].getY() << " " << position[i].getZ() << endl;
		}
		outPosition.close();
	}

	void setPosition() {
		ifstream inPosition;
		inPosition.open("EllipticalPosition.txt");
		string temp;
		int num = 0;
		while (getline(inPosition, temp)) {
			num++;
		}
		inPosition.close();
		int x, y, z;
		inPosition.open("EllipticalPosition.txt");
		for (int i = 0; i < num; i++) {
			inPosition >> x >> y >> z;
			position.push_back(Vector3(x, y, z));
		}
		inPosition.close();
	}
	void setBase(Vector3 xxbase, Vector3 yybase, Vector3 zzbase) {
		xbase = xxbase;
		ybase = yybase;
		zbase = zzbase;
	}
	void findTransformationPosition(camera c) {
		for (int i = 0; i < position.size(); i++)
		{
			Vector3 positionVector = position[i] - c.position;
			Matrix33 A(xbase, ybase, zbase);
			Matrix33 B(c.xbase, c.ybase, c.zbase);
			B.transposition();
			Matrix33 C = findExcessMatrix(A, B);

			Matrix33 X(Vector3(positionVector.getX(), 0, 0), Vector3(positionVector.getY(), 0, 0), Vector3(positionVector.getZ(), 0, 0));
			Matrix33 Y = findExcessMatrix(C, X);
			viewPosition.push_back(Vector3(Y.getXvector3().getX(), Y.getYvector3().getX(), Y.getZvector3().getX()));
		}
	}

};
struct Hyperbocured {
	Vector3 xbase = Vector3(1, 0, 0);
	Vector3 ybase = Vector3(0, 1, 0);
	Vector3 zbase = Vector3(0, 0, 1);

	vector<Vector3>position;
	vector<Vector3>viewPosition;
	void findPosition(float a, float b, float c) {
		for (int x = -50; x < 50; x++) {
			for (int y = -50; y < 50; y++) {
				for (int z = -50; z < 50; z++) {
					if ((pow(x, 2) / pow(a, 2)) + (pow(y, 2) / pow(b, 2)) - (pow(z, 2) / pow(c, 2)) <= 1 + 0.1 &&
						(pow(x, 2) / pow(a, 2)) + (pow(y, 2) / pow(b, 2)) - (pow(z, 2) / pow(c, 2)) >= 1 - 0.1) {
						position.push_back(Vector3(x, y, z));
					}
				}
			}
		}
		ofstream outPosition;
		outPosition.open("HyperbocuredPosition.txt");
		for (int i = 0; i < position.size(); i++) {
			outPosition << position[i].getX() << " " << position[i].getY() << " " << position[i].getZ() << endl;
		}
		outPosition.close();
	}

	void setPosition() {
		ifstream inPosition;
		inPosition.open("HyperbocuredPosition.txt");
		string temp;
		int num = 0;
		while (getline(inPosition, temp)) {
			num++;
		}
		inPosition.close();
		int x, y, z;
		inPosition.open("HyperbocuredlPosition.txt");
		for (int i = 0; i < num; i++) {
			inPosition >> x >> y >> z;
			position.push_back(Vector3(x, y, z));
		}
		inPosition.close();
	}
	void setBase(Vector3 xxbase, Vector3 yybase, Vector3 zzbase) {
		xbase = xxbase;
		ybase = yybase;
		zbase = zzbase;
	}
	void findTransformationPosition(camera c) {
		for (int i = 0; i < position.size(); i++)
		{
			Vector3 positionVector = position[i] - c.position;
			Matrix33 A(xbase, ybase, zbase);
			Matrix33 B(c.xbase, c.ybase, c.zbase);
			B.transposition();
			Matrix33 C = findExcessMatrix(A, B);

			Matrix33 X(Vector3(positionVector.getX(), 0, 0), Vector3(positionVector.getY(), 0, 0), Vector3(positionVector.getZ(), 0, 0));
			Matrix33 Y = findExcessMatrix(C, X);
			viewPosition.push_back(Vector3(Y.getXvector3().getX(), Y.getYvector3().getX(), Y.getZvector3().getX()));
		}
	}

};

void initializeBlock(int length, Vector3 point, block myBlocks[]) {
	int x = point.getX();
	int y = point.getY();
	int z = point.getZ();
	for (int i = 0; i < length; i++) {
		myBlocks[i].setPosition(1 + i + x, 0 + y, 0 + z);
		myBlocks[i + length * 1].setPosition(1 + i + x, length + 1 + y, 0 + z);
		myBlocks[i + length * 2].setPosition(1 + i + x, 0 + y, length + 1 + z);
		myBlocks[i + length * 3].setPosition(1 + i + x, length + 1 + y, length + 1 + z);
		myBlocks[i + length * 4].setPosition(0 + x, 1 + i + y, 0 + z);
		myBlocks[i + length * 5].setPosition(length + 1 + x, 1 + i + y, 0 + z);
		myBlocks[i + length * 6].setPosition(0 + x, 1 + i + y, length + 1 + z);
		myBlocks[i + length * 7].setPosition(length + 1 + x, 1 + i + y, length + 1 + z);
		myBlocks[i + length * 8].setPosition(0 + x, 0 + y, 1 + i + z);
		myBlocks[i + length * 9].setPosition(length + 1 + x, 0 + y, 1 + i + z);
		myBlocks[i + length * 10].setPosition(0 + x, length + 1 + y, 1 + i + z);
		myBlocks[i + length * 11].setPosition(length + 1 + x, length + 1 + y, 1 + i + z);
	}
	myBlocks[length * 12].setPosition(0 + x, 0 + y, 0 + z);
	myBlocks[length * 12 + 1].setPosition(length + 1 + x, 0 + y, 0 + z);
	myBlocks[length * 12 + 2].setPosition(0 + x, length + 1 + y, 0 + z);
	myBlocks[length * 12 + 3].setPosition(length + 1 + x, length + 1 + y, 0 + z);
	myBlocks[length * 12 + 4].setPosition(0 + x, 0 + y, length + 1 + z);
	myBlocks[length * 12 + 5].setPosition(length + 1 + x, 0 + y, length + 1 + z);
	myBlocks[length * 12 + 6].setPosition(0 + x, length + 1 + y, length + 1 + z);
	myBlocks[length * 12 + 7].setPosition(length + 1 + x, length + 1 + y, length + 1 + z);

}
void initializeGroud(int groundLength,int groundWidth,int groundGirdSize,block myBlocks[]) {
	for (int i = 0; i < groundLength + groundWidth; i++) {
		if (i < groundLength) {
			for (int j = 0; j < groundWidth / groundGirdSize; j++) {
				myBlocks[i + j+i*(groundWidth / groundGirdSize)].setPosition(0 - groundLength / 2 + i, 0 - groundWidth / 2 + j * groundGirdSize, 0);
			}
		}
		else {
			for (int j = 0; j < groundLength / groundGirdSize; j++) {
				//myBlocks[i + j + i * (groundWidth / groundGirdSize)].setPosition(0 + groundWidth / 2 - j * groundGirdSize, 0 - groundLength / 2 + i , 0);
			}
		}
	}
	
}

Matrix33 findExcessMatrix(Matrix33 A, Matrix33 B) {
	Matrix33 AA=A;
	Matrix33 BB=B;
	//while (!(AA == Matrix33())) {
		//
		while (AA.getXvector3().getX() == 0.0) {
			Vector3 tempXvector3 = AA.getXvector3();
			Vector3 tempYvector3 = AA.getYvector3();
			Vector3 tempZvector3 = AA.getZvector3();
			AA.xVector3= tempYvector3;
			AA.yVector3= tempZvector3;
			AA.zVector3= tempXvector3;
			Vector3 tempBXvector3 = BB.getXvector3();
			Vector3 tempBYvector3 = BB.getYvector3();
			Vector3 tempBZvector3 = BB.getZvector3();
			BB.xVector3 = tempBYvector3;
			BB.yVector3 = tempBZvector3;
			BB.zVector3 = tempBXvector3;
		}
		if (AA.getXvector3().getX() != 1.0) {
			Vector3 tempXvector3 = AA.getXvector3();
			Vector3 tempYvector3 = AA.getYvector3();
			Vector3 tempZvector3 = AA.getZvector3();
			AA.xVector3 = Vector3(AA.getXvector3().getX() / AA.getXvector3().getX(),
									  AA.getXvector3().getY() / AA.getXvector3().getX(),
									  AA.getXvector3().getZ() / AA.getXvector3().getX());
			BB.xVector3 = Vector3(BB.getXvector3().getX() / tempXvector3.getX(),
									  BB.getXvector3().getY() / tempXvector3.getX(),
									  BB.getXvector3().getZ() / tempXvector3.getX());
		}
		if (AA.getYvector3().getX() != 0.0) {
			Vector3 tempXvector3 = AA.getXvector3();
			Vector3 tempYvector3 = AA.getYvector3();
			Vector3 tempZvector3 = AA.getZvector3();
			AA.yVector3 = Vector3(AA.getYvector3().getX() - AA.getXvector3().getX() * AA.getYvector3().getX(),
									  AA.getYvector3().getY() - AA.getXvector3().getY() * AA.getYvector3().getX(),
									  AA.getYvector3().getZ() - AA.getXvector3().getZ() * AA.getYvector3().getX());
			BB.yVector3 = Vector3(BB.getYvector3().getX() - BB.getXvector3().getX() * tempYvector3.getX(),
									  BB.getYvector3().getY() - BB.getXvector3().getY() * tempYvector3.getX(),
									  BB.getYvector3().getZ() - BB.getXvector3().getZ() * tempYvector3.getX());
		}
		if (AA.getZvector3().getX() != 0.0) {
			Vector3 tempXvector3 = AA.getXvector3();
			Vector3 tempYvector3 = AA.getYvector3();
			Vector3 tempZvector3 = AA.getZvector3();
			AA.zVector3 = Vector3(AA.getZvector3().getX() - AA.getXvector3().getX() * AA.getZvector3().getX(),
									  AA.getZvector3().getY() - AA.getXvector3().getY() * AA.getZvector3().getX(),
									  AA.getZvector3().getZ() - AA.getXvector3().getZ() * AA.getZvector3().getX());
			BB.zVector3 = Vector3(BB.getZvector3().getX() - BB.getXvector3().getX() * tempZvector3.getX(),
									  BB.getZvector3().getY() - BB.getXvector3().getY() * tempZvector3.getX(),
									  BB.getZvector3().getZ() - BB.getXvector3().getZ() * tempZvector3.getX());
		}
		//
		while (AA.getYvector3().getY() == 0.0) {
			Vector3 tempYvector3 = AA.getYvector3();
			Vector3 tempZvector3 = AA.getZvector3();
			AA.yVector3 = tempZvector3;
			AA.zVector3 = tempYvector3;
			Vector3 tempBYvector3 = BB.getYvector3();
			Vector3 tempBZvector3 = BB.getZvector3();
			BB.yVector3 = tempBZvector3;
			BB.zVector3 = tempBYvector3;
		}
		if (AA.getYvector3().getY() != 1.0) {
			Vector3 tempXvector3 = AA.getXvector3();
			Vector3 tempYvector3 = AA.getYvector3();
			Vector3 tempZvector3 = AA.getZvector3();
			AA.yVector3 = Vector3(AA.getYvector3().getX() / AA.getYvector3().getY(),
								      AA.getYvector3().getY() / AA.getYvector3().getY(),
									  AA.getYvector3().getZ() / AA.getYvector3().getY());
			BB.yVector3 = Vector3(BB.getYvector3().getX() / tempYvector3.getY(),
									  BB.getYvector3().getY() / tempYvector3.getY(),
									  BB.getYvector3().getZ() / tempYvector3.getY());
		}
		if (AA.getZvector3().getY() != 0.0) {
			Vector3 tempXvector3 = AA.getXvector3();
			Vector3 tempYvector3 = AA.getYvector3();
			Vector3 tempZvector3 = AA.getZvector3();
			AA.zVector3 = Vector3(AA.getZvector3().getX() - AA.getYvector3().getX() * AA.getZvector3().getY(),
									  AA.getZvector3().getY() - AA.getYvector3().getY() * AA.getZvector3().getY(),
									  AA.getZvector3().getZ() - AA.getYvector3().getZ() * AA.getZvector3().getY());
			BB.zVector3 = Vector3(BB.getZvector3().getX() - BB.getYvector3().getX() * tempZvector3.getY(),
									  BB.getZvector3().getY() - BB.getYvector3().getY() * tempZvector3.getY(),
									  BB.getZvector3().getZ() - BB.getYvector3().getZ() * tempZvector3.getY());
		}
		//
		if (AA.getZvector3().getZ() != 1.0) {
			Vector3 tempXvector3 = AA.getXvector3();
			Vector3 tempYvector3 = AA.getYvector3();
			Vector3 tempZvector3 = AA.getZvector3();
			AA.zVector3 = Vector3(AA.getZvector3().getX() / AA.getZvector3().getZ(),
									  AA.getZvector3().getY() / AA.getZvector3().getZ(),
									  AA.getZvector3().getZ() / AA.getZvector3().getZ());
			BB.zVector3 = Vector3(BB.getZvector3().getX() / tempZvector3.getZ(),
									  BB.getZvector3().getY() / tempZvector3.getZ(),
									  BB.getZvector3().getZ() / tempZvector3.getZ());
		}
		//
		if (AA.getXvector3().getZ() != 0.0) {
			Vector3 tempXvector3 = AA.getXvector3();
			Vector3 tempYvector3 = AA.getYvector3();
			Vector3 tempZvector3 = AA.getZvector3();
			AA.xVector3 = Vector3(AA.getXvector3().getX() - AA.getZvector3().getX() * AA.getXvector3().getZ(),
								   	  AA.getXvector3().getY() - AA.getZvector3().getY() * AA.getXvector3().getZ(),
									  AA.getXvector3().getZ() - AA.getZvector3().getZ() * AA.getXvector3().getZ());
			BB.xVector3 = Vector3(BB.getXvector3().getX() - BB.getZvector3().getX() * tempXvector3.getZ(),
									  BB.getXvector3().getY() - BB.getZvector3().getY() * tempXvector3.getZ(),
									  BB.getXvector3().getZ() - BB.getZvector3().getZ() * tempXvector3.getZ());
		}
		if (AA.getYvector3().getZ() != 0.0) {
			Vector3 tempXvector3 = AA.getXvector3();
			Vector3 tempYvector3 = AA.getYvector3();
			Vector3 tempZvector3 = AA.getZvector3();
			AA.yVector3 = Vector3(AA.getYvector3().getX() - AA.getZvector3().getX() * AA.getYvector3().getZ(),
									  AA.getYvector3().getY() - AA.getZvector3().getY() * AA.getYvector3().getZ(),
									  AA.getYvector3().getZ() - AA.getZvector3().getZ() * AA.getYvector3().getZ());
			BB.yVector3 = Vector3(BB.getYvector3().getX() - BB.getZvector3().getX() * tempYvector3.getZ(),
									  BB.getYvector3().getY() - BB.getZvector3().getY() * tempYvector3.getZ(),
									  BB.getYvector3().getZ() - BB.getZvector3().getZ() * tempYvector3.getZ());
		}
		//
		if (AA.getXvector3().getY() != 0.0) {
			Vector3 tempXvector3 = AA.getXvector3();
			Vector3 tempYvector3 = AA.getYvector3();
			Vector3 tempZvector3 = AA.getZvector3();
			AA.xVector3 = Vector3(AA.getXvector3().getX() - AA.getYvector3().getX() * AA.getXvector3().getY(),
									  AA.getXvector3().getY() - AA.getYvector3().getY() * AA.getXvector3().getY(),
									  AA.getXvector3().getZ() - AA.getYvector3().getZ() * AA.getXvector3().getY());
			BB.xVector3 = Vector3(BB.getXvector3().getX() - BB.getYvector3().getX() * tempXvector3.getY(),
									  BB.getXvector3().getY() - BB.getYvector3().getY() * tempXvector3.getY(),
									  BB.getXvector3().getZ() - BB.getYvector3().getZ() * tempXvector3.getY());
		}
		//
	//}
	Matrix33 C = BB;
	return C;
}
bool Screen[500][1000] = { false };


void shootOnScreen(block b,camera c) {
	float screenXposition = 400;
	float screenYposition = 0;
	float screenZposition = 0;
	b.findTransformationPosition(c);
	if (b.viewPosition.getX() > 0) {

		float shootX = screenXposition* b.viewPosition.getY() / b.viewPosition.getX();
		float shootY = screenXposition* b.viewPosition.getZ() / b.viewPosition.getX();

		if (shootX < 500 && shootX >= -500 && shootY < 250 && shootY >= -250) {
			int x = shootX;
			int y = shootY;
			if (x == 500) {
				x--;
			}
			if (y == 250) {
				y--;
			}
			Screen[249-y][499-x] = true;
		}

	}
}
void shootOnScreen( HyperbolaParabola h/*Hyperbocured h*/  /*Elliptical h*/, camera c) {
	float screenXposition = 400;
	float screenYposition = 0;
	float screenZposition = 0;
	h.findTransformationPosition(c);
	for(int i=0;i< h.viewPosition.size();i++){
		if (h.viewPosition[i].getX() > 0) {

			float shootX = screenXposition * h.viewPosition[i].getY() / h.viewPosition[i].getX();
			float shootY = screenXposition * h.viewPosition[i].getZ() / h.viewPosition[i].getX();

			if (shootX < 500 && shootX >= -500 && shootY < 250 && shootY >= -250) {
				int x = shootX;
				int y = shootY;
				if (x == 500) {
					x--;
				}
				if (y == 250) {
					y--;
				}
				Screen[249 - y][499 - x] = true;
			}

		}
	}
}


int main() {

	int length1 = 20, length2 = 40, length3 = 50, length4 = 100;
	//Vector3 point1(1,1,1), point2(25,25,10), point3(-70,-60,520), point4(-10,-140,250);
	//block* myBlocks1 = new block[length1 * 12 + 8];
	//block* myBlocks2 = new block[length2 * 12 + 8];
	//block* myBlocks3 = new block[length3 * 12 + 8];
	//block* myBlocks4 = new block[length4 * 12 + 8];

	int groundLength=100, groundWidth=100, groundGirdSize=20;
	//block* myGround = new block[groundLength * groundWidth / groundGirdSize + groundLength * groundWidth / groundGirdSize];

	//initializeBlock(length1, point1, myBlocks1);
	//initializeBlock(length2, point2, myBlocks2);
	//initializeBlock(length3, point3, myBlocks3);
	//initializeBlock(length4, point4, myBlocks4);

	//initializeGroud(groundLength, groundWidth, groundGirdSize, myGround);

	HyperbolaParabola myHP;
	//myHP.findPosition(4,4);
	myHP.setPosition();
	Elliptical myEP;
	//myEP.findPosition(50, 30, 15);
	myEP.setPosition();
	Hyperbocured myHB;
	//myHB.findPosition(10, 20, 30);
	myHB.setPosition();


	camera myCamera;
	myCamera.setPosition(50, 0, 0);
	
	


	//myCamera.setBase(Vector3(cosf(-Pi / 4), sinf(-Pi / 4), 0), Vector3(cosf(-Pi / 4 + Pi / 2), sinf(-Pi / 4 + Pi / 2), 0), Vector3(0, 0, 1));
	myCamera.setBase(Vector3(-1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1));

	for (int i = 0; i < 500; i++) {
		for (int j = 0; j < 1000; j++) {
			Screen[i][j] = false;
		}
	}

	sf::RenderWindow window(sf::VideoMode(1000, 500), "3D game");
	sf::Texture text;
	text.loadFromFile("block.png");

	sf::Sprite/*指可移动图像*/s(text);
	s.setTextureRect(sf::IntRect(0, 0, 1, 1));

	bool mouseLocked = false;     // 当前鼠标是否锁定
	bool wasPressedH = false;     // 用于检测 H 键的按下瞬间（避免持续触发）

	sf::Vector2u windowCenter = window.getSize();
	windowCenter.x /= 2;
	windowCenter.y /= 2;

	float tickTime = 0, delay = 0.3;

	sf::Clock clock;

	int moveNum = 1;

	while (window.isOpen())
	{
            

		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		tickTime += time;
		/*
		if (tickTime > 4) {
			delete[] myBlocks2;
			myBlocks2 = new block[length2 * 12 + 8];
			point2 = Vector3(25 + moveNum, 25, 150);
			initializeBlock(length2, point2, myBlocks2);
			moveNum++;
			tickTime = 0;
		}
		*/
		sf::Event gameEvent;
		sf::Vector2i previousMousePosition = sf::Mouse::getPosition(window);

		while (window.pollEvent(gameEvent))
		{
			


			if (gameEvent.type == sf::Event::Closed) {
				window.close();
			}
			// 当窗口失去焦点时自动解锁鼠标
			if (gameEvent.type == sf::Event::LostFocus) {
				if (mouseLocked) {
					mouseLocked = false;
					window.setMouseCursorVisible(true);
				}
			}
			if (gameEvent.type == sf::Event::KeyPressed) {
				if (gameEvent.key.code == sf::Keyboard::W)	myCamera.move('W');
				if (gameEvent.key.code == sf::Keyboard::A)myCamera.move('A');
				if (gameEvent.key.code == sf::Keyboard::S)myCamera.move('S');
				if (gameEvent.key.code == sf::Keyboard::D)myCamera.move('D');
				if (gameEvent.key.code == sf::Keyboard::Q)myCamera.move('Q');
				if (gameEvent.key.code == sf::Keyboard::E)myCamera.move('E');

				else if (gameEvent.key.code == sf::Keyboard::Z)myCamera.rotate(camera::LEFT);
				else if (gameEvent.key.code == sf::Keyboard::X)myCamera.rotate(camera::RIGHT);
				else if (gameEvent.key.code == sf::Keyboard::C)myCamera.rotate(camera::UP);
				else if (gameEvent.key.code == sf::Keyboard::V)myCamera.rotate(camera::DOWN);
				else if (gameEvent.key.code == sf::Keyboard::R)myCamera.rotate(camera::LEFTLEAN);
				else if (gameEvent.key.code == sf::Keyboard::T)myCamera.rotate(camera::RIGHTLEAN);

				for (int i = 0; i < 500; i++) {
					for (int j = 0; j < 1000; j++) {
						Screen[i][j] = false;
					}
				}
			}
			// 检测 H 键按下（切换锁定状态）
			bool isHPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::H);
			if (isHPressed && !wasPressedH) {
				mouseLocked = !mouseLocked;
				window.setMouseCursorVisible(!mouseLocked); // 锁定时光标隐藏
			}
			wasPressedH = isHPressed;

			// 如果鼠标锁定，处理鼠标移动
			if (mouseLocked) {
				// 获取当前鼠标位置（相对于窗口）
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);

				// 计算相对于窗口中心的偏移量
				int dx = mousePos.x - static_cast<int>(windowCenter.x);
				int dy = mousePos.y - static_cast<int>(windowCenter.y);

				// 将鼠标重置到窗口中心
				sf::Mouse::setPosition(sf::Vector2i(windowCenter.x, windowCenter.y), window);
			}

			if (gameEvent.MouseMoved == sf::Event::EventType::MouseMoved) {

				sf::Vector2i currentMousePosition = sf::Mouse::getPosition(window);

				if (currentMousePosition.x < previousMousePosition.x)myCamera.rotate(camera::LEFT);
				else if (currentMousePosition.x > previousMousePosition.x)myCamera.rotate(camera::RIGHT);
				//if (currentMousePosition.y > previousMousePosition.y)myCamera.rotate(camera::UP);
				//else if (currentMousePosition.y < previousMousePosition.y)myCamera.rotate(camera::DOWN);
				//else if (gameEvent.mouseMove.x > 0)myCamera.rotate(camera::LEFTLEAN);
				//else if (gameEvent.mouseMove.x > 0)myCamera.rotate(camera::RIGHTLEAN);

				for (int i = 0; i < 500; i++) {
					for (int j = 0; j < 1000; j++) {
						Screen[i][j] = false;
					}
				}

				previousMousePosition = currentMousePosition;
			}

		}
		

		for (int i = 0; i < length1 * 12 + 8; i++) {
			//shootOnScreen(myBlocks1[i], myCamera);
		}
		for (int i = 0; i < length2 * 12 + 8; i++) {
			//shootOnScreen(myBlocks2[i], myCamera);
		}
		for (int i = 0; i < length3 * 12 + 8; i++) {
			//shootOnScreen(myBlocks3[i], myCamera);
		}
		for (int i = 0; i < length4 * 12 + 8; i++) {
			//shootOnScreen(myBlocks4[i], myCamera);
		}
		for (int i = 0; i < groundLength * groundWidth / groundGirdSize + groundLength * groundWidth / groundGirdSize; i++) {
			//shootOnScreen(myGround[i], myCamera);
		}
		shootOnScreen(myHP, myCamera);
		//shootOnScreen(myEP, myCamera);
		//shootOnScreen(myHB, myCamera);

		window.clear(sf::Color::White);


		for (int i = 0; i < 500; i++) {
			for (int j = 0; j < 1000; j++) {
				if (Screen[i][j]) {
					s.setTextureRect(sf::IntRect(0, 0, 1, 1));//矩形范围

					s.setPosition(j, i);
					window.draw(s);
					/*
					s.setPosition(j + 500, i + 1 + 250);
					window.draw(s);
					s.setPosition(j + 500, i - 1 + 250);
					window.draw(s);
					s.setPosition(j + 1 + 500, i + 250);
					window.draw(s);
					s.setPosition(j - 1 + 500, i + 250);
					window.draw(s);
					s.setPosition(j + 1 + 500, i + 1 + 250);
					window.draw(s);
					s.setPosition(j + 1 + 500, i - 1 + 250);
					window.draw(s);
					s.setPosition(j - 1 + 500, i + 1 + 250);
					window.draw(s);
					s.setPosition(j - 1 + 500, i - 1 + 250);
					window.draw(s);
					*/
				}
				else {

				}
			}

		}
		/*
		for (int i = 0; i < 500; i++) {
			for (int j = 0; j < 1000; j++) {
				if (Screen[i][j]) {
					cout << '#';
				}
				else {
					cout << ' ';
				}
			}
			cout << endl;
		}
		system("cls");
		*/
		window.display();
	
	}
}
