#ifndef _MAZE_H_
#define _MAZE_H_
#include <vector>

const double blocksize = 0.3;
extern vector<Wall*>WallManager;

void buildspiral(Point3D center, Point3D px, Point3D py, Point3D pz) {
	int x = (px - center).length() / blocksize + eps,
		y = (py - center).length() / blocksize + eps;
	WallManager.emplace_back(new Wall(center, px, py, pz, x, y, 0));
	center = center.rot90();px = px.rot90();py = py.rot90();pz = pz.rot90();
	WallManager.emplace_back(new Wall(center, px, py, pz, x, y, 1));
	center = center.rot90();px = px.rot90();py = py.rot90();pz = pz.rot90();
	WallManager.emplace_back(new Wall(center, px, py, pz, x, y, 0));
	center = center.rot90();px = px.rot90();py = py.rot90();pz = pz.rot90();
	WallManager.emplace_back(new Wall(center, px, py, pz, x, y, 1));
}
void buildMaze() {
	/*
	buildspiral(
		Point3D(-bordersize, -0.5, -bordersize - blocksize),
		Point3D(bordersize, -0.5, -bordersize - blocksize),
		Point3D(-bordersize, -0.5 + blocksize * 3, -bordersize - blocksize),
		Point3D(-bordersize, -0.5, -bordersize)
	);
	*/
	for(int i = 1; i <= 4; ++i)
		buildspiral(
			Point3D(-i * 5.1, -0.5, -i * 5.1),
			Point3D(i * 5.1 - blocksize, -0.5, -i * 5.1),
			Point3D(-i * 5.1, -0.5 + blocksize * 6, -i * 5.1),
			Point3D(-i * 5.1, -0.5, -i * 5.1 + blocksize)
		);
}
#endif
