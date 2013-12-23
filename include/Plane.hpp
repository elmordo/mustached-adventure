/*
 * Plane.hpp
 *
 *  Created on: 26.9.2013
 *      Author: petr
 */

#ifndef PLANE_HPP_
#define PLANE_HPP_

#include<cstring>
#include <cmath>
#include <MathException.hpp>
#include <Vector3D.hpp>

namespace Gremlin {
namespace Math {

using namespace std;

template<class T>
class Plane {

	/**
	 * pamet s koeficienty
	 */
	T coefs[4];

public:

	/**
	 * pristupove koeficienty k parametrum roviny
	 */
	enum {
		COEF_A = 0, COEF_B = 1, COEF_C = 2, COEF_D = 3
	};

	enum PLANE_TYPE {
		FRONT, BACK, TOP, BOTTOM, LEFT, RIGHT
	};

	Plane() {
		coefs[COEF_A] = 0;
		coefs[COEF_B] = 0;
		coefs[COEF_C] = 0;
		coefs[COEF_D] = 0;
	}

	/**
	 * vytvori rovinu prochazejici pocatekem odpovidajici specifikaci
	 */
	Plane(PLANE_TYPE which) {
		// anulace
		coefs[COEF_A] = 0;
		coefs[COEF_B] = 0;
		coefs[COEF_C] = 0;
		coefs[COEF_D] = 0;

		// nastaveni jednicky nebo minus jednicky dle pozadavku
		switch (which) {
		case FRONT:
			coefs[COEF_C] = 1;
			break;

		case BACK:
			coefs[COEF_C] = -1;
			break;

		case TOP:
			coefs[COEF_B] = 1;
			break;

		case BOTTOM:
			coefs[COEF_B] = -1;
			break;

		case LEFT:
			coefs[COEF_A] = -1;
			break;

		case RIGHT:
			coefs[COEF_A] = 1;
			break;
		}
	}

	/**
	 * vytvori rovinu z normaloveho bodu a posunuti
	 */
	Plane(const Vector3D<T> &n, T d) {
		coefs[COEF_A] = n[0];
		coefs[COEF_B] = n[1];
		coefs[COEF_C] = n[2];
		coefs[COEF_D] = d;
	}

	/**
	 * vytvori rovinu z koeficientu
	 */
	Plane(T a, T b, T c, T d) {
		coefs[COEF_A] = a;
		coefs[COEF_B] = b;
		coefs[COEF_C] = c;
		coefs[COEF_D] = d;
	}

	/**
	 * kopytor
	 */
	Plane(const Plane<T> &o) {
		memcpy(coefs, o.coefs, 4 * sizeof(T));
	}

	/**
	 * vraci koeficient
	 */
	T coef(int c) const {
		return coefs[c];
	}

	/**
	 * nastavi koeficient
	 */
	void coef(int c, T val) {
		coefs[c] = val;
	}

	/**
	 * vraci cislo reprezentujici smer, kterym bod lezi od roviny:
	 * - kladne cislo pro bod nad rovinou
	 * - nulu pro bod na rovine
	 * - zaporne cislo pro bod pod rovinou
	 */
	T direction(const Point3D<T> &p) const {
		return p.x() * coefs[COEF_A] + p.y() * coefs[COEF_B] + p.z() + coefs[COEF_C] + coefs[COEF_D];
	}

	/**
	 * vraci vzdalenost bodu od roviny
	 * pokud je vzdalenost kladna, pak je bod nad rovinou
	 * pokud je vzdalenost zaporna, pak je bod pod rovinou
	 */
	T distance(const Point3D<T> &p) const {
		T up = p.x() * coefs[COEF_A] + p.y() * coefs[COEF_B]
				+ p.z() * coefs[COEF_C] + coefs[COEF_D];
		T down = sqrt(
				pow(coefs[COEF_A], 2.0) + pow(coefs[COEF_B], 2.0)
						+ pow(coefs[COEF_C], 2.0));

		return up / down;
	}

	/**
	 * vraci True, pokud jsou roviny identicke
	 */
	bool isIdentical(const Plane<T> &o) const {
		// musi platit rovnobeznost
		if (!isParalel(o))
			return false;

		// bod nasobek souradnic a D se musi rovnat
		for (int i = 0; i < 3; i++) {
			if (o.coefs[i] * o.coefs[COEF_D] != coefs[i] * coefs[COEF_D])
				return false;
		}

		return true;
	}

	/**
	 * vraci True, pokud bod nalezi rovine
	 */
	bool isIdentical(const Point3D<T> &p) const {
		// po dosazeni souradnic bodu do rovnice musi byt vysledek nula
		T result = coefs[COEF_A] * p.x() + coefs[COEF_B] * p.y()
				+ coefs[COEF_C] * p.z() + coefs[COEF_D];

		return result == 0;
	}

	/**
	 * vraci True, pokud jsou roviny rovnobezne
	 */
	bool isParalel(const Plane<T> &o) const {
		// vypocet vektoroveho soucinu
		T ca = coefs[COEF_B] * o.coefs[COEF_C]
				- coefs[COEF_C] * o.coefs[COEF_B];
		T cb = coefs[COEF_A] * o.coefs[COEF_C]
				- coefs[COEF_C] * o.coefs[COEF_A];
		T cc = coefs[COEF_A] * o.coefs[COEF_B]
				- coefs[COEF_B] * o.coefs[COEF_A];

		// pokud jsou vsechny slozky nulove, pak se jedna o rovnobezne roviny
		return ca == 0 && cb == 0 && cc == 0;
	}

	/**
	 * posune rovinu tak, aby prochazela bodem
	 */
	void moveToPoint(const Point3D<T> &p) {
		coefs[COEF_D] = - p.x() * coefs[COEF_A] - p.y() * coefs[COEF_B] - p.z() * coefs[COEF_C];
	}

	/**
	 * nastavi parametry roviny ze trech bodu
	 */
	void setFromPoints(const Point3D<T> &p1, const Point3D<T> &p2, const Point3D<T> p3) {
		// vypcoet normaloveho vektoru
		Vector3D<T> v1 = p2 - p1;
		Vector3D<T> v2 = p3 - p1;

		Vector3D<T> n = v1 * v2;

		n.normalise();

		coefs[COEF_A] = n[0];
		coefs[COEF_B] = n[1];
		coefs[COEF_C] = n[2];

		// vypocet koeficientu D
		coefs[COEF_D] = - p1.x() * n[0] - p1.y() * n[1] - p1.z() * n[2];
	}

	/**
	 * prepocita koeficienty tak, aby slozky normaly A, B a C tvorili jednotkovy vektor
	 */
	void unify() {
		T l = sqrt(
				pow(coefs[COEF_A], 2) + pow(coefs[COEF_B], 2)
						+ pow(coefs[COEF_C], 2));

		// pokud l == 0, pak je nekde chyba
		if (l == 0) THROW_EXCEPTION(MathException, "Plane zero normal vector", 1, 0x0);

		// vsechny ostatni koeficienty jsou vynasobeny D a D je nasledne rovno jedne
		coefs[COEF_A] /= l;
		coefs[COEF_B] /= l;
		coefs[COEF_C] /= l;
		coefs[COEF_D] /= l;
	}
};

}
}

#endif /* PLANE_HPP_ */
