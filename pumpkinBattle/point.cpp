#include "point.h"
#include <algorithm>
#include <vector>
using namespace std;

vector<Cuboid*> CuboidManager;

double pointtoplanedistance(const Point3D &o, const Point3D &p, const Point3D &q, const Point3D &r) {
	Point3D temp = ((r - p) * (q - p)).resize(1);
	return (o - p) % temp;
}
inline void pointtolinecheck(double &ret, const Point3D &o, const Point3D &p, const Point3D &q) {
	if (sgn((p - q) % (o - q)) > 0 && sgn((q - p) % (o - p)) > 0)
		ret = min(ret, ((p - o) * (q - o)).length() / (p - q).length());
}
double realpointtoplanedistance(const Point3D &o, const Point3D &p, const Point3D &q, const Point3D &r) {//fuck this...
	double ret = min(min((o - p).length(), (o - q).length()), (o - r).length());

	pointtolinecheck(ret, o, p, q);
	pointtolinecheck(ret, o, q, r);
	pointtolinecheck(ret, o, r, p);
	
	Point3D norm = ((q - p) * (r - p)).resize(1),
		normp = ((q - o) * (r - o)).resize(1),
		normq = ((r - o) * (p - o)).resize(1),
		normr = ((p - o) * (q - o)).resize(1);
	if (sgn(norm%normp) > 0 && sgn(norm%normq) > 0 && sgn(norm%normr) > 0)
		ret = min(ret, abs(pointtoplanedistance(o, p, q, r)));

	return ret;
}
double sqr(const double &x) { return x * x; }

int sgn(const double &x) { return x < -eps ? -1 : x > eps; }

double dist(const Point3D &A, const Point3D &B) {
	return sqrt(sqr(A.x - B.x) + sqr(A.y - B.y));
}