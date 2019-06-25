#ifndef _MONSTER_H_
#define _MONSTER_H_

#include "camera.h"
#include "Wall.h"
#include "Model.h"

extern const int bordersize;
class Monster : public OurModel{
public:
	int hitpoint;
	int hittime;

	Monster(const char *s, Point3D pos, double size, int hitpoint, 
		Point3D centerpos = Point3D(0, 0, 0), double xlength = 0, double ylength = 0, double zlength = 0) : 
		OurModel(s, pos, size, 1, centerpos, xlength, ylength, zlength), hittime(false), hitpoint(hitpoint){}
	void Hit() {
		--hitpoint;
		hittime = 5;
	}
	bool checkhit() {
		bool ret;
		if (hittime) {
			ret = true;
		}
		else ret = false;
		
		return ret;
	}
	int sethit() {
		if(hittime)	--hittime;
		return hittime;
	}
	int gethit() {
		int ret = hittime;
		return ret;
	}
	bool Checkhitpoint() {
		return hitpoint > 0;
	}
	bool Trymove(Point3D goal, double movelen) {
		if (Checkhitpoint() == false) return false;
		if((goal - pos).length() <= 0.5) {
			return true;
		}
		else {
			goal.y = pos.y;

			double nowdis = (goal - pos).length();
			int trymodified = rand() % 100 <= 1;
			bool check = abs(pos.x) > bordersize || abs(pos.z) > bordersize;
			if (trymodified) {
				int go = rand() % max(1, (int)(goal - pos).length());
				if (go <= 3 || check)
					Front = goal - pos;
				else
					Front = Point3D((rand() % 2 * 2 - 1) * (rand() % 50 + 1), 0, (rand() % 2 * 2 - 1) * (rand() % 50 + 1));
			}

			pos = pos + Front.resize(movelen);
			Front = Front.resize(1);
			Point3D temp = Point3D(0, 1, 0) * Front;
			if (sgn(temp.length()) != 0)Left = temp;
			Left = Left.resize(1);

			Up = Front * Left;
			Up = Up.resize(1);
		}
		return false;
	}
};

#endif