/*
 * EqSolver.hpp
 *
 *  Created on: 12.10.2013
 *      Author: petr
 */

#ifndef EQSOLVER_HPP_
#define EQSOLVER_HPP_

#include "Matrix.hpp"
#include "MathException.hpp"

namespace Gremlin {
namespace Math {

template<class T>
class EqSolver {

public:

	EqSolver() {

	}

	virtual ~EqSolver() {

	}

	virtual Matrix<T> solve(Matrix<T> A) const = 0;
};

}
}

#endif /* EQSOLVER_HPP_ */
