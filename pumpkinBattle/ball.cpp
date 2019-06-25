#include "ball.h"
#include "monster.h"

vector<Ball*> BallManager;

pair<double, Point3D> Ball::collide(const Point3D &A, const Point3D &C, const Point3D &B) const {//this is reversed for some reason...
	double projdis = pointtoplanedistance(posi, A, B, C);
	if (projdis < 0.01)return make_pair(1, velo);
	if (pointtoplanedistance(posi + velo, A, B, C) > 0.25)return make_pair(1, velo);
	Point3D temp = ((C - A) * (B - A)).resize(1);
	if (sgn(pointtoplanedistance(posi, A, B, posi + velo)) < 0)return make_pair(1, velo);
	if (sgn(pointtoplanedistance(posi, B, C, posi + velo)) < 0)return make_pair(1, velo);
	if (sgn(pointtoplanedistance(posi, C, A, posi + velo)) < 0)return make_pair(1, velo);
	return make_pair((projdis - 0.25) / -(velo%temp), velo - temp.resize(velo%temp * 2));
}

void Ball::CollBalls(){
	for (Ball *pball : BallManager) {
		if (pball->posi.y > 10)pball->exstime = 0;
		if (pball->exstime <= 0)continue;//this should be deleted???
		pball->exstime -= 1;
		if (sgn(pball->velo.length()) <= 0) {
			//pball->velo = pball->velo + Point3D(0., -0.001, 0.);
			continue;
		}
		//printf("%d ", ++cnt); pball->posi.Print(); puts("");

		OurModel *phit = nullptr;
		pair<double, Point3D>coll = make_pair(1, pball->velo);

		pair<double, Point3D>tmp = pball->collide(Point3D(-100, -0.5, -100), Point3D(-100, -0.5, 100), Point3D(100, -0.5, 100));
		if (tmp.first < coll.first)coll = tmp, phit = nullptr;
		tmp = pball->collide(Point3D(100, -0.5, 100), Point3D(100, -0.5, -100), Point3D(-100, -0.5, -100));
		if (tmp.first < coll.first)coll = tmp, phit = nullptr;

		for (Wall *pwall : WallManager) {
			int siz = pwall->allface.size();
			for (int i = 0; i < siz; i += 4) {
				pair<double, Point3D>temp = pball->collide(pwall->allface[i], pwall->allface[i + 1], pwall->allface[i + 2]);
				if (temp.first < coll.first)coll = temp, phit = nullptr;
				temp = pball->collide(pwall->allface[i + 2], pwall->allface[i + 3], pwall->allface[i]);
				if (temp.first < coll.first)coll = temp, phit = nullptr;
			}
		}
		int cnt = 0, now = -1;
		for (OurModel *pourmodel : ModelManager) {
			if (pourmodel->typ != 0) {
				if (pourmodel->Checkhitpoint() == false) continue;
			}
			int siz = pourmodel->allface.size();
			++cnt;
			for (int i = 0; i < siz; i += 3) {
				pair<double, Point3D>temp = pball->collide(
					pourmodel->pos + pourmodel->allface[i], 
					pourmodel->pos + pourmodel->allface[i + 1], 
					pourmodel->pos + pourmodel->allface[i + 2]);
				if (temp.first < coll.first)coll = temp, phit = pourmodel, now = cnt;
			}
		}
		//if(now!=-1)printf("%d\n", now);
		if (phit == nullptr || phit->typ == 0) {//wall or obj
			//if (phit != nullptr)printf("typ:%d\n", phit->typ);
			pball->posi = pball->posi + pball->velo * coll.first;
			pball->velo = coll.second;
		}
		else {//monster
			puts("Hit");
			pball->exstime = 0;
			phit->Hit();
		}

	}
}
