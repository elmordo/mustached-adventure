/*
 * GEM.hpp
 *
 *  Created on: 12.10.2013
 *      Author: petr
 */

#ifndef GEM_HPP_
#define GEM_HPP_

#include "EqSolver.hpp"

namespace Gremlin {
namespace Math {

template<class T>
class GEM: public EqSolver<T> {

	/**
	 * na radku i a ve sloupci i bude jednicka a pod ni same nuly
	 */
	void solveX(Matrix<T> &A, size_t i) const {
		// prevedeni prvku na i-tem radku a v i-tem sloupci na jednicku
		MatrixSegment<T> s = A[i];

		// nejprve kontrola, jestli je prvek [i,i] nulovy
		if (s[i] == 0) {
			// prvek je nulovy, musime radek zamenit za takovy radek, kde neni prvni prvek nulovy
			for (size_t j = i + 1; j < A.rows() && s[i] == 0; j++) {
				MatrixSegment<T> o = A[j];

				if (o[i] != 0) {
					// byl nalezen radek, kde se i-ty sloupec nerovna nule - dojde k prohozeni radku
					s.swap(o);
				}
			}

			// pokud se stale rovna i-ty prvek nule, pak dojde k vyhozeni vyjimky
			if (s[i] == 0) THROW_EXCEPTION(EqSolverException, "Matrix has linear rows", 1, 0x0);
		}

		s /= s[i];

		// iterace nad dalsimi radky a anulace i-teho sloupce v nich
		for (size_t j = i + 1; j < A.rows(); j++) {
			// nacteni radku
			MatrixSegment<T> r = A[j];

			// kontrola nulovosti - pokud je index nulovy, pak se nic delat nemusi
			if (r[i] == 0) continue;

			// prevedeni sloupce na jednicku
			r /= r[i];

			// anulace
			r -= s;
		}
	}

public:

	GEM() {

	}

	virtual ~GEM() {

	}

	virtual Matrix<T> solve(Matrix<T> A) const {
		// vytvoreni matice navratovych hodnot
		Matrix<T> retVal(A.rows(), 1, 0);

		size_t bIndex = A.columns() - 1;

		// provedeni modifikaci matice A
		for (size_t i = 0; i < A.rows(); i++) {
			solveX(A, i);
		}

		// vypocet vysledku
		size_t i = A.rows();

		do {
			// dekrementace indexu
			i--;

			// priprava hodnoty promenne
			MatrixSegment<T> seg = A[i];
			T xVal = -seg[bIndex];

			// pricteni dalsich hodnot
			for (size_t j = i + 1; j < bIndex; j++) {
				xVal -= seg[j] * retVal.cellValue(i, j);
			}

			// zapis vypocitane hodnoty
			retVal.cellValue(i, 0, xVal);
		} while (i != 0);

		return retVal;
	}
};

}
}

#endif /* GEM_HPP_ */
