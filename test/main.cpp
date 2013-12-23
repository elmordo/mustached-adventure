/*
 * main.cpp
 *
 *  Created on: 31.7.2013
 *      Author: petr
 */
#include "Matrix.hpp"
#include "Transform3D.hpp"
#include "Plane.hpp"
#include "Box.hpp"
#include "GEM.hpp"

#include <iostream>

using namespace Gremlin::Math;
using namespace std;

void printBox(const Box<double_t> &b) {
	for (int i = 0; i < Box<double_t>::POINTS_COUNT; i++) {
		Point3D<double_t> p = b.point(i);
		Point3D<double_t> pt = b.tPoint(i);

		cout << p.x() << " " << p.y() << " " << p.z() << " --- ";
		cout << pt.x() << " " << pt.y() << " " << pt.z() << endl;
	}

	cout << endl;

	for (int i = 0; i < Box<double_t>::PLANE_COUNT; i++) {
		Plane<double_t> p = b.plane(i);

		cout << p.coef(0) << " " << p.coef(1) << " " << p.coef(2) << " " << p.coef(3) << endl;
	}

	cout << endl;
}

void printMatrix(const Matrix<GLfloat> M) {
	for (size_t i = 0; i < M.rows(); i++) {
		for (size_t j = 0; j < M.columns(); j++) {
			cout << M[i][j] << " ";
		}

		cout << endl;
	}
}

int main(int argc, char** argv) {
	Matrix<int> m1(2, 2, 1), m2(2, 2, 1);

	m1.cellValue(0, 0, 1);
	m1.cellValue(0, 1, 2);
	m1.cellValue(1, 0, 3);
	m1.cellValue(1, 1, 4);

	m2.cellValue(0, 0, 5);
	m2.cellValue(0, 1, 6);
	m2.cellValue(1, 0, 7);
	m2.cellValue(1, 1, 8);

	Matrix<int> retVal = m1.product(m2);

	Transform3Dd t;
	t.scale(2, 2, 2);

	std::cout << retVal.cellValue(0, 0) << " " << retVal.cellValue(0, 1)
			<< endl;
	cout << retVal.cellValue(1, 0) << " " << retVal.cellValue(1, 1) << endl
			<< endl;

	// zkouska hranic
	Box<double_t> b1, b2;

	b1.matrix(&t);
	b2.matrix(&t);

	b1.checkPoint( 1,  1,  1);
	b1.checkPoint(-1, -1, -1);

	b2.checkPoint(-2, -2, -2);
	b2.checkPoint(2, 2, 2);

	b1.transform();
	b2.transform();

	printBox(b1);
	printBox(b2);

	if (b1.checkIntersect(b2)) {
		cout << "protinaji se" << endl;
	} else {
		cout << "neprotinaji se" << endl;
	}

	// zkouska GEM
	GEM<GLfloat> solver;

	// zkusebni matice
	Matrix<GLfloat> A(2, 3, 1);
	A[0][0] = 1;
	A[0][1] = 15;
	A[0][2] = -53;

	A[1][0] = 3;
	A[1][1] = 1;
	A[1][2] = -27;

	// predpokladany vysledek je x1=8 a x2=3
	Matrix<GLfloat> r = solver.solve(A);
	printMatrix(r);

	return 0;
}
