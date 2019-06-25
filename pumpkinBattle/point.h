#ifndef _POINT_
#define _POINT_

#define DATASIZE 8

#include <cmath>
#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "point.h"

using namespace std;

const int bordersize = 25;
const double eps = 1e-10, inf = 1e10;
int sgn(const double &x);
double sqr(const double &x);

struct Point2D {
	double x, y;
	Point2D(double x, double y) :x(x), y(y) {}
	double operator * (const Point2D &t) const {
		return x * t.y - y * t.x;
	}
	double operator % (const Point2D &t) const {
		return x * t.x + y * t.y;
	}
	Point2D operator + (const Point2D &t) const {
		return Point2D(x + t.x, y + t.y);
	}
	Point2D operator - (const Point2D &t) const {
		return Point2D(x - t.x, y - t.y);
	}
	double length() const {
		return sqrt(x * x + y * y);//hypot?
	}
	void Print() {
		printf("%.2f %.2f\n", x, y);
	}
};

struct Point3D {
	double x, y, z;
	Point3D(){}
	Point3D(double x, double y, double z) :x(x), y(y), z(z) {}
	Point3D(const glm::vec3 &p): x(p.x), y(p.y), z(p.z){}
	Point3D rot90() { return Point3D(-z, y, x); }
	glm::vec3 to_glvec()
	{
		return glm::vec3(this->x, this->y, this->z);
	}

	Point3D operator * (const Point3D &t) const {
		return Point3D(
			y * t.z - z * t.y,
			z * t.x - x * t.z,
			x * t.y - y * t.x
		);
	}
	double operator % (const Point3D &t) const {
		return x * t.x + y * t.y + z * t.z;
	}
	Point3D operator + (const Point3D &t) const {
		return Point3D(x + t.x, y + t.y, z + t.z);
	}
	Point3D operator - (const Point3D &t) const {
		return Point3D(x - t.x, y - t.y, z - t.z);
	}
	Point3D operator * (const double &t) {
		return Point3D(x * t, y * t, z * t);
	}
	Point3D operator / (const double &t) {
		return Point3D(x / t, y / t, z / t);
	}
	friend bool operator == (const Point3D& a, const Point3D& b)
	{
		return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
	}
	friend bool operator != (const Point3D& a, const Point3D& b)
	{
		return !(a == b);
	}
	double length() const {
		return sqrt(x * x + y * y + z * z);
	}
	void PushPoint(Point3D dir, float *vertices, Point2D texture = Point2D(0.0, 0.0)) {
		vertices[0] = x;
		vertices[1] = y;
		vertices[2] = z;
		vertices[3] = dir.x;
		vertices[4] = dir.y;
		vertices[5] = dir.z;
		vertices[6] = texture.x;
		vertices[7] = texture.y;
	}

	void Print() {
		printf("%.2f %.2f %.2f\n", x, y, z);
	}

	Point3D resize(double len = 1) {
		double t = len / length();
		return Point3D(x * t, y * t, z * t);
	}
};
double pointtoplanedistance(const Point3D &o, const Point3D &p, const Point3D &q, const Point3D &r);
void pointtolinecheck(double &ret, const Point3D &o, const Point3D &p, const Point3D &q);
double realpointtoplanedistance(const Point3D &o, const Point3D &p, const Point3D &q, const Point3D &r);

struct Cube {
	Point3D pos; double size; Point3D text; int type;
	Cube(Point3D pos, int type, double size, Point3D text): pos(pos), type(type), size(size), text(text){}
};


class Cuboid{
public:
	Point3D center, px, py, pz;
	Cuboid(Point3D center, Point3D px, Point3D py, Point3D pz) :center(center), px(px), py(py), pz(pz) {}
	Cuboid(Point3D center, double xlength, double ylength, double zlength,
			Point3D Front = Point3D(0, 0, 1), Point3D Left = Point3D(1, 0, 0), Point3D Up = Point3D(0, 1, 0)) :
	center(center), px(center + Left * xlength), py(center + Up * ylength), pz(center + Front * zlength) {}
	void DrawRect(Point3D A, Point3D B, Point3D C, Point3D D, float *vertices) {
		Point3D dir = ((B - A) * (C - A)).resize(1);
		A.PushPoint(dir, vertices + DATASIZE * 0);
		B.PushPoint(dir, vertices + DATASIZE * 1);
		C.PushPoint(dir, vertices + DATASIZE * 2);
		A.PushPoint(dir, vertices + DATASIZE * 3);
		C.PushPoint(dir, vertices + DATASIZE * 4);
		D.PushPoint(dir, vertices + DATASIZE * 5);
	}

	void Drawcube(float *vertices) {
		Point3D discenter = center + (px - center) + (py - center) + (pz - center);

		Point3D qx = discenter - (px - center);
		Point3D qy = discenter - (py - center);
		Point3D qz = discenter - (pz - center);
		DrawRect(center, pz, qx, py, vertices + DATASIZE * 0);
		DrawRect(px, qz, discenter, qy, vertices + DATASIZE * 6);
		DrawRect(py, qx, discenter, qz, vertices + DATASIZE * 12);
		DrawRect(qy, pz, center, px, vertices + DATASIZE * 18);
		DrawRect(center, py, qz, px, vertices + DATASIZE * 24);
		DrawRect(discenter, qx, pz, qy, vertices + DATASIZE * 30);
	}
};

extern vector<Cuboid*> CuboidManager;

#endif