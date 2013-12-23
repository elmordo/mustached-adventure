/*
 * Matrix.hpp
 *
 *  Created on: 31.7.2013
 *      Author: petr
 */

#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <memory>
#include <cstdlib>
#include <cstring>
#include "MatrixSegment.hpp"

namespace Gremlin {
namespace Math {

using namespace std;

template<class T>
class Matrix {

	/**
	 * pocet radku
	 */
	size_t m;

	/**
	 * pocet sloupcu
	 */
	size_t n;

	/**
	 * hodnoty prvku matice
	 */
	T *vals;

protected:

	/**
	 * vynasobi matice a vraci ukazatel na novou pamet
	 */
	static T *productMem(const Matrix &m1, const Matrix &m2) {
		// alokace pameti
		allocator<T> alloc;
		T *retVal;
		size_t m = m1.m, n = m2.n, o = m1.n;

		retVal = alloc.allocate(m * n);

		// iterace nad radky a sloupci a vypocet novych prvku
		for (size_t i = 0; i < m; i++) {
			// prochazime i-ty radek
			for (size_t j = 0; j < n; j++) {
				// jsme na i-tem radku a v j-tem sloupci
				T cellVal = 0;
				for (size_t k = 0; k < o; k++) {
					cellVal += m1.cellValue(i, k) * m2.cellValue(k, j);
				}

				retVal[i * n + j] = cellVal;
			}
		}

		return retVal;
	}

	/**
	 * nastavi novou pameti
	 */
	void newMem(T *newMem) {
		if (vals) {
			allocator<T> alloc;
			alloc.deallocate(vals, m*n);
		}

		vals = newMem;
	}

public:
	Matrix() {
		m = 0;
		n = 0;
		vals = 0x0;
	}

	/**
	 * vytvori matici a inicializuje ji vychozi hodnotou
	 */
	Matrix(size_t m, size_t n, T defVal) {
		// zapis rozmeru
		this->m = m;
		this->n = n;

		// alokace pameti
		size_t len = m * n;

		if (len == 0) {
			vals = 0x0;
			return;
		}

		allocator<T> loc;
		vals = loc.allocate(len);

		// nastaveni vychozi hodnoty
		for (size_t i = 0; i < len; i++) {
			vals[i] = defVal;
		}
	}

	/**
	 * kopirovaci konstruktor
	 */
	Matrix(const Matrix &o) {
		m = o.m;
		n = o.n;

		// alokace pameti
		allocator<T> alloc;
		vals = alloc.allocate(m*n);

		// prekopirovani pameti
		memcpy(vals, o.vals, m*n * sizeof(T));
	}

	/**
	 * pokud jsou alokovany prvky, destruktor je dealokuje
	 */
	virtual ~Matrix() {
		// odstraneni hodnot, pokud je treba
		if (vals) {
			allocator<T> loc;
			loc.deallocate(vals, m*n);
		}
	}

	/**
	 * vraci surova data
	 */
	const T *rawData() const {
		return vals;
	}

	T *rawData() {
		return vals;
	}

	/**
	 * nastavi surova data
	 */
	void rawData(T *data) {
		vals = data;
	}

	/**
	 * provede nasobeni matic
	 */
	Matrix<T> product(const Matrix<T> &b) const {
		// priprava navratove hodnoty
		Matrix<T> retVal;

		// kontrola velikosti matic
		if (b.m != n) return Matrix();

		// vytvoreni nove matice
		retVal.m = m;
		retVal.n = b.n;

		// iterace nad radky a sloupci a vypocet novych prvku
		retVal.vals = productMem(*this, b);

		return retVal;
	}

	/**
	 * nasobeni skalarem
	 */
	Matrix<T> product(T k) const {
		Matrix<T> retVal(m, n, 0);

		// nasobeni prvku
		size_t len = m*n;

		for (size_t i = 0; i < len; i++) {
			retVal.vals[i] = vals[i] * k;
		}

		return retVal;
	}

	/**
	 * vraci hodnotu na souradnicich
	 */
	inline T cellValue(size_t r, size_t c) const {
		return vals[n * r + c];
	}

	/**
	 * nastavi hodnotu na souradnicich
	 */
	inline void cellValue(size_t r, size_t c, T v) {
		vals[n * r + c] = v;
	}

	/**
	 * operator nasobeni matic
	 */
	Matrix<T> operator*(const Matrix<T> &o) const {
		return product(o);
	}

	/**
	 * vraci pocet radku
	 */
	inline size_t rows() const {
		return m;
	}

	/**
	 * vraci pocet sloupcu
	 */
	inline size_t columns() const {
		return n;
	}

	/**
	 * operator nasobeni skalarem
	 */
	Matrix<T> operator*(T k) const {
		return product(k);
	}

	/**
	 * operator prirazeni
	 */
	Matrix<T> &operator =(const Matrix<T> &o) {
		// vypocet velikosti nove pameti
		size_t length = o.m * o.n;

		// alokace nove pameti
		allocator<T> alloc;
		T* mem = alloc.allocate(length);

		// prekopirovani hodnot a nastaveni nove pameti
		memcpy(mem, o.vals, length * sizeof(T));
		newMem(mem);

		// update rozmeru
		m = o.m;
		n = o.n;

		return *this;
	}

	/**
	 * operator porovnani
	 */
	bool operator==(const Matrix<T> &o) const {
		// porovnani rozmeru
		if (m != o.m || n != o.n) return false;

		// porovnani prvku
		size_t len = m*n;

		for (size_t i = 0; i < len; i++) {
			if (vals[i] != o.vals[i]) return false;
		}

		return true;
	}

	/**
	 * vraci vybrany radek
	 */
	MatrixSegment<T> operator[] (size_t i) {
		return MatrixSegment<T>(vals, n * i, 1, n);
	}

	MatrixSegment<const T> operator[] (size_t i) const {
		return MatrixSegment<const T>(vals, n * i, 1, n);
	}
};

} // end Math
} // end

#endif /* MATRIX_HPP_ */
