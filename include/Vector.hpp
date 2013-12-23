/*
 * Vector.hpp
 *
 *  Created on: 8.8.2013
 *      Author: petr
 */

#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <cstddef>
#include <cstring>
#include <memory>
#include <cmath>

#include "Matrix.hpp"
#include <GL/gl.h>

using namespace std;

namespace Gremlin {
namespace Math {

template<class T>
class Vector: public Matrix<T> {

public:

	/**
	 * vytvori prazdny vektor
	 */
	Vector() :
			Matrix<T>() {

	}

	/**
	 * vytvori n rozmerny prazdny vektor
	 */
	Vector(size_t n) :
			Matrix<T>(n, 1, 0) {

	}

	/**
	 * kopytor
	 */
	Vector(const Vector<T> &o) :
			Matrix<T>(o) {

	}

	/**
	 * dealokuje pamet
	 */
	virtual ~Vector() {

	}

	/**
	 * vypocita absolitni velikost vektoru
	 */
	T abs() const {
		T sum = 0;

		for (size_t i = 0; i < Matrix<T>::rows(); i++) {
			sum += pow(Matrix<T>::rawData()[i], 2.0);
		}

		return sqrt(sum);
	}

	/**
	 * vraci rozmer vektoru
	 */
	inline size_t dimension() const {
		return Matrix<T>::rows() * Matrix<T>::columns();
	}

	/**
	 * provede normalizaci vektoru
	 */
	void normalise() {
		// vypocet absolutni hodnoty
		T s = abs();

		// kazsa slozka je podelena absolutni hodnotou
		for (size_t i = 0; i < Matrix<T>::rows(); i++)
			Matrix<T>::rawData()[i] /= s;
	}

	/**
	 * pristup k prvkum
	 */
	T &operator [](size_t i) {
		return Matrix<T>::rawData()[i];
	}

	const T &operator [](size_t i) const {
		return Matrix<T>::rawData()[i];
	}

	/**
	 * skalarni soucin vektoru
	 */
	T operator *(const Vector<T> &o) {
		// kontrola rozmeru vektoru
		if (Matrix<T>::rows() != o.rows())
			throw bad_exception();

		// inicializace navratove hodnoty
		T retVal = 0;
		const T *data1 = Matrix<T>::rawData();
		const T *data2 = o.rawData();

		// iterace nad prvky a jejich nasobeni
		for (size_t i = 0; i != Matrix<T>::rows(); i++)
			retVal += data1[i] * data2[i];

		return retVal;
	}
};

typedef Vector<GLdouble> GLVector;

} /* namespace Math */
} /* namespace Gremlin */
#endif /* VECTOR_HPP_ */
