/*
 * MathException.hpp
 *
 *  Created on: 27.9.2013
 *      Author: petr
 */

#ifndef MATHEXCEPTION_HPP_
#define MATHEXCEPTION_HPP_

#include <Exception.hpp>

namespace Gremlin {
namespace Math {

NEW_EXCEPTION_CLASS(MathException,Gremlin::Utils::Exception);

NEW_EXCEPTION_CLASS(MatrixException, MathException);

NEW_EXCEPTION_CLASS(EqSolverException, MathException);

}
}


#endif /* MATHEXCEPTION_HPP_ */
