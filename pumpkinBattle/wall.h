#ifndef _WALL_H_
#define _WALL_H_

#include<stdio.h>
#include<vector>
#include<cmath>
#include "point.h"

using namespace std;

class Face {
friend class Wall;
public:
	Face(int live) : live(live){
		
	}
private:
	int live;
};


class Wall {
public:
	Wall(Point3D center, Point3D px, Point3D py, Point3D pz, int accx, int accy, int type): 
		center(center), px(px), py(py), pz(pz), accx(accx), accy(accy), type(type),
		xlength((px - center).length()),
		ylength((py - center).length()),
		zlength((pz - center).length()) {
		face.clear();	
		double tx = 1.0 / accx;
		double ty = 1.0 / accy;

		Point3D dirx = px - center;
		Point3D diry = py - center;
		Point3D dirz = pz - center;
		for (int i = 0; i < accx; i++) {
			vector<Face>Y;
			for (int j = 0; j < accy; j++)
				Y.emplace_back(Face(1));
			face.emplace_back(Y);
		}
		//may be slow
	}
	pair<double, pair<Wall*, pair<int, int>>> clicked(const Point3D &circenter, const Point3D &direction);
	void Addface(Point3D A, Point3D B, Point3D C, Point3D D);
	void Display(vector<Cube> &trans);
	vector<Point3D>allface;
	void bkdown(int x, int y);
	void filp(int x, int y);
private:
	Point3D center, px, py, pz;
	int accx, accy;
	double xlength, ylength, zlength;
	vector<vector<Face> >face;
	int type;
};

extern vector<Wall*> WallManager;

#endif