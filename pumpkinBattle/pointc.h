#ifndef _POINT_
#define _POINT_

#define DATASIZE 6

#include <cmath>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const double eps = 1e-10, inf = 1e10;
int sgn(const double &x);
double sqr(const double &x);

struct Point3D {
	double x, y, z;
	Point3D(){}
	Point3D(double x, double y, double z) :x(x), y(y), z(z) {}
	Point3D(const glm::vec3 &p): x(p.x), y(p.y), z(p.z){}
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
	double length() const {
		return sqrt(x * x + y * y + z * z);
	}
	void PushPoint(Point3D dir, float *vertices) {
		vertices[0] = x;
		vertices[1] = y;
		vertices[2] = z;
		vertices[3] = dir.x;
		vertices[4] = dir.y;
		vertices[5] = dir.z;
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

void DrawRect(Point3D A, Point3D B, Point3D C, Point3D D, float *vertices);
void Drawcube(Point3D center, Point3D px, Point3D py, Point3D pz, float *vertices);

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
	
#endif