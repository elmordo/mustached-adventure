/*
 * Point3D.hpp
 *
 *  Created on: 1.8.2013
 *      Author: petr
 */

#ifndef POINT3D_HPP_
#define POINT3D_HPP_

#include "Matrix.hpp"
#include "Vector3D.hpp"
#include <GL/gl.h>

namespace Gremlin {
namespace Math {

// definice typu
typedef Point3D<GLfloat> Point3Df;
typedef Point3D<GLdouble> Point3Dd;

} /* namespace Math */
} /* namespace Gremlin */
#endif /* POINT3D_HPP_ */
