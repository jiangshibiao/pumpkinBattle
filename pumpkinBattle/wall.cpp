#include "wall.h"

vector<Wall*> WallManager;

void Wall::Addface(Point3D A, Point3D B, Point3D C, Point3D D) {
	allface.emplace_back(A);
	allface.emplace_back(B);
	allface.emplace_back(C);
	allface.emplace_back(D);
}

void Wall::Display(vector<Cube> &trans) {
	allface.clear();
	Point3D discenter = center + (px - center) + (py - center) + (pz - center);

	Point3D qx = discenter - (px - center);
	Point3D qy = discenter - (py - center);
	Point3D qz = discenter - (pz - center);

	//DrawRect(center, pz, qx, py, vertices + 0);
	//DrawRect(px, qz, discenter, qy, vertices + 36);
	//普通墙默认不画下底面
	//DrawRect(py, qx, discenter, qz, vertices + 72);

	double tx = 1.0 / accx;
	double ty = 1.0 / accy;

	Point3D dirx = px - center;
	Point3D diry = py - center;
	Point3D dirz = pz - center;

	//printf("%.2f %.2f\n", dirx.x, dirz.z);


	for (int i = 0; i < accx; i++)
		for (int j = 0; j < accy; j++) {
			if (face[i][j].live == 0) continue;
			//if ((i + j) & 1) continue;
			//if (i == 0 && j == 1) continue;
			Point3D A = center + dirx * (i * tx) + diry * (j * ty);
			Point3D B = center + dirx * (i * tx) + diry * ((j + 1) * ty);
			Point3D C = center + dirx * ((i + 1) * tx) + diry * ((j + 1) * ty);
			Point3D D = center + dirx * ((i + 1) * tx) + diry * (j * ty);

			Addface(A, B, C, D);

			Addface(A + dirz, D + dirz, C + dirz, B + dirz);

			if (i + 1 == accx || face[i + 1][j].live == 0)
				Addface(D, C, C + dirz, D + dirz);
			if (j + 1 == accy || face[i][j + 1].live == 0)
				Addface(C, B, B + dirz, C + dirz);
			if (i == 0 || face[i - 1][j].live == 0)
				Addface(B, A, A + dirz, B + dirz);
			if (j == 0 || face[i][j - 1].live == 0)
				Addface(A, D, D + dirz, A + dirz);
			trans.emplace_back(Cube(center + dirx * ((i + 0.5) * tx) + diry * ((j + 0.5) * ty) + dirz * 0.5, type,
				diry.length() / accy * 0.5, Point3D((i % accy) * 1.0 / accy, j * 1.0 / accy, 1.0 / accy)));
		}
}

pair<double, pair<Wall*, pair<int, int>>> Wall::clicked(const Point3D &circenter, const Point3D &direction) {//returns inf when not clicked
		//assert(sgn(direction.length())!=0);
		//proj to 2D
	Point2D projcenter(
		(circenter - center) % (px - center) / xlength,
		(circenter - center) % (py - center) / ylength
	);
	double projdis = pointtoplanedistance(circenter, center, px, py);
	Point3D temp = (px - center) * (py - center) / xlength / ylength;
	double clickdis = (direction % temp) / direction.length();//direction.length() == 1? what?
	if (projdis < 0) {
		projdis = -projdis - zlength;
		if (projdis < 0) {
			projdis = 0;
			return make_pair(inf, make_pair(nullptr, make_pair(-1, -1)));//Walls can not be clicked when you are inside of it; Should be handled separately?
		}
		if (sgn(clickdis) >= 0)return make_pair(inf, make_pair(nullptr, make_pair(-1, -1)));
		clickdis = -clickdis;
	}
	else {
		if (sgn(clickdis) <= 0)return make_pair(inf, make_pair(nullptr, make_pair(-1, -1)));
	}
	//intersection
	Point2D intersection(
		direction % (px - center) / xlength / direction.length() * projdis / clickdis,//direction.length() == 1?
		direction % (py - center) / ylength / direction.length() * projdis / clickdis
	);
	intersection = intersection + projcenter;
	if (intersection.x < 0 || intersection.x >= xlength || intersection.y < 0 || intersection.y >= ylength)
		return make_pair(inf, make_pair(nullptr, make_pair(-1, -1)));

	if (face[trunc(intersection.x / (xlength / accx))][trunc(intersection.y / (ylength / accy))].live == 0)
		return make_pair(inf, make_pair(nullptr, make_pair(-1, -1)));
	return make_pair(
		projdis / clickdis,
		make_pair(
			this,
			make_pair(
				(int)trunc(intersection.x / (xlength / accx)),
				(int)trunc(intersection.y / (ylength / accy))
			)
		)
	);
}

void Wall::filp(int x, int y) {
	if (x < 0 || x >= accx || y < 0 || y >= accy) return;
	face[x][y].live = 0;
}
void Wall::bkdown(int x, int y) {
	filp(x, y);
	filp(x - 1, y);filp(x + 1, y);filp(x, y - 1);filp(x, y + 1);
	if (rand() % 3 == 0)filp(x - 1, y - 1);
	if (rand() % 3 == 0)filp(x + 1, y - 1);
	if (rand() % 3 == 0)filp(x - 1, y + 1);
	if (rand() % 3 == 0)filp(x + 1, y + 1);
}