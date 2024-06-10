/**************************************************************************/
/*  plane.cpp                                                             */
/**************************************************************************/


#include "plane.h"

#include "core/math/math_funcs.h"

void Plane::set_normal(const Vector3 &p_normal) {
	normal = p_normal;
}

void Plane::normalize() {
	real_t l = normal.length();
	if (l == 0) {
		*this = Plane(0, 0, 0, 0);
		return;
	}
	normal /= l;
	d /= l;
}

Plane Plane::normalized() const {
	Plane p = *this;
	p.normalize();
	return p;
}

Vector3 Plane::get_any_point() const {
	return get_normal() * d;
}

/* intersections */

bool Plane::intersect_3(const Plane &p_plane1, const Plane &p_plane2, Vector3 *r_result) const {
	const Plane &p_plane0 = *this;
	Vector3 normal0 = p_plane0.normal;
	Vector3 normal1 = p_plane1.normal;
	Vector3 normal2 = p_plane2.normal;

	real_t denom = vec3_cross(normal0, normal1).dot(normal2);

	if (Math::is_zero_approx(denom)) {
		return false;
	}

	if (r_result) {
		*r_result = ((vec3_cross(normal1, normal2) * p_plane0.d) +
							(vec3_cross(normal2, normal0) * p_plane1.d) +
							(vec3_cross(normal0, normal1) * p_plane2.d)) /
				denom;
	}

	return true;
}

bool Plane::intersects_ray(const Vector3 &p_from, const Vector3 &p_dir, Vector3 *p_intersection) const {
	Vector3 segment = p_dir;
	real_t den = normal.dot(segment);

	//printf("den is %i\n",den);
	if (Math::is_zero_approx(den)) {
		return false;
	}

	real_t dist = (normal.dot(p_from) - d) / den;
	//printf("dist is %i\n",dist);

	if (dist > (real_t)CMP_EPSILON) { //this is a ray, before the emitting pos (p_from) doesn't exist

		return false;
	}

	dist = -dist;
	*p_intersection = p_from + segment * dist;

	return true;
}

bool Plane::intersects_segment(const Vector3 &p_begin, const Vector3 &p_end, Vector3 *p_intersection) const {
	Vector3 segment = p_begin - p_end;
	real_t den = normal.dot(segment);

	//printf("den is %i\n",den);
	if (Math::is_zero_approx(den)) {
		return false;
	}

	real_t dist = (normal.dot(p_begin) - d) / den;
	//printf("dist is %i\n",dist);

	if (dist < (real_t)-CMP_EPSILON || dist > (1 + (real_t)CMP_EPSILON)) {
		return false;
	}

	dist = -dist;
	*p_intersection = p_begin + segment * dist;

	return true;
}

/* misc */

bool Plane::is_equal_approx(const Plane &p_plane) const {
	return normal.is_equal_approx(p_plane.normal) && Math::is_equal_approx(d, p_plane.d);
}

Plane::operator String() const {
	return normal.operator String() + ", " + rtos(d);
}
