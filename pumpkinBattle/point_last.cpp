#include "point.h"

double pointtoplanedistance(const Point3D &o, const Point3D &p, const Point3D &q, const Point3D &r) {
	Point3D temp = ((r - p) * (q - p)).resize(1);
	return (o - p) % temp;
}

double sqr(const double &x) { return x * x; }

int sgn(const double &x) { return x < -eps ? -1 : x > eps; }

double dist(const Point3D &A, const Point3D &B) {
	return sqrt(sqr(A.x - B.x) + sqr(A.y - B.y));
}

/*void DrawRect(Point3D A, Point3D B, Point3D C, Point3D D, float *vertices) {
	Point3D dir = ((B - A) * (C - A)).resize(1);
	A.PushPoint(dir, vertices + DATASIZE * 0);
	B.PushPoint(dir, vertices + DATASIZE * 1);
	C.PushPoint(dir, vertices + DATASIZE * 2);
	A.PushPoint(dir, vertices + DATASIZE * 3);
	C.PushPoint(dir, vertices + DATASIZE * 4);
	D.PushPoint(dir, vertices + DATASIZE * 5);
}

void Drawcube(Point3D center, Point3D px, Point3D py, Point3D pz, float *vertices) {
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
}*/