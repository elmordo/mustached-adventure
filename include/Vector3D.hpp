/*
 * Vector3D.hpp
 *
 *  Created on: 26.9.2013
 *      Author: petr
 */

#ifndef VECTOR3D_HPP_
#define VECTOR3D_HPP_

#include "Vector.hpp"
#include <cmath>

namespace Gremlin {
namespace Math {

template<class T> class Point3D;

template<class T>
class Vector3D : public Vector<T> {

public:

	/**
	 * vytvori nulovy vektor
	 */
	Vector3D() : Vector<T>(3) {

	}

	/**
	 * vytvori vektor z bodu
	 */
	Vector3D(const Point3D<T>& p) : Vector<T>(3) {
		T* data = Matrix<T>::rawData();

		data[0] = p.x();
		data[1] = p.y();
		data[2] = p.z();
	}

	/**
	 * vytvori vektor ze slozek
	 */
	Vector3D(T x, T y, T z) : Vector<T>(3) {
		T* data = Matrix<T>::rawData();
		data[0] = x;
		data[1] = y;
		data[2] = z;
	}

	/**
	 * destruktor
	 */
	virtual ~Vector3D() {

	}

	/**
	 * vektorovy soucin vektoru
	 */
	Vector3D<T> operator *(const Vector3D<T>& o) {
		Vector3D<T> retVal;

		const T *data1 = Matrix<T>::rawData();
		const T *data2 = o.rawData();

		retVal[0] = data1[1] * data2[2] - data1[2] * data2[1];
		retVal[1] = data1[0] * data2[2] - data1[2] * data2[0];
		retVal[2] = data1[0] * data2[1] - data1[1] * data2[0];

		return retVal;
	}
};

/**
 * matice 4x1 popisujici bod v prostoru
 * radky 0 az 2 jsou slozky vektoru
 * radek 3 slouzi k nevim cemu
 */
template <class T>
class Point3D: public Matrix<T> {
public:

	/**
	 * vytvori standardni 3rozmerny bod
	 */
	Point3D() : Matrix<T>(4, 1, 0) {
		Matrix<T>::cellValue(3, 0, 1.0);
	}

	/**
	 * vytvori bod a inicializuje souradnice
	 */
	Point3D(T x, T y, T z)  : Matrix<T>(4, 1, 0) {
		Matrix<T>::cellValue(0, 0, x);
		Matrix<T>::cellValue(1, 0, y);
		Matrix<T>::cellValue(2, 0, z);
	}

	/**
	 * vytvori bod z ukazatele na pamet
	 */
	Point3D(T *mem) : Matrix<T>(4, 1, 0) {
		Matrix<T>::newMem(mem);
	}

	/**
	 * kopytor
	 */
	Point3D(const Point3D &o) : Matrix<T>(o) {

	}

	/**
	 * na teto urovni nic nedela
	 */
	virtual ~Point3D() {

	}

	/**
	 * vraci vzdalenost bodu od pocatku
	 */
	T distance() const {
		T x, y, z;

		x = pow(this->cellValue(0, 0), 2);
		y = pow(this->cellValue(1, 0), 2);
		z = pow(this->cellValue(2, 0), 2);

		return sqrt(x + y + z);
	}

	/**
	 * vraci vzdalenost dvou bodu
	 */
	T distance(const Point3D<T> &o) {
		T dx, dy, dz, x, y, z;

		dx = this->cellValue(0, 0) - o.cellValue(0, 0);
		dy = this->cellValue(1, 0) - o.cellValue(1, 0);
		dz = this->cellValue(2, 0) - o.cellValue(2, 0);

		x = pow(dx, 2);
		y = pow(dy, 2);
		z = pow(dz, 2);

		return sqrt(x + y + z);
	}

	/**
	 * vraci souradnici X
	 */
	T x() const {
		return Matrix<T>::cellValue(0, 0);
	}

	/**
	 * nastavi souradnici X
	 */
	void x(T x) {
		Matrix<T>::cellValue(0, 0, x);
	}

	/**
	 * vraci souradnici Y
	 */
	T y() const {
		return Matrix<T>::cellValue(1, 0);
	}

	/**
	 * nastavi souradnici Y
	 */
	void y(T y) {
		Matrix<T>::cellValue(1, 0, y);
	}

	/**
	 * vraci souradnici Z
	 */
	T z() const {
		return Matrix<T>::cellValue(2, 0);
	}

	/**
	 * nastavi souradnici Z
	 */
	void z(T z) {
		Matrix<T>::cellValue(2, 0, z);
	}

	/**
	 * vraci True, pokud je instance OpenGL vektorem
	 */
	bool isVector() const {
		return Matrix<T>::cellValue(3, 0);
	}

	/**
	 * nastavi, zda je instance OpenGL vektorem
	 */
	void vector(bool isVector) {
		Matrix<T>::cellValue(3, 0, isVector ? 0 : 1);
	}

	/**
	 * pristup a konstantni pristup k souradnicicm bodu
	 */

	T &operator[] (size_t i) {
		return Matrix<T>::rawData()[i];
	}

	const T &operator[] (size_t i) const {
		return Matrix<T>::rawData()[i];
	}

	Vector3D<T> operator- (const Point3D<T> o) const {
		Vector3D<T> retVal(x() - o.x(), y() - o.y(), z() - o.z());

		return retVal;
	}
};

}
}


#endif /* VECTOR3D_HPP_ */
