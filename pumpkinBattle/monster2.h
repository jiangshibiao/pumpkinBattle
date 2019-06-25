#pragma once
#include "camera.h"
#include "Wall.h"
#include "Model.h"

class Monster : public OurModel{
public:
	Monster(const char *s, Point3D pos, double size) : OurModel(s, pos, size) {}
	void Trymove(Point3D goal, double movelen) {
		goal.y = pos.y; dir = goal - pos;
		if (dir.length() > 0.5) pos = pos + dir.resize(movelen);
	}
};