/*
 * Transform3D.hpp
 *
 *  Created on: 1.8.2013
 *      Author: petr
 */

#ifndef TRANSFORM3D_HPP_
#define TRANSFORM3D_HPP_

#include "Matrix.hpp"
#include "Point3D.hpp"
#include <GL/gl.h>
#include <cmath>
#include <cstdarg>

namespace Gremlin {
namespace Math {

template<class T>
class Transform3D: public Matrix<T> {

	/**
	 * vynasobi dany sloupec konstantou
	 *
	 * rs a re jsou pocatecni a koncovy radek
	 */
	void multiplyColl(size_t c, size_t rs, size_t re, T k) {
		// nasobeni prvku
		for (size_t i = rs; i < re; i++) {
			// vynasobeni i-teho radku a c-teho sloupce hodnotou k
			Matrix<T>::cellValue(i, c, k * Matrix<T>::cellValue(i, c));
		}
	}

	/**
	 * inicializuje matici nevyvolavajici zadne transformace
	 */
	void initEmptyMatrix() {
		Matrix<T>::cellValue(0, 0, 1.0);
		Matrix<T>::cellValue(1, 1, 1.0);
		Matrix<T>::cellValue(2, 2, 1.0);
		Matrix<T>::cellValue(3, 3, 1.0);
	}

	/**
	 * vytvori matici z pameti
	 */
	Transform3D(T *mem) :
			Matrix<T>(4, 4, 0) {
		Matrix<T>::rawData(mem);
	}

public:

	enum TRANSFORM_TYPE {
		TRANSLATION,
		SCALE,
		SCALE_X,
		SCALE_Y,
		SCALE_Z,
		SCALE_SYMETRIC,
		ROT_X,
		ROT_Y,
		ROT_Z
	};

	/**
	 * vytvori nic nedelajici matici
	 */
	Transform3D() :
			Matrix<T>(4, 4, 0.0f) {

		// inicializace dat
		initEmptyMatrix();
	}

	/**
	 * vytvori matici pro danou transformaci
	 */
	Transform3D(Transform3D::TRANSFORM_TYPE t, ...) :
			Matrix<T>(4, 4, 0) {
		// nastaveni vychozi matice
		initEmptyMatrix();

		// inicializace podpory vice argumentu
		va_list argList;
		va_start(argList, t);

		// vyhodnoceni transformace
		GLdouble x, y, z;

		switch (t) {
		case TRANSLATION:
			x = va_arg(argList, double);
			y = va_arg(argList, double);
			z = va_arg(argList, double);

			Matrix<T>::cellValue(3, 0, x);
			Matrix<T>::cellValue(3, 1, y);
			Matrix<T>::cellValue(3, 2, z);
			break;

		case SCALE:
			x = va_arg(argList, double);
			y = va_arg(argList, double);
			z = va_arg(argList, double);

			Matrix<T>::cellValue(0, 0, x);
			Matrix<T>::cellValue(1, 1, y);
			Matrix<T>::cellValue(2, 2, z);
			break;

		case ROT_X:
			x = va_arg(argList, double);
			y = cos(x);
			z = sin(x);

			// nastaveni hodnot
			Matrix<T>::cellValue(1, 1, y);
			Matrix<T>::cellValue(1, 2, -z);
			Matrix<T>::cellValue(2, 1, z);
			Matrix<T>::cellValue(2, 2, y);
			break;

		case ROT_Y:
			x = va_arg(argList, double);
			y = cos(x);
			z = sin(x);

			// nastaveni hodnot
			Matrix<T>::cellValue(0, 0, y);
			Matrix<T>::cellValue(0, 2, -z);
			Matrix<T>::cellValue(2, 0, z);
			Matrix<T>::cellValue(2, 2, y);
			break;

		case ROT_Z:
			x = va_arg(argList, double);
			y = cos(x);
			z = sin(x);

			// nastaveni hodnot
			Matrix<T>::cellValue(0, 0, y);
			Matrix<T>::cellValue(0, 1, -z);
			Matrix<T>::cellValue(1, 0, z);
			Matrix<T>::cellValue(1, 1, y);
			break;

		case SCALE_X:
			x = va_arg(argList, double);
			Matrix<T>::cellValue(0, 0, x);
			break;

		case SCALE_Y:
			y = va_arg(argList, double);
			Matrix<T>::cellValue(1, 1, y);
			break;

		case SCALE_Z:
			z = va_arg(argList, double);
			Matrix<T>::cellValue(2, 2, z);
			break;

		case SCALE_SYMETRIC:
			x = va_arg(argList, double);
			Matrix<T>::cellValue(0, 0, x);
			Matrix<T>::cellValue(1, 1, x);
			Matrix<T>::cellValue(2, 2, x);
			break;
		}

		// deinicializace argumentu
		va_end(argList);
	}

	/**
	 * kopytor
	 */
	Transform3D(const Transform3D &o) :
			Matrix<T>(o) {

	}

	/**
	 * na teto urovni nedela nic
	 */
	virtual ~Transform3D() {

	}

	/**
	 * provede rotaci kolem osy X
	 */
	Transform3D<T> &rotateX(T a, bool prep = false) {
		// vytvoreni pomocne matice
		Transform3D<T> helper(ROT_X, a);

		// dealokace stare pameti a nastaveni nove
		if (prep) {
			Matrix<T>::newMem(Matrix<T>::productMem(helper, *this));
		} else {
			Matrix<T>::newMem(Matrix<T>::productMem(*this, helper));
		}

		return *this;
	}

	/**
	 * provede rotaci kolem osy Y
	 */
	Transform3D<T> &rotateY(T a, bool prep = false) {
		// vytvoreni pomocne matice
		Transform3D<T> helper(ROT_Y, a);

		// dealokace stare pameti a nastaveni nove
		if (prep) {
			Matrix<T>::newMem(Matrix<T>::productMem(helper, *this));
		} else {
			Matrix<T>::newMem(Matrix<T>::productMem(*this, helper));
		}

		return *this;
	}

	/**
	 * provede rotaci kolem osy Z
	 */
	Transform3D<T> &rotateZ(T a, bool prep = false) {
		Transform3D<T> helper(ROT_Z, a);

		// dealokace stare pameti a nastaveni nove
		if (prep) {
			Matrix<T>::newMem(Matrix<T>::productMem(helper, *this));
		} else {
			Matrix<T>::newMem(Matrix<T>::productMem(*this, helper));
		}

		return *this;
	}

	/**
	 * provede symetrickou zmenu meritka
	 */
	Transform3D<T> &scale(T s, bool prep = false) {
		// zavolame obecnou zmenu meritka se stejnymi hodnotami pro x, y i z
		scale(s, s, s, prep);

		return *this;
	}

	/**
	 * provede zmenu meritka v ruznych osach
	 */
	Transform3D<T> &scale(T x, T y, T z, bool prep = false) {
		// nasobeni sloupcu
		Transform3D<T> helper(SCALE, x, y, z);

		// dealokace stare pameti a nastaveni nove
		if (prep) {
			Matrix<T>::newMem(Matrix<T>::productMem(helper, *this));
		} else {
			Matrix<T>::newMem(Matrix<T>::productMem(*this, helper));
		}

		return *this;
	}

	/**
	 * provede translaci (relativne od aktualniho bodu)
	 */
	Transform3D<T> &translate(T x, T y, T z, bool prep = false) {

		Transform3D<T> helper(TRANSLATION, x, y, z);

		// dealokace stare pameti a nastaveni nove
		if (prep) {
			Matrix<T>::newMem(Matrix<T>::productMem(helper, *this));
		} else {
			Matrix<T>::newMem(Matrix<T>::productMem(*this, helper));
		}

		return *this;
	}

	/**
	 * nasobeni transformacnich matic
	 */
	Transform3D<T> operator*(const Transform3D<T> &o) const {
		T *mem = Matrix<T>::productMem(*this, o);

		return Transform3D<T>(mem);
	}

	/**
	 * nasobeni transformacnich matic
	 */
	Point3D<T> operator*(const Point3D<T> &o) const {
		T *mem = Matrix<T>::productMem(*this, o);

		return Point3D<T>(mem);
	}

};

// definice typu
typedef Transform3D<GLfloat> Transform3Df;
typedef Transform3D<GLdouble> Transform3Dd;

} /* namespace Math */
} /* namespace Gremlin */
#endif /* TRANSFORM3D_HPP_ */
