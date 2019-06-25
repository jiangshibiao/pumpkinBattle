#ifndef _BALL_H_
#define _BALL_H_

#include "point.h"
#include "wall.h"
#include "model.h"
#include <algorithm>
#include <vector>

using namespace std;

struct Ball {
public:
	Ball(Point3D posi, Point3D velo, double radi = 1., int exstime = 500) :
		posi(posi), velo(velo), radi(radi), exstime(exstime) {};
	Point3D posi, velo;//velo != 0
	double radi;
	int exstime;
	pair<double, Point3D> collide(const Point3D &A, const Point3D &B, const Point3D &C)const;
	static void CollBalls();
};

extern vector<Ball*> BallManager;

#endif