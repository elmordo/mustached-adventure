/*
 * VolumeObject.hpp
 *
 *  Created on: 3.10.2013
 *      Author: petr
 */

#ifndef VOLUMEOBJECT_HPP_
#define VOLUMEOBJECT_HPP_

#include <cstddef>
#include <memory>
#include <cstring>

#include "Transform3D.hpp"
#include "Point3D.hpp"

namespace Gremlin {
namespace Math {

using namespace std;

template<class T>
class VolumeObject {

	/**
	 * transformacni matice
	 */
	const Transform3D<T> *m;

	/**
	 * puvodni definicni body
	 */
	Point3D<T> *p;

	/**
	 * transformovane definicni body
	 */
	Point3D<T> *pt;

	/**
	 * pocet definicnich bodu
	 */
	size_t pc;

	void deallocate() {
		if (p) {
			allocator<Point3D<T> > alloc;

			p = alloc.allocate(pc);
			pt = alloc.allocate(pc);

			pc = 0;
		}
	}

	void allocate(size_t pc) {
		// dealokace puvodni pameti
		deallocate();

		// alokace pameti
		allocator<Point3D<T> > alloc;

		p = alloc.allocate(pc);
		pt = alloc.allocate(pc);

		Point3D<T> proto;

		for (size_t i = 0; i < pc; i++) {
			alloc.construct(p + i, proto);
			alloc.construct(pt + i, proto);
		}
	}

protected:

	VolumeObject(size_t pc) {
		allocate(pc);

		m = 0x0;
		this->pc = pc;
	}

	Point3D<T> &point(size_t i) {
		return p[i];
	}

	Point3D<T> &tPoint(size_t i) {
		return pt[i];
	}

public:

	VolumeObject() {
		m = 0x0;
		p = 0x0;
		pt = 0x0;
		pc = 0;
	}

	VolumeObject(const VolumeObject<T> &o) {
		*this = o;

	}

	virtual ~VolumeObject() {
		deallocate();
	}

	/**
	 * zkonstroluje prunik objektu
	 */
	virtual bool checkIntersect(const VolumeObject<T> &o) const = 0;

	/**
	 * metody slouzici ke kontrole bodu pro posun souradnic
	 */
	virtual void checkPoint(const Point3D<T> &p) = 0;
	virtual void checkPoint(T x, T y, T z) = 0;

	/**
	 * vraci transformacni matici
	 */
	const Transform3D<T> *matrix() const {
		return m;
	}

	/**
	 * nastavi novou transformacni matici
	 */
	void matrix(const Transform3D<T> *matrix) {
		m = matrix;
	}

	/**
	 * vraci vyzadany bod dle indexu
	 */
	const Point3D<T> &point(size_t i) const {
		return p[i];
	}

	/**
	 * resetuje definicni body
	 */
	virtual void reset() {
		// vytvoreni nuloveho bodu
		Point3D<T> zero;

		for (size_t i = 0; i < pc; i++) {
			// priradime nulovy bod i temu definicnimu bodu
			p[i] = zero;
		}
	}

	const Point3D<T> &tPoint(size_t i) const {
		return pt[i];
	}

	/**
	 * transformuje definicni body
	 */
	virtual void transform() {
		// iterujse se nad vsemi body a kazdy bod se vynasovi matici
		for (size_t i = 0; i < pc; i++) {
			pt[i] = (*m) * p[i];
		}
	}

	VolumeObject<T> &operator= (const VolumeObject &o) {
		deallocate();

		// zapis standardnich dat
		m = o.m;
		pc = o.pc;

		// prekopirovani pameti
		allocate(pc);

		for (size_t i = 0; i < pc; i++) {
			p[i] = o.p[i];
			pt[i] = o.pt[i];
		}

		return *this;
	}
};

}
}

#endif /* VOLUMEOBJECT_HPP_ */
