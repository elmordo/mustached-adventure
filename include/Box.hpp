/*
 * Box.hpp
 *
 *  Created on: 3.10.2013
 *      Author: petr
 */

#ifndef BOX_HPP_
#define BOX_HPP_

/**
 * nastavi novy posun roviny a definicnich bodu
 */
#define __SET_BOUNDARY(COORD_NAME, P1, P2, P3, P4) VolumeObject<T>::point(P1).COORD_NAME(p.COORD_NAME());\
	VolumeObject<T>::point(P2).COORD_NAME(p.COORD_NAME());\
	VolumeObject<T>::point(P3).COORD_NAME(p.COORD_NAME());\
	VolumeObject<T>::point(P4).COORD_NAME(p.COORD_NAME());

#include <cstring>

#include "VolumeObject.hpp"
#include "Plane.hpp"
#include "Vector3D.hpp"
#include "GEM.hpp"

namespace Gremlin {
namespace Math {

using namespace std;

template<class T>
class Box: public VolumeObject<T> {
public:

	/**
	 * vycet indexu definicnnich bodu
	 *
	 */
	enum POINT_INDEX {
		POINT_FUL = 0,
		POINT_FUR = 1,
		POINT_FBL = 2,
		POINT_FBR = 3,
		POINT_BUL = 4,
		POINT_BUR = 5,
		POINT_BBL = 6,
		POINT_BBR = 7
	};

	/**
	 * vycet indexu kontrolnich rovin
	 */
	enum PLANE_INDEX {
		PLANE_FRONT = 0,
		PLANE_BACK = 1,
		PLANE_LEFT = 2,
		PLANE_RIGHT = 3,
		PLANE_TOP = 4,
		PLANE_BOTTOM = 5
	};

	/**
	 * uchovava indexy tvorici hrany
	 */
	struct EDGE_POINTS {
		size_t p0;
		size_t p1;
	};

	/**
	 * uchovava indexy bodu tvoricich steny
	 */
	struct SIDE_POINTS {
		size_t p0;
		size_t p1;
		size_t p2;
	};

	/**
	 * pocet definicnich bodu
	 */
	static const int POINTS_COUNT = 8;

	/**
	 * pocet rovin
	 */
	static const int PLANE_COUNT = 6;

	/**
	 * pocet hran
	 */
	static const int EDGES_COUNT = 12;

	/**
	 * pocet stran
	 */
	static const int SIDES_COUNT = 6;

	/**
	 * seznam kombinaci bodu, ktere vytvareji hrany
	 */
	static const EDGE_POINTS EDGES[EDGES_COUNT];

	/**
	 * seznam kombinaci bodu, ktere tvori steny
	 */
	static const SIDE_POINTS SIDES[SIDES_COUNT];

private:

	/**
	 * kontrolni roviny
	 */
	Plane<T> pl[PLANE_COUNT];

	/**
	 * transformovane kontrolni roviny
	 */
	Plane<T> tpl[PLANE_COUNT];

	/**
	 * stredovy bod kvadru
	 */
	Point3D<T> c;

	/**
	 * radius opsane koule kvadru
	 */
	T r;

	/**
	 * prepinac inicializace
	 */
	bool init;

	/**
	 * zkontroluje, zda se bod nachazi nad rovinou
	 * pokud ano, posune rovinu a definicni body
	 */
	void checkPlane(const Point3D<T> &p, Plane<T>& pl, PLANE_INDEX pi) {
		// pokud se bod nachazi nad rovinou, dojde k posunuti roviny a definicniho bodu
		T d = pl.distance(p);

		if (d > 0 || !init) {
			// vyhodnoceni typu roviny a nastaveni dat
			switch (pi) {
			case PLANE_BACK:
				__SET_BOUNDARY(z, POINT_BBL, POINT_BBR, POINT_BUL, POINT_BUR)
				;
				break;

			case PLANE_FRONT:
				__SET_BOUNDARY(z, POINT_FBL, POINT_FBR, POINT_FUL, POINT_FUR)
				;
				break;

			case PLANE_BOTTOM:
				// nastaveni roviny
				__SET_BOUNDARY(y, POINT_FBL, POINT_FBR, POINT_BBL, POINT_BBR)
				;
				break;

			case PLANE_TOP:
				// nastaveni roviny
				__SET_BOUNDARY(y, POINT_FUL, POINT_FUR, POINT_BUL, POINT_BUR)
				;
				break;

			case PLANE_LEFT:
				__SET_BOUNDARY(x, POINT_FBL, POINT_FUL, POINT_BUL, POINT_BBL)
				;
				break;

			case PLANE_RIGHT:
				__SET_BOUNDARY(x, POINT_FBR, POINT_FUR, POINT_BUR, POINT_BBR)
				;
				break;
			}

			recalculatePlane(pi);
		}
	}

	/**
	 * prepocita novou polohu roviny
	 */
	void recalculatePlane(PLANE_INDEX pi) {
		switch (pi) {
		case PLANE_FRONT:
			pl[pi].moveToPoint(VolumeObject<T>::point(POINT_FBL));
			break;

		case PLANE_BACK:
			pl[pi].moveToPoint(VolumeObject<T>::point(POINT_BBL));
			break;

		case PLANE_LEFT:
			pl[pi].moveToPoint(VolumeObject<T>::point(POINT_FBL));
			break;

		case PLANE_RIGHT:
			pl[pi].moveToPoint(VolumeObject<T>::point(POINT_FBR));
			break;

		case PLANE_TOP:
			pl[pi].moveToPoint(VolumeObject<T>::point(POINT_FUL));
			break;

		case PLANE_BOTTOM:
			pl[pi].moveToPoint(VolumeObject<T>::point(POINT_FBL));
			break;
		}
	}

	/**
	 * zkontroluje, zda nektery z bodu b1 lezi uvnitr b2
	 * pokud ano, vraci TRUE, pokud ne, vraci FALSE
	 */
	static bool checkIntersect(const Point3D<T> &tp0, const Point3D<T> &tp1,
			const Point3D<T> &op0, const Point3D<T> &op1,
			const Point3D<T> &op2) {

		// vygenerovani vektoru urceneho mistnimi body
		Vector3D<T> e = tp1 - tp0;

		// vygenerovani dvou vektoru urcujicich rovinu
		Vector3D<T> pl1 = op1 - op0;
		Vector3D<T> pl2 = op2 - op0;

		// vygenerovani matice pro solver
		Matrix<T> A(3, 4, 0);

		// separace segmentu
		MatrixSegment<T> x = A[0], y = A[1], z = A[2];

		// naplneni rovnic pro X
		x[0] = pl1[0];
		x[1] = pl2[0];
		x[2] = -e[0];
		x[3] = op0.x() - tp0.x();

		// naplneni rovnic pro y
		y[0] = pl1[1];
		y[1] = pl2[1];
		y[2] = -e[1];
		y[3] = op0.y() - tp0.y();

		// naplneni rovnic pro Z
		z[0] = pl1[2];
		z[1] = pl2[2];
		z[2] = -e[2];
		z[3] = op0.z() - tp0.z();

		// priprava solveru
		GEM<T> solver;

		Matrix<T> r;

		try {
			r = solver.solve(A);
		} catch (EqSolverException &e) {
			// pokud byla zachycena vyjimky, tato kombinace se neprotina
			return false;
		}

		// aby doslo k pruniku, musi byt vsechny parametry v intervalu 0 az 1
		T rX = r.cellValue(0, 0), rY = r.cellValue(1, 0), rZ = r.cellValue(2,
				0);

		return rX >= 0 && rX <= 1 && rY >= 0 && rY <= 1 && rZ >= 0 && rZ <= 1;
	}

	/**
	 * zkontroluje body prvniho kvadru, zda lezu uvnitr druheho kvadru
	 */
	static bool checkPoints(const Box<T> &pointBox, const Box<T> &planeBox) {
		for (size_t i = 0; i < POINTS_COUNT; i++) {
			const Point3D<T> &p = pointBox.tPoint(i);

			bool thisIter = true;

			for (size_t j = 0; j < PLANE_COUNT && thisIter; j++) {
				const Plane<T> &pl = planeBox.tPlane(j);

				T dist = pl.direction(p);

				if (dist > 0) thisIter = false;
			}

			if (thisIter) return true;
		}

		return false;
	}

public:

	/**
	 * bezparametricky konstruktor
	 * zavola rodicvosky konstruktor, ktery vytvori pole definicncih bodu
	 */
	Box() :
			VolumeObject<T>(POINTS_COUNT) {

		pl[PLANE_FRONT] = Plane<T>(Plane<T>::FRONT);
		pl[PLANE_BACK] = Plane<T>(Plane<T>::BACK);
		pl[PLANE_BOTTOM] = Plane<T>(Plane<T>::BOTTOM);
		pl[PLANE_TOP] = Plane<T>(Plane<T>::TOP);
		pl[PLANE_LEFT] = Plane<T>(Plane<T>::LEFT);
		pl[PLANE_RIGHT] = Plane<T>(Plane<T>::RIGHT);

		r = 0;

		init = false;
	}

	/**
	 * kopytor
	 */
	Box(const Box<T> &o) :
			VolumeObject<T>(POINTS_COUNT) {
		// prekopirovani dat rovin
		memcpy(pl, o.pl, sizeof(Plane<T> ) * PLANE_COUNT);

		init = o.init;
		r = o.r;
		c = o.c;
	}

	/**
	 * destrutkor - nic nedela
	 */
	virtual ~Box() {

	}

	const Point3D<T> &center() const {
		return c;
	}

	/**
	 * zkontroluje prunik objektu
	 */
	virtual bool checkIntersect(const VolumeObject<T> &o) const {
		/*
		 * nyni muzeme bezobav pretypovat parametr na box
		 * v budoucnu se bude muset pres RTTI zjistovat typ
		 */
		const Box<T> &b = dynamic_cast<const Box<T> &>(o);

		// kontrola kouli :)
		T d = (b.c - this->c).abs();		// vzdalenost stredu kvadru

		if (d - (b.r + r) > 0) {
			// pokud je rozdil vzdalenosti a souctu prumeru vetsi nez nula
			// pak k pruniku nemuze dojit
			return false;
		}

		// kotrnola bodu jednoho kvadru uvnitr druheho kvadru
		//if (checkPoints(*this, b)) return true;
		//if (checkPoints(b, *this)) return true;

		// kontrola sten
		for (int i = 0; i < SIDES_COUNT; i++) {
			const Point3D<T> &op0 = b.tPoint(Box<T>::SIDES[i].p0);
			const Point3D<T> &op1 = b.tPoint(Box<T>::SIDES[i].p1);
			const Point3D<T> &op2 = b.tPoint(Box<T>::SIDES[i].p2);

			// kontrola hran
			for (int j = 0; j < EDGES_COUNT; j++) {
				// nacteni indexu bodu hrany
				size_t pi0 = Box<T>::EDGES[j].p0;
				size_t pi1 = Box<T>::EDGES[j].p1;

				// reference na body hrany
				const Point3D<T> &tp0 = VolumeObject<T>::tPoint(pi0);
				const Point3D<T> &tp1 = VolumeObject<T>::tPoint(pi1);

				// pokud se hrana protina se stranou, pak doslo k pruniku
				if (checkIntersect(tp0, tp1, op0, op1, op2))
					return true;
			}
		}

		return false;
	}

	/**
	 * zkontroluje bod, jestli se nachazi uvnitr kvadru
	 * pokud ne, posune hranice kvadru
	 */
	virtual void checkPoint(const Point3D<T> &p) {
		// iterace nad vsemi rovinami
		for (int i = 0; i < PLANE_COUNT; i++) {
			// kontrola, zda se bod nachazi nad rovinou
			checkPlane(p, pl[i], static_cast<PLANE_INDEX>(i));
		}

		// pokud jsou roviny oznaceny jako neinicializovane, oznaci se jako inicializovane
		init = true;
	}

	virtual void checkPoint(T x, T y, T z) {
		checkPoint(Point3D<T>(x, y, z));
	}

	/**
	 * vraci ohranicujici rovinu
	 */
	const Plane<T> &plane(size_t index) const {
		return pl[index];
	}

	/**
	 * vraci transformovanou rovinu
	 */
	const Plane<T> &tPlane(size_t index) const {
		return tpl[index];
	}

	/**
	 * vraci polemr opsane koule
	 */
	T radius() const {
		return r;
	}

	/**
	 * oznaceni ohranicujici roviny jako neinicializovane
	 */
	virtual void reset() {
		init = false;
	}

	virtual void transform() {
		// zavolani metody predka
		VolumeObject<T>::transform();

		// prepocitani vsech bodu
		tpl[PLANE_FRONT].setFromPoints(
				VolumeObject<T>::tPoint(POINT_FBL),
				VolumeObject<T>::tPoint(POINT_FBR),
				VolumeObject<T>::tPoint(POINT_FUL)
				);

		tpl[PLANE_BACK].setFromPoints(
				VolumeObject<T>::tPoint(POINT_BBL),
				VolumeObject<T>::tPoint(POINT_BBR),
				VolumeObject<T>::tPoint(POINT_BUL)
				);

		tpl[PLANE_TOP].setFromPoints(
				VolumeObject<T>::tPoint(POINT_FUL),
				VolumeObject<T>::tPoint(POINT_FUR),
				VolumeObject<T>::tPoint(POINT_BUL)
				);

		tpl[PLANE_BOTTOM].setFromPoints(
				VolumeObject<T>::tPoint(POINT_FBL),
				VolumeObject<T>::tPoint(POINT_FBR),
				VolumeObject<T>::tPoint(POINT_BBL)
				);

		tpl[PLANE_LEFT].setFromPoints(
				VolumeObject<T>::tPoint(POINT_FBL),
				VolumeObject<T>::tPoint(POINT_FUL),
				VolumeObject<T>::tPoint(POINT_BBL)
				);

		tpl[PLANE_RIGHT].setFromPoints(
				VolumeObject<T>::tPoint(POINT_FBR),
				VolumeObject<T>::tPoint(POINT_FUR),
				VolumeObject<T>::tPoint(POINT_BBR)
				);

		// prepocet stredoveho bodu a polomeru
		c = this->tPoint(POINT_FBL);

		// protejsi bod
		const Point3D<T> &op = this->tPoint(POINT_BUR);

		// posunuti stredu
		c.x(c.x() + (op.x() + c.x()) / 2);
		c.y(c.y() + (op.y() + c.y()) / 2);
		c.z(c.z() + (op.z() + c.z()) / 2);

		// vypocet polomeru
		r = (this->tPoint(POINT_FBL) - this->tPoint(POINT_BUR)).abs() / 2;
	}
};

template<class T>
const typename Box<T>::EDGE_POINTS Box<T>::EDGES[12] = {
// predek
		{ Box<T>::POINT_FBL, Box<T>::POINT_FBR }, { Box<T>::POINT_FBL,
				Box<T>::POINT_FUL }, { Box<T>::POINT_FBR, Box<T>::POINT_FUR }, {
				Box<T>::POINT_FUL, Box<T>::POINT_FUR },

		// zadek
		{ Box<T>::POINT_BBL, Box<T>::POINT_BBR }, { Box<T>::POINT_BBL,
				Box<T>::POINT_BUL }, { Box<T>::POINT_BBR, Box<T>::POINT_BUR }, {
				Box<T>::POINT_BUL, Box<T>::POINT_BUR },

		// prostredek
		{ Box<T>::POINT_FBL, Box<T>::POINT_BBL }, { Box<T>::POINT_FBR,
				Box<T>::POINT_BBR }, { Box<T>::POINT_FUL, Box<T>::POINT_BUL }, {
				Box<T>::POINT_FUR, Box<T>::POINT_BUR } };

template<class T>
const typename Box<T>::SIDE_POINTS Box<T>::SIDES[6] = {
// predni
		{ Box<T>::POINT_FBL, Box<T>::POINT_FBR, Box<T>::POINT_FUL },

		// zadni
		{ Box<T>::POINT_BBL, Box<T>::POINT_BBR, Box<T>::POINT_BUL },

		// horni
		{ Box<T>::POINT_FUL, Box<T>::POINT_FUR, Box<T>::POINT_BUL },

		// spodni
		{ Box<T>::POINT_FBL, Box<T>::POINT_FBR, Box<T>::POINT_BBL },

		// leva
		{ Box<T>::POINT_FBL, Box<T>::POINT_FUL, Box<T>::POINT_BBL },

		// prava
		{ Box<T>::POINT_FBR, Box<T>::POINT_FUR, Box<T>::POINT_BBR } };

}
}

#endif /* BOX_HPP_ */
