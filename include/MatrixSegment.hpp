/*
 * MatrixSegment.hpp
 *
 *  Created on: 12.8.2013
 *      Author: petr
 */

#ifndef MATRIXSEGMENT_HPP_
#define MATRIXSEGMENT_HPP_

#include <cstdlib>

using namespace std;

namespace Gremlin {
namespace Math {

template<class T>
class MatrixSegment {

	/**
	 * ukazatel na hodnoty
	 */
	T* vals;

	/**
	 * offset od nulte hodnoty
	 */
	size_t offset;

	/**
	 * inkrement na jednotku indexu
	 */
	size_t increment;

	/**
	 * pocet prvku v segmentu
	 */
	size_t itemsCount;

public:

	/**
	 * vytvori neiinicializovanou matici
	 */
	MatrixSegment() {
		vals = 0x0;
		itemsCount = offset = increment = 0;
	}

	/**
	 * vytvori inicializovanou instanci
	 * nastavi ukazatel na data a priradi hodnotu offsetu a inkrementu
	 */
	MatrixSegment(T* vals, size_t offset, size_t increment, size_t itemsCount) {
		this->vals = vals;
		this->increment = increment;
		this->offset = offset;
		this->itemsCount = itemsCount;
	}

	/**
	 * kopytor
	 */
	MatrixSegment(const MatrixSegment<T> &o) {
		vals = o.vals;
		increment = o.increment;
		offset = o.offset;
		itemsCount = o.itemsCount;
	}

	virtual ~MatrixSegment() {

	}

	/**
	 * vraci pocet prvku v segmentu
	 */
	inline size_t count() const {
		return itemsCount;
	}

	/**
	 * prohodi hodnoty ve dvou segmentech
	 */
	void swap(MatrixSegment<T> &s) {
		size_t index = 0;
		T buffer;

		for (size_t i = 0; i < itemsCount; i++) {
			// vypocitani indexu
			index = i * increment;

			// zapis aktualni hodnoty do pomocne promenne
			buffer = vals[index];

			// prohozeni hodnota
			vals[index] = s.vals[index];
			s.vals[index] = buffer;
		}
	}

	/**
	 * vraci referenci na prvek segmentu
	 */
	T& operator[](size_t i) {
		return vals[offset + i * increment];
	}

	/**
	 * vraci konstantni referenci na prvek segmentu
	 */
	const T& operator[](size_t i) const {
		return vals[offset + i * increment];
	}

	/**
	 * pricte konstantu k segmentu
	 */
	MatrixSegment<T> &operator +=(T k) {
		size_t index = offset;

		for (size_t i = 0; i < itemsCount; i++) {
			vals[index] += k;
			index += increment;
		}

		return *this;
	}

	/**
	 * odecte od segmentu konstantu
	 */
	MatrixSegment<T> &operator -=(T k) {
		size_t index = offset;

		for (size_t i = 0; i < itemsCount; i++) {
			vals[index] -= k;
			index += increment;
		}

		return *this;
	}

	/**
	 * pricte k segmentu jiny segment
	 */
	MatrixSegment<T> &operator +=(const MatrixSegment<T> &o) {
		size_t indexThis = offset;
		size_t indexOther = o.offset;

		for (size_t i = 0; i < itemsCount; i++) {
			vals[indexThis] += o.vals[indexOther];
			indexThis += increment;
			indexOther += o.increment;
		}

		return *this;
	}

	/**
	 * odecte od segmentu jiny segment
	 */
	MatrixSegment<T> &operator -=(const MatrixSegment<T> &o) {
		size_t indexThis = offset;
		size_t indexOther = o.offset;

		for (size_t i = 0; i < itemsCount; i++) {
			vals[indexThis] -= o.vals[indexOther];
			indexThis += increment;
			indexOther += o.increment;
		}

		return *this;
	}

	/**
	 * vynasobi prvky segmentu konstantou
	 */
	MatrixSegment<T> &operator *=(T k) {
		size_t index = offset;

		for (size_t i = 0; i < itemsCount; i++) {
			vals[index] *= k;
			index += increment;
		}

		return *this;
	}

	/**
	 * podeli prvky segmentu konstantou
	 */
	MatrixSegment<T> &operator /=(T k) {
		size_t index = offset;

		for (size_t i = 0; i < itemsCount; i++) {
			vals[index] /= k;
			index += increment;
		}

		return *this;
	}

	/**
	 * operator prirazeni
	 */
	MatrixSegment<T> &operator =(const MatrixSegment<T> &o) {
		vals = o.vals;
		increment = o.increment;
		offset = o.offset;
		itemsCount = o.itemsCount;

		return *this;
	}
};

}
}

#endif /* MATRIXSEGMENT_HPP_ */
