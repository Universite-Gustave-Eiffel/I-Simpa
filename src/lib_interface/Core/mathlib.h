/* ----------------------------------------------------------------------
* I-SIMPA (http://i-simpa.ifsttar.fr). This file is part of I-SIMPA.
*
* I-SIMPA is a GUI for 3D numerical sound propagation modelling dedicated
* to scientific acoustic simulations.
* Copyright (C) 2007-2014 - IFSTTAR - Judicael Picaut, Nicolas Fortin
*
* I-SIMPA is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
* 
* I-SIMPA is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software Foundation,
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA or 
* see <http://ww.gnu.org/licenses/>
*
* For more information, please consult: <http://i-simpa.ifsttar.fr> or 
* send an email to i-simpa@ifsttar.fr
*
* To contact Ifsttar, write to Ifsttar, 14-20 Boulevard Newton
* Cite Descartes, Champs sur Marne F-77447 Marne la Vallee Cedex 2 FRANCE
* or write to scientific.computing@ifsttar.fr
* ----------------------------------------------------------------------*/

#include "../std_tools.hpp"
#include <math.h>
#include <stdlib.h>
#include <float.h>


/*! \file Mathlib.h
    \brief Ce fichier contient la librairie mathématique de l'interface

    Le type vecteur est déclaré ici, ainsi que les opérations sur ces vecteurs.
*/
#ifndef __HMATHLIB__
#define __HMATHLIB__

// Number precision for display (after comma separator)
const int COMMA_PRECISION_DB = 1;
const int COMMA_PRECISION_TIME_S = 2;
const int COMMA_PRECISION_TIME_MS = 1;
const int COMMA_PRECISION_PERCENT = 1;
const int COMMA_PRECISION_AREA = 2;


/**
 * @brief Fonctions mathématiques
 */
namespace core_mathlib
{
typedef float decimal;
#ifndef EPSILON
	#define EPSILON				(decimal)0.000001						/*!< Approximation lors de la comparaison de 2 decimal */
#endif
#define BARELY_EPSILON		(decimal)0.0001									/*!< Approximation large lors de la comparaison de 2 decimal */
#ifndef M_PI
#define M_PI				(decimal)3.141592653589793238462643383279		/*!< PI */
#endif
#define M_PIDIV2			(decimal)1.570796326794896619231321691639		/*!< PI / 2 */
#define M_2PI				(decimal)6.283185307179586476925286766559		/*!< 2 * PI */
#define M_PI2				(decimal)9.869604401089358618834490999876		/*!< PI au carre */
#define M_PIDIV180			(decimal)0.01745329251994329576923690768488		/*!< PI / 180 */
#define M_180DIVPI			(decimal)57.295779513082320876798154814105		/*!< 180 / PI */

#define DegToRad(a)	a*=M_PIDIV180
#define RadToDeg(a)	a*=M_180DIVPI
#define RADIANS(a)	a*M_PIDIV180
#define DEGRES(a)	a*M_180DIVPI

#ifndef SIGN
	#define SIGN(x) ((x) > 0 ? 1 : -1)
#endif

class vec2;
class vec4;
class ivec2;
class ivec3;
class ivec4;
/*****************************************************************************/
/*                                                                           */
/* vec3                                                                      */
/*                                                                           */
/*****************************************************************************/

/**
 * @brief Vecteur 3D
 * Vecteur comprenant 3 flottants
 */
template<typename base_t>
class base_vec3 {
public:
	base_vec3(void) : x(0), y(0), z(0) { }
	base_vec3(const base_t& _x,const base_t& _y,const base_t& _z) : x(_x), y(_y), z(_z) { }
	base_vec3(const double _v[3]) : x(_v[0]), y(_v[1]), z(_v[2]) { }
	base_vec3(const float _v[3]) : x(_v[0]), y(_v[1]), z(_v[2]) { }
	base_vec3(const vec2 &_v,base_t _z);
	base_vec3(const base_vec3 &_v) : x(_v.x), y(_v.y), z(_v.z) { }
	base_vec3(const vec4 &_v);

	int operator==(const base_vec3 &_v) {
		return (fabs(this->x - _v.x) < EPSILON && fabs(this->y - _v.y) < EPSILON && fabs(this->z - _v.z) < EPSILON);
	}
	int operator!=(const base_vec3 &_v) { return !(*this == _v); }

	base_vec3 &operator=(base_t _f) { this->x=_f; this->y=_f; this->z=_f; return (*this); }
	const base_vec3 operator*(base_t _f) const { return base_vec3(this->x * _f,this->y * _f,this->z * _f); }
	const base_vec3 operator/(base_t _f) const {
		if(fabs(_f) < EPSILON) return *this;
		_f = 1.0f / _f;
		return (*this) * _f;
	}
	base_vec3 operator/(const base_vec3 &_v) const {
		return base_vec3(x/_v.x,y/_v.y,z/_v.z);
	}
	const base_vec3 operator+(const base_vec3 &_v) const { return base_vec3(this->x + _v.x,this->y + _v.y,this->z + _v.z); }
	const base_vec3 operator-() const { return base_vec3(-this->x,-this->y,-this->z); }
	const base_vec3 operator-(const base_vec3 &_v) const { return base_vec3(this->x - _v.x,this->y - _v.y,this->z - _v.z); }

	base_vec3 &operator*=(base_t _f) { return *this = *this * _f; }
	base_vec3 &operator/=(base_t _f) { return *this = *this / _f; }
	base_vec3 &operator+=(const base_vec3 &_v) { return *this = *this + _v; }
	base_vec3 &operator-=(const base_vec3 &_v) { return *this = *this - _v; }

	base_t operator*(const base_vec3 &_v) const { return this->x * _v.x + this->y * _v.y + this->z * _v.z; }
	base_t operator*(const vec4 &_v) const;

	/**
	 * Copy this vector into the float array in parameter
	 * @param arr[out] External array
	 */
	void copyTo(float* arr) {
		arr[0] = x;
		arr[1] = y;
		arr[2] = z;
	}

	operator base_t*() { return this->v; }
	operator const base_t*() const { return this->v; }
	base_t &operator[](int _i) { return this->v[_i]; }
	const base_t &operator[](int _i) const { return this->v[_i]; }

	bool barelyEqual(const base_vec3 &_v) const  { return (fabs(this->x - _v.x) < BARELY_EPSILON && fabs(this->y - _v.y) < BARELY_EPSILON && fabs(this->z - _v.z) < BARELY_EPSILON); }
	void set(base_t _x,base_t _y,base_t _z) { this->x = _x; this->y = _y; this->z = _z; }
	void reset(void) { this->x = this->y = this->z = 0; }
	base_t length(void) const { return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z); }
	base_t normalize(void) {
		base_t inv,l = this->length();
		if(l < EPSILON) return 0.0f;
		inv = 1.0f / l;
		this->x *= inv;
		this->y *= inv;
		this->z *= inv;
		return l;
	}
	void cross(const base_vec3 &v1,const base_vec3 &v2) {
		this->x = v1.y * v2.z - v1.z * v2.y;
		this->y = v1.z * v2.x - v1.x * v2.z;
		this->z = v1.x * v2.y - v1.y * v2.x;
	}
	void cross(const base_vec3 &v2) {
		base_t x = this->y * v2.z - this->z * v2.y;
		base_t y = this->z * v2.x - this->x * v2.z;
		this->z = this->x * v2.y - this->y * v2.x;
		this->y = y;
		this->x = x;
	}
	base_t cosinus(const base_vec3 &ac){
	    return (this->x*ac.x + this->y*ac.y + this->z*ac.z)/(length() * ac.length());
    }
	base_t dot(const base_vec3 &v) const { return ((this->x*v.x) + (this->y*v.y) + (this->z*v.z)); } // Produit scalaire
	// this < _v
	bool compare(const base_vec3 &_v,base_t epsi=EPSILON) { return (fabs(this->x - _v.x) < epsi && fabs(this->y - _v.y) < epsi && fabs(this->z - _v.z) < epsi); }
	base_t angle(const base_vec3 &v)  const { // retourne l'angle en radians entre *this et v
		base_t angle = acos(this->dot(v)/(this->length()*v.length()));
		if(angle < EPSILON) return 0;
		return angle;
	}
	/** @brief  retourne les coordonnée du point le plus proche de *this sur la droite passant par vA et vB */
	base_vec3 closestPointOnLine(const base_vec3 &vA, const base_vec3 &vB) { return (((vB-vA) * this->projectionOnLine(vA, vB)) + vA); }
	/** @brief  retourne les coordonnées du point le plus proche de *this sur le segment vA,vB */
	base_vec3 closestPointOnSegment(const base_vec3 &vA, const base_vec3 &vB) {
		base_t factor = this->projectionOnLine(vA, vB);
		if (factor <= 0.0f) return vA;
		if (factor >= 1.0f) return vB;
		return (((vB-vA) * factor) + vA);
	}
	/** @brief  retourne le facteur de la projection de *this sur la droite passant par vA et vB */
	base_t projectionOnLine(const base_vec3 &vA, const base_vec3 &vB) {
		base_vec3 v(vB - vA);
		return v.dot(*this - vA) / v.dot(v);
	}
	/** @brief  Fonction d'interpolation linéaire entre 2 vecteurs */
	base_vec3 lerp(base_vec3 &u, base_vec3 &v, base_t factor) { return ((u * (1 - factor)) + (v * factor)); }

	/** @brief Calcul la distance entre deux points
	 * @return Résultat du calcul
	 */
    double distance(const base_vec3& a_vector) const
    {
      // compute distance between both points
      double dx = x - a_vector.x;
      double dy = y - a_vector.y;
      double dz = z - a_vector.z;

      // return result
      return(sqrt( dx * dx + dy * dy + dz * dz ));
    }
	/** @brief Rotation d'un vecteur selon deux angles
	 *
	 * Fonction pour le calcul du changement de coordonnées du vecteur
	 * this par rotation suivant le vecteur n
	 * d'un angle (radians)
	 *
	 * @param[in] angle Angle en radians
	 * @param[in] n Vecteur de rotation
	 */
	base_vec3 Rotation(const base_vec3 &n,const base_t &angle) const
	{
		base_t m1=cos(angle);
		base_t m2=1-m1;
		base_t m3=sin(angle);

		base_t m2nx=m2*n.x;

		return base_vec3( (m1 +   m2nx*n.x)*this->x + (m2nx*n.y -  m3*n.z)*this->y + (m2nx*n.z +  m3*n.y)*this->z,
			(m2nx*n.y +  m3*n.z)*this->x + (m1 +   m2*n.y*n.y)*this->y + (m2*n.y*n.z -  m3*n.x)*this->z,
			(m2nx*n.z -  m3*n.y)*this->x +(m2*n.y*n.z +  m3*n.x)*this->y + (m1 +   m2*n.z*n.z)*this->z);
	}
	union {
		struct {base_t x,y,z;};
		struct {base_t s,t,p;};
		struct {base_t r,g,b;};
		base_t v[3];
	};
};
typedef base_vec3<decimal> vec3;
typedef base_vec3<double> dvec3;


inline vec3 dvec3_to_vec3(const dvec3 &v1) {
	return vec3(v1.x, v1.y, v1.z);
}

inline dvec3 vec3_to_dvec3(const vec3 &v1) {
	return dvec3(v1.x, v1.y, v1.z);
}

/*****************************************************************************/
/*                                                                           */
/* vec2                                                                      */
/*                                                                           */
/*****************************************************************************/
/**
 * @brief Vecteur 2D
 * Vecteur comprenant 2 flottants
 */
class vec2 {
public:
	vec2(void) : x(0), y(0) { }
	vec2(decimal _x,decimal _y) : x(_x), y(_y) { }
	vec2(const decimal *_v) : x(_v[0]), y(_v[1]) { }
	vec2(const vec2 &_v) : x(_v.x), y(_v.y) { }
	vec2(const vec3 &_v);
	vec2(const vec4 &_v);

	int operator==(const vec2 &_v) { return (fabs(this->x - _v.x) < EPSILON && fabs(this->y - _v.y) < EPSILON); }

	int operator!=(const vec2 &_v) { return !(*this == _v); }

	vec2 &operator=(decimal _f) { this->x=_f; this->y=_f; return (*this); }
	const vec2 operator*(decimal _f) const { return vec2(this->x * _f,this->y * _f); }
	const vec2 operator/(decimal _f) const {
		if(fabs(_f) < EPSILON) return *this;
		_f = 1.0f / _f;
		return (*this) * _f;
	}
	const vec2 operator+(const vec2 &_v) const { return vec2(this->x + _v.x,this->y + _v.y); }
	const vec2 operator-() const { return vec2(-this->x,-this->y); }
	const vec2 operator-(const vec2 &_v) const { return vec2(this->x - _v.x,this->y - _v.y); }

	vec2 &operator*=(decimal _f) { return *this = *this * _f; }
	vec2 &operator/=(decimal _f) { return *this = *this / _f; }
	vec2 &operator+=(const vec2 &_v) { return *this = *this + _v; }
	vec2 &operator-=(const vec2 &_v) { return *this = *this - _v; }

	decimal operator*(const vec2 &_v) const { return this->x * _v.x + this->y * _v.y; }

	operator decimal*() { return this->v; }
	operator const decimal*() const { return this->v; }
	decimal &operator[](int _i) { return this->v[_i]; }
	const decimal &operator[](int _i) const { return this->v[_i]; }


	void set(decimal _x,decimal _y) { this->x = _x; this->y = _y; }
	void reset(void) { this->x = this->y = 0; }
	decimal length(void) const { return sqrtf(this->x * this->x + this->y * this->y); }
	decimal normalize(void) {
		decimal inv,l = this->length();
		if(l < EPSILON) return 0.0f;
		inv = 1.0f / l;
		this->x *= inv;
		this->y *= inv;
		return l;
	}
	decimal dot(const vec2 &v) { return ((this->x*v.x) + (this->y*v.y)); } // Produit scalaire
	bool compare(const vec2 &_v,decimal epsi=EPSILON) { return (fabs(this->x - _v.x) < epsi && fabs(this->y - _v.y) < epsi); }
	/** @brief retourne les coordonnée du point le plus proche de *this sur la droite passant par vA et vB */
	vec2 closestPointOnLine(const vec2 &vA, const vec2 &vB) { return (((vB-vA) * this->projectionOnLine(vA, vB)) + vA); }
	/** @brief retourne les coordonnée du point le plus proche de *this sur le segment vA,vB */
	vec2 closestPointOnSegment(const vec2 &vA, const vec2 &vB) {
		decimal factor = this->projectionOnLine(vA, vB);
		if (factor <= 0.0f) return vA;
		if (factor >= 1.0f) return vB;
		return (((vB-vA) * factor) + vA);
	}
	/** @brief retourne le facteur de la projection de *this sur la droite passant par vA et vB */
	decimal projectionOnLine(const vec2 &vA, const vec2 &vB) {
		vec2 v(vB - vA);
		return v.dot(*this - vA) / v.dot(v);
	}
	/** @brief  Fonction d'interpolation linéaire entre 2 vecteurs */
	vec2 lerp(vec2 &u, vec2 &v, decimal factor) { return ((u * (1 - factor)) + (v * factor)); }
	decimal angle(void) { return (decimal)atan2(this->y,this->x); }
	decimal angle(const vec2 &v) { return (decimal)atan2(v.y-this->y,v.x-this->x); }

	union {
		struct {decimal x,y;};
		struct {decimal s,t;};
		decimal v[2];
	};
};


inline void Cross(const vec3 &v1,const vec3 &v2,vec3 &vout) {
	vout.x=v1.y * v2.z - v1.z * v2.y;
	vout.y=v1.z * v2.x - v1.x * v2.z;
	vout.z=v1.x * v2.y - v1.y * v2.x;
}

template<typename base_t>
inline base_vec3<base_t> Cross_r(const base_vec3<base_t> &v1,const base_vec3<base_t> &v2) {
	return base_vec3<base_t>(v1.y * v2.z - v1.z * v2.y,v1.z * v2.x - v1.x * v2.z,v1.x * v2.y - v1.y * v2.x);
}
template<typename base_t>
inline base_vec3<base_t>::base_vec3(const vec2 &_v,base_t _z)
 : x(_v.x), y(_v.y), z(_z)
  {

  }
inline vec2::vec2(const vec3 &_v) {
	this->x = _v.x;
	this->y = _v.y;
}

// This calculates a vector between 2 points and returns the result
template<typename base_t>
inline void Vector(const  base_vec3<base_t> &vp1, const  base_vec3<base_t> &vp2, base_vec3<base_t> &vout) {

	vout.x=vp1.x - vp2.x;
	vout.y=vp1.y - vp2.y;
	vout.z=vp1.z - vp2.z;
}

template<typename base_t>
inline base_vec3<base_t> Vector_r(const base_vec3<base_t> &vp1, const base_vec3<base_t> &vp2)
{
	return base_vec3<base_t>(vp1.x - vp2.x,vp1.y - vp2.y,vp1.z - vp2.z);
}
/**
 * This calculates determinant 3*3
 */
inline decimal Determinant(const vec3 &vp1, const vec3 &vp2, const vec3 &vp3) {
	return vp1.x*vp2.y*vp3.z
		  +vp1.y*vp2.z*vp3.x
		  +vp1.z*vp2.x*vp3.y
		  -vp1.x*vp2.z*vp3.y
		  -vp1.y*vp2.x*vp3.z
		  -vp1.z*vp2.y*vp3.x;
}

/*****************************************************************************/
/*                                                                           */
/* vec4                                                                      */
/*                                                                           */
/*****************************************************************************/

template<typename base_t>
inline base_vec3<base_t> FaceNormal(const base_vec3<base_t> &vp1, const base_vec3<base_t> &vp2, const base_vec3<base_t> &vp3)
{

	base_vec3<base_t> vret=Cross_r(Vector_r(vp1, vp2),Vector_r(vp2, vp3));
	vret.normalize();
	return vret;
}
/**
 * @brief Vecteur 4D
 * Vecteur comprenant 4 flottants
 */
class vec4 {
public:
	vec4(void) : x(0), y(0), z(0), w(1) { }
	vec4(decimal _x,decimal _y,decimal _z,decimal _w) : x(_x), y(_y), z(_z), w(_w) { }
	vec4(const decimal *_v) : x(_v[0]), y(_v[1]), z(_v[2]), w(_v[3]) { }
	vec4(const vec3 &_v) : x(_v.x), y(_v.y), z(_v.z), w(1) { }
	vec4(const vec3 &_v,decimal _w) : x(_v.x), y(_v.y), z(_v.z), w(_w) { }
	vec4(const vec4 &_v) : x(_v.x), y(_v.y), z(_v.z), w(_v.w) { }

	int operator==(const vec4 &_v) { return (fabs(this->x - _v.x) < EPSILON && fabs(this->y - _v.y) < EPSILON && fabs(this->z - _v.z) < EPSILON && fabs(this->w - _v.w) < EPSILON); }
	int operator!=(const vec4 &_v) { return !(*this == _v); }

	vec4 &operator=(decimal _f) { this->x=_f; this->y=_f; this->z=_f; this->w=_f; return (*this); }
	const vec4 operator*(decimal _f) const { return vec4(this->x * _f,this->y * _f,this->z * _f,this->w * _f); }
	const vec4 operator/(decimal _f) const {
		if(fabs(_f) < EPSILON) return *this;
		_f = 1.0f / _f;
		return (*this) * _f;
	}
	const vec4 operator+(const vec4 &_v) const { return vec4(this->x + _v.x,this->y + _v.y,this->z + _v.z,this->w + _v.w); }
	const vec4 operator-() const { return vec4(-x,-y,-z,-w); }
	const vec4 operator-(const vec4 &_v) const { return vec4(this->x - _v.x,this->y - _v.y,this->z - _v.z,this->w - _v.w); }

	vec4 &operator*=(decimal _f) { return *this = *this * _f; }
	vec4 &operator/=(decimal _f) { return *this = *this / _f; }
	vec4 &operator+=(const vec4 &_v) { return *this = *this + _v; }
	vec4 &operator-=(const vec4 &_v) { return *this = *this - _v; }

	decimal operator*(const vec3 &_v) const { return this->x * _v.x + this->y * _v.y + this->z * _v.z + this->w; }
	decimal operator*(const vec4 &_v) const { return this->x * _v.x + this->y * _v.y + this->z * _v.z + this->w * _v.w; }

	operator decimal*() { return this->v; }
	operator const decimal*() const { return this->v; }
//	decimal &operator[](int _i) { return this->v[_i]; }
//	const decimal &operator[](int _i) const { return this->v[_i]; }

	void set(decimal _x,decimal _y,decimal _z,decimal _w) { this->x=_x; this->y=_y; this->z=_z; this->w=_w; }
	void reset(void) { this->x = this->y = this->z = this->w = 0; }
	bool compare(const vec4 &_v,decimal epsi=EPSILON) { return (fabs(this->x - _v.x) < epsi && fabs(this->y - _v.y) < epsi && fabs(this->z - _v.z) < epsi && fabs(this->w - _v.w) < epsi); }

	union {
		struct {decimal x,y,z,w;};
		struct {decimal s,t,p,q;};
		struct {decimal r,g,b,a;};
		struct {decimal fov,ratio,znear,zfar;};
		decimal v[4];
	};
};
/**
 * Calcul du determinant
 *
 * |x1 y1 z1 w1|
 * |x2 y2 z2 w2|
 * |x3 y3 z3 w3|
 * |x4 y4 z4 w4|
 */
inline decimal Determinant(const vec4 &vp1, const vec4 &vp2, const vec4 &vp3, const vec4 &vp4) {
	return -(vp1.w*(vp2.x*(vp3.y*vp4.z-vp4.y*vp3.z)-vp3.x*(vp2.y*vp4.z-vp4.y*vp2.z)+vp4.x*(vp2.y*vp3.z-vp3.y*vp2.z))-vp2.w*(vp1.x*(vp3.y*vp4.z-vp4.y*vp3.z)-vp3.x*(vp1.y*vp4.z-vp4.y*vp1.z)+vp4.x*(vp1.y*vp3.z-vp3.y*vp1.z))+vp3.w*(vp1.x*(vp2.y*vp4.z-vp4.y*vp2.z)-vp2.x*(vp1.y*vp4.z-vp4.y*vp1.z)+vp4.x*(vp1.y*vp2.z-vp2.y*vp1.z))-vp4.w*(vp1.x*(vp2.y*vp3.z-vp3.y*vp2.z)-vp2.x*(vp1.y*vp3.z-vp3.y*vp1.z)+vp3.x*(vp1.y*vp2.z-vp2.y*vp1.z)));
}
/**
 * Calcul du determinant
 *
 * |x1 y1 z1 1|
 * |x2 y2 z2 1|
 * |x3 y3 z3 1|
 * |x4 y4 z4 1|
 */
inline decimal Determinant(const vec3 &vp1, const vec3 &vp2, const vec3 &vp3, const vec3 &vp4) {
	return vp1.x*(vp2.y*(vp3.z-vp4.z)-vp3.y*(vp2.z-vp4.z)+vp4.y*(vp2.z-vp3.z))-vp2.x*(vp1.y*(vp3.z-vp4.z)-vp3.y*(vp1.z-vp4.z)+vp4.y*(vp1.z-vp3.z))+vp3.x*(vp1.y*(vp2.z-vp4.z)-vp2.y*(vp1.z-vp4.z)+vp4.y*(vp1.z-vp2.z))-vp4.x*(vp1.y*(vp2.z-vp3.z)-vp2.y*(vp1.z-vp3.z)+vp3.y*(vp1.z-vp2.z));
}

template<typename base_t>
inline base_vec3<base_t>::base_vec3(const vec4 &_v) {
	this->x = _v.x;
	this->y = _v.y;
	this->z = _v.z;
}
template<typename base_t>
inline base_t base_vec3<base_t>::operator*(const vec4 &_v) const {
	return this->x * _v.x + this->y * _v.y + this->z * _v.z + _v.w;
}


inline vec2::vec2(const vec4 &_v) {
	this->x = _v.x;
	this->y = _v.y;
}

template<typename base_t>
inline bool colinear( const base_vec3<base_t>& A, const base_vec3<base_t>& B, const base_vec3<base_t>& C, const decimal& aproximation)
{
	base_vec3<base_t> AB=B-A;
	base_vec3<base_t> AC=C-A;
	return (AB / AB.length() - AC / AC.length()).length() < aproximation;
}
/*****************************************************************************/
/*                                                                           */
/* ivec2                                                                     */
/*                                                                           */
/*****************************************************************************/

/**
 * @brief Vecteur 2D
 * Vecteur comprenant 2 entiers
 */
class ivec2 {
public:
	ivec2(void) : a(0), b(0) { }
	ivec2(long _a,long _b) : a(_a), b(_b) { }
	ivec2(const long *iv) : a(iv[0]), b(iv[1]) { }
	ivec2(const ivec2 &iv) : a(iv.a), b(iv.b) { }

	int operator==(const ivec2 &iv) { return ((this->a == iv.a) && (this->b == iv.b)); }
	int operator!=(const ivec2 &iv) { return !(*this == iv); }

	ivec2 &operator=(long _i) { this->x=_i; this->y=_i; return (*this); }
	const ivec2 operator*(long _i) const { return ivec2(this->a * _i,this->b * _i); }
	const ivec2 operator/(long _i) const { return ivec2(this->a / _i,this->b / _i); }
	const ivec2 operator+(const ivec2 &iv) const { return ivec2(this->a + iv.a,this->b + iv.b); }
	const ivec2 operator-() const { return ivec2(-this->a,-this->b); }
	const ivec2 operator-(const ivec2 &iv) const { return ivec2(this->a - iv.a,this->b - iv.b); }

	ivec2 &operator*=(long _i) { return *this = *this * _i; }
	ivec2 &operator/=(long _i) { return *this = *this / _i; }
	ivec2 &operator+=(const ivec2 &iv) { return *this = *this + iv; }
	ivec2 &operator-=(const ivec2 &iv) { return *this = *this - iv; }

	long operator*(const ivec2 &iv) const { return this->a * iv.a + this->b * iv.b; }

	operator long*() { return this->i; }
	operator const long*() const { return this->i; }
//	long &operator[](int _i) { return this->i[_i]; }
//	const long &operator[](int _i) const { return this->i[_i]; }

	void set(long _a,long _b) { this->a = _a; this->b = _b; }
	void reset(void) { this->a = this->b = 0; }
	void swap(ivec2 &iv) { long tmp=a; a=iv.a; iv.a=tmp; tmp=b; b=iv.b; iv.b=tmp; }
	void swap(ivec2 *iv) { this->swap(*iv); }

	union {
		struct {long a,b;};
		struct {long x,y;};
		long i[2];
	};
};

/*****************************************************************************/
/*                                                                           */
/* ivec3                                                                     */
/*                                                                           */
/*****************************************************************************/

/**
 * @brief Vecteur 3D
 * Vecteur comprenant 3 entiers
 */
class ivec3 {
public:
	ivec3(void) : a(0), b(0), c(0) { }
	ivec3(long _a,long _b,long _c) : a(_a), b(_b), c(_c) { }
	ivec3(const long *iv) : a(iv[0]), b(iv[1]), c(iv[2]) { }
	ivec3(const int *iv) : a(iv[0]), b(iv[1]), c(iv[2]) { }
	ivec3(const ivec3 &iv) : a(iv.a), b(iv.b), c(iv.c) { }
	ivec3(const ivec4 &iv);

	int operator==(const ivec3 &iv) { 
		return ((this->a == iv.a) && (this->b == iv.b) && (this->c == iv.c));
	}
	int operator!=(const ivec3 &iv) { return !(*this == iv); }

	ivec3 &operator=(long _i) { this->x=_i; this->y=_i; this->z=_i; return (*this); }
	ivec3 &operator=(vec3 &iv) { this->a = (long)iv[0]; this->b =  (long)iv[1]; this->c =  (long)iv[2]; return (*this); }
	const ivec3 operator*(long _i) const { return ivec3(this->a * _i,this->b * _i,this->c * _i); }
	const ivec3 operator/(long _i) const { return ivec3(this->a / _i,this->b / _i,this->c / _i); }
	const ivec3 operator+(const ivec3 &iv) const { return ivec3(this->a + iv.a,this->b + iv.b,this->c + iv.c); }
	const ivec3 operator-() const { return ivec3(-this->a,-this->b,-this->c); }
	const ivec3 operator-(const ivec3 &iv) const { return ivec3(this->a - iv.a,this->b - iv.b,this->c - iv.c); }

	ivec3 &operator*=(long _i) { return *this = *this * _i; }
	ivec3 &operator/=(long _i) { return *this = *this / _i; }
	ivec3 &operator+=(const ivec3 &iv) { return *this = *this + iv; }
	ivec3 &operator-=(const ivec3 &iv) { return *this = *this - iv; }

	long operator*(const ivec3 &iv) const { return this->a * iv.a + this->b * iv.b + this->c * iv.c; }
	long operator*(const ivec4 &iv) const;

	operator long*() { return this->i; }
	operator const long*() const { return this->i; }

	void set(long _a,long _b,long _c) { this->a = _a; this->b = _b; this->c = _c; }
	void set(int tab[3]) { this->a = tab[0]; this->b = tab[1]; this->c = tab[2]; }
	void set(int index, const long& value) { i[index] = value; }
	void reset(void) { this->a = this->b = this->c = 0; }
	void swap(ivec3 &iv) { long tmp=a; a=iv.a; iv.a=tmp; tmp=b; b=iv.b; iv.b=tmp; tmp=c; c=iv.c; iv.c=tmp; }
	void swap(ivec3 *iv) { this->swap(*iv); }
	/**
	 * Return vertices index corresponding to ccw triangle side
	 */
    ivec2 sideVertices(short idside)
	{
		switch(idside)
		{
			case 0:
				return ivec2(i[0],i[1]);
			case 1:
				return ivec2(i[1],i[2]);
			case 2:
				return ivec2(i[2],i[0]);
			default:
				throw "sideVertices index out of bound";
		}
	}
	union {
		struct {long a,b,c;};
		struct {long x,y,z;};
		struct {long red,green,blue;};
		long i[3];
	};
};

/*****************************************************************************/
/*                                                                           */
/* ivec4                                                                     */
/*                                                                           */
/*****************************************************************************/

/**
 * @brief Vecteur 4D
 * Vecteur comprenant 4 entiers
 */
class ivec4 {
public:
	ivec4(void) : a(0), b(0), c(0), d(1) { }
	ivec4(long _a,long _b,long _c,long _d) : a(_a), b(_b), c(_c), d(_d) { }
	ivec4(const long *iv) : a(iv[0]), b(iv[1]), c(iv[2]), d(iv[3]) { }
	ivec4(const int *iv) : a(iv[0]), b(iv[1]), c(iv[2]), d(iv[3]) { }
	ivec4(const ivec3 &iv) : a(iv.a), b(iv.b), c(iv.c), d(1) { }
	ivec4(const ivec3 &iv,long _d) : a(iv.a), b(iv.b), c(iv.c), d(_d) { }
	ivec4(const ivec4 &iv) : a(iv.a), b(iv.b), c(iv.c), d(iv.d) { }

	int operator==(const ivec4 &iv) { return ((this->a == iv.a) && (this->b == iv.b) && (this->c == iv.c) && (this->d == iv.d)); }
	int operator!=(const ivec4 &iv) { return !(*this == iv); }

	ivec4 &operator=(long _i) { this->x=_i; this->y=_i; this->z=_i; this->w=_i; return (*this); }
	const ivec4 operator*(long _i) const { return ivec4(this->a * _i,this->b * _i,this->c * _i,this->d * _i); }
	const ivec4 operator/(long _i) const { return ivec4(this->a / _i,this->b / _i,this->c / _i,this->d / _i); }
	const ivec4 operator+(const ivec4 &iv) const { return ivec4(this->a + iv.a,this->b + iv.b,this->c + iv.c,this->d + iv.d); }
	const ivec4 operator-() const { return ivec4(-this->a,-this->b,-this->c,-this->d); }
	const ivec4 operator-(const ivec4 &iv) const { return ivec4(this->a - iv.a,this->b - iv.b,this->c - iv.c,this->d - iv.d); }

	ivec4 &operator*=(long _i) { return *this = *this * _i; }
	ivec4 &operator/=(long _i) { return *this = *this / _i; }
	ivec4 &operator+=(const ivec4 &iv) { return *this = *this + iv; }
	ivec4 &operator-=(const ivec4 &iv) { return *this = *this - iv; }

	long operator*(const ivec3 &iv) const { return this->a * iv.a + this->b * iv.b + this->c * iv.c + this->d; }
	long operator*(const ivec4 &iv) const { return this->a * iv.a + this->b * iv.b + this->c * iv.c + this->d * iv.d; }

	operator long*() { return this->i; }
	operator const long*() const { return this->i; }
	long &operator[](int _i) { return this->i[_i]; }
	const long &operator[](int _i) const { return this->i[_i]; }

	void set(long _a,long _b,long _c,long _d) { this->a = _a; this->b = _b; this->c = _c; this->d = _d; }
	void reset(void) { this->a = this->b = this->c = this->d = 0; }
	void swap(ivec4 &iv) { long tmp=a; a=iv.a; iv.a=tmp; tmp=b; b=iv.b; iv.b=tmp; tmp=c; c=iv.c; iv.c=tmp; tmp=d; d=iv.d; iv.d=tmp; }
	void swap(ivec4 *iv) { this->swap(*iv); }

	union {
		struct {long a,b,c,d;};
		struct {long x,y,z,w;};
		struct {long red,green,blue,alpha;};
		long i[4];
	};
};

inline ivec3::ivec3(const ivec4 &iv) {
	this->a = iv.a;
	this->b = iv.b;
	this->c = iv.c;
}

inline long ivec3::operator*(const ivec4 &iv) const {
	return this->a * iv.a + this->b * iv.b + this->c * iv.c + iv.d;
}


inline decimal CalcTetraVolume(vec3 A,vec3 B,vec3 C,vec3 D)
{
	A-=D;
	B-=D;
	C-=D;
	return fabs((A.dot(Cross_r(B,C)))/6.f);
}
/**
 * @return La position du point central d'un triangle
 */
template<typename base_t>
inline base_vec3<base_t> GetGTriangle(const base_vec3<base_t>& A,const base_vec3<base_t>& B,const base_vec3<base_t>& C)
{
	base_vec3<base_t> I=(B+C)/2;
	base_vec3<base_t> AG=(I-A)*(2.f/3.f);
	return A+AG;
}
inline vec3 GetGTetra(const vec3& A,const vec3& B,const vec3& C,const vec3& D)
{
	return (A+B+C+D)/4;
}
template<typename base_t>
inline decimal GetAireTriangle(const base_vec3<base_t>& a,const base_vec3<base_t>& b,const base_vec3<base_t>& c)
{
	    base_vec3<base_t> ab;
		base_vec3<base_t> ac;
		Vector(a,b,ab);
		Vector(a,c,ac);
		ab.cross(ac);
		return .5*ab.length();
}
/*
 * @param ecart Au maximum 2*PI au minimum 0
 * @return Vrai si Si ecart==0 ou ecart>0.1
 */
template<typename base_t>
inline bool DotIsInVertex(const base_vec3<base_t>& dot,const base_vec3<base_t>& va,const base_vec3<base_t>& vb,const base_vec3<base_t>& vc,decimal* ecart=NULL)
{ //retourne vrai si le point est dans un triangle, le total à la fin correspond à environ 2PI si c'est le cas
	decimal totangle=0;
	// calcul des vecteurs des cotés
	base_vec3<base_t> vda(dot-va);
	base_vec3<base_t> vdb(dot-vb);
	base_vec3<base_t> vdc(dot-vc);
	//Calcul de la somme des angles sur le plan xy
	totangle+=vda.angle(vdb);
	totangle+=vda.angle(vdc);
	totangle+=vdb.angle(vdc);
	if(ecart)
		*ecart=fabs(M_2PI-totangle);
	if(int(totangle*10)==int(M_2PI*10) || !st_isfinite(totangle))
		return true;
	else
		return false;
}
/**
 * Retourne vrai si le point dotTest est dans le tetraedre formé par les sommets v1,v2,v3,v4
 * @ref http://steve.hollasch.net/cgindex/geometry/ptintet.html
 */
static bool DotInTetra(const vec3& dotTest,const vec3& v1,const vec3& v2,const vec3& v3,const vec3& v4)
{
	int sd0=SIGN(Determinant(v1,v2,v3,v4));
	if(SIGN(Determinant(dotTest,v2,v3,v4))!=sd0)
		return false;
	if(SIGN(Determinant(v1,dotTest,v3,v4))!=sd0)
		return false;
	if(SIGN(Determinant(v1,v2,dotTest,v4))!=sd0)
		return false;
	if(SIGN(Determinant(v1,v2,v3,dotTest))!=sd0)
		return false;
	return true;
}
/**
 * Calcul du segment entre les lignes (p1,p2) et (p3,p4)
 * @return Faux si aucune solution n'existe
 */
inline int LineLineIntersect(
   const vec3& p1,const vec3& p2,const vec3& p3,const vec3& p4,vec3* pa,vec3* pb,
   decimal* mua, decimal* mub)
{
   vec3 p13,p43,p21;
   decimal d1343,d4321,d1321,d4343,d2121;
   decimal numer,denom;

   p13.x = p1.x - p3.x;
   p13.y = p1.y - p3.y;
   p13.z = p1.z - p3.z;
   p43.x = p4.x - p3.x;
   p43.y = p4.y - p3.y;
   p43.z = p4.z - p3.z;
   if (fabs(p43.x)  < EPSILON && fabs(p43.y)  < EPSILON && fabs(p43.z)  < EPSILON)
      return(false);
   p21.x = p2.x - p1.x;
   p21.y = p2.y - p1.y;
   p21.z = p2.z - p1.z;
   if (fabs(p21.x)  < EPSILON && fabs(p21.y)  < EPSILON && fabs(p21.z)  < EPSILON)
      return(false);

   d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
   d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
   d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
   d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
   d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;

   denom = d2121 * d4343 - d4321 * d4321;
   if (fabs(denom) < EPSILON)
      return(false);
   numer = d1343 * d4321 - d1321 * d4343;

   *mua = numer / denom;
   *mub = (d1343 + d4321 * (*mua)) / d4343;

   pa->x = p1.x + *mua * p21.x;
   pa->y = p1.y + *mua * p21.y;
   pa->z = p1.z + *mua * p21.z;
   pb->x = p3.x + *mub * p43.x;
   pb->y = p3.y + *mub * p43.y;
   pb->z = p3.z + *mub * p43.z;

   return(true);
}
//const Vector& P, decimal* pfSParam, decimal* pfTParam
/**
 * Distance la plus courte entre un triangle et un point.
 * @param va Sommet A du triangle
 * @param vb Sommet B du triangle
 * @param vc Sommet C du triangle
 * @param P Point à comparer
 * @param pfSParam  Barycentric coordinate of triangle at point closest to p (u)
 * @param pfTParam  Barycentric coordinate of triangle at point closest to p (v)
 * @return Shortest distance squared.
 *
 * The third Barycentric coordinate is implicit, ie. w = 1.0 - u - v
 *
 * Taken from:
 * geometrictools.com
 * @see http://www.geometrictools.com/LibFoundation/Distance/Distance.html
 */

inline decimal ClosestDistanceBetweenDotAndTriangle(
			   const vec3& va,const vec3& vb,const vec3& vc, //Triangle
			   const vec3& P,
			   decimal* pfSParam,
			   decimal* pfTParam
			   )
{
	//-------------------------------------------------------------------
	// 2 edges of the triangle
	//-------------------------------------------------------------------
	vec3 E0 = (vb -va);
	vec3 E1 = (vc - va);

    vec3 kDiff = (va - P);
    decimal fA00 = E0 * E0;
    decimal fA01 = E0 * E1;
    decimal fA11 = E1 * E1;
    decimal fB0  = kDiff * E0;
	decimal fB1  = kDiff * E1;
	decimal fC   = kDiff * kDiff;
    decimal fDet = (decimal) fabs(fA00*fA11-fA01*fA01);
    decimal fS   = fA01*fB1-fA11*fB0;
    decimal fT   = fA01*fB0-fA00*fB1;
    decimal fSqrDist;

	if (fabs(fDet) < 0.00000001f)
		return 100000000.0f;

    if ( fS + fT <= fDet )
    {
        if ( fS < (decimal)0.0 )
        {
            if ( fT < (decimal)0.0 )  // region 4
            {
                if ( fB0 < (decimal)0.0 )
                {
                    fT = (decimal)0.0;
                    if ( -fB0 >= fA00 )
                    {
                        fS = (decimal)1.0;
                        fSqrDist = fA00+((decimal)2.0)*fB0+fC;
                    }
                    else
                    {
                        fS = -fB0/fA00;
                        fSqrDist = fB0*fS+fC;
                    }
                }
                else
                {
                    fS = (decimal)0.0;
                    if ( fB1 >= (decimal)0.0 )
                    {
                        fT = (decimal)0.0;
                        fSqrDist = fC;
                    }
                    else if ( -fB1 >= fA11 )
                    {
                        fT = (decimal)1.0;
                        fSqrDist = fA11+((decimal)2.0)*fB1+fC;
                    }
                    else
                    {
                        fT = -fB1/fA11;
                        fSqrDist = fB1*fT+fC;
                    }
                }
            }
            else  // region 3
            {
                fS = (decimal)0.0;
                if ( fB1 >= (decimal)0.0 )
                {
                    fT = (decimal)0.0;
                    fSqrDist = fC;
                }
                else if ( -fB1 >= fA11 )
                {
                    fT = (decimal)1.0;
                    fSqrDist = fA11+((decimal)2.0)*fB1+fC;
                }
                else
                {
                    fT = -fB1/fA11;
                    fSqrDist = fB1*fT+fC;
                }
            }
        }
        else if ( fT < (decimal)0.0 )  // region 5
        {
            fT = (decimal)0.0;
            if ( fB0 >= (decimal)0.0 )
            {
                fS = (decimal)0.0;
                fSqrDist = fC;
            }
            else if ( -fB0 >= fA00 )
            {
                fS = (decimal)1.0;
                fSqrDist = fA00+((decimal)2.0)*fB0+fC;
            }
            else
            {
                fS = -fB0/fA00;
                fSqrDist = fB0*fS+fC;
            }
        }
        else  // region 0
        {
            // minimum at interior point
            decimal fInvDet = ((decimal)1.0)/fDet;
            fS *= fInvDet;
            fT *= fInvDet;
            fSqrDist = fS*(fA00*fS+fA01*fT+((decimal)2.0)*fB0) +
                fT*(fA01*fS+fA11*fT+((decimal)2.0)*fB1)+fC;
        }
    }
    else
    {
        decimal fTmp0, fTmp1, fNumer, fDenom;

        if ( fS < (decimal)0.0 )  // region 2
        {
            fTmp0 = fA01 + fB0;
            fTmp1 = fA11 + fB1;
            if ( fTmp1 > fTmp0 )
            {
                fNumer = fTmp1 - fTmp0;
                fDenom = fA00-2.0f*fA01+fA11;
                if ( fNumer >= fDenom )
                {
                    fS = (decimal)1.0;
                    fT = (decimal)0.0;
                    fSqrDist = fA00+((decimal)2.0)*fB0+fC;
                }
                else
                {
                    fS = fNumer/fDenom;
                    fT = (decimal)1.0 - fS;
                    fSqrDist = fS*(fA00*fS+fA01*fT+2.0f*fB0) +
                        fT*(fA01*fS+fA11*fT+((decimal)2.0)*fB1)+fC;
                }
            }
            else
            {
                fS = (decimal)0.0;
                if ( fTmp1 <= (decimal)0.0 )
                {
                    fT = (decimal)1.0;
                    fSqrDist = fA11+((decimal)2.0)*fB1+fC;
                }
                else if ( fB1 >= (decimal)0.0 )
                {
                    fT = (decimal)0.0;
                    fSqrDist = fC;
                }
                else
                {
                    fT = -fB1/fA11;
                    fSqrDist = fB1*fT+fC;
                }
            }
        }
        else if ( fT < (decimal)0.0 )  // region 6
        {
            fTmp0 = fA01 + fB1;
            fTmp1 = fA00 + fB0;
            if ( fTmp1 > fTmp0 )
            {
                fNumer = fTmp1 - fTmp0;
                fDenom = fA00-((decimal)2.0)*fA01+fA11;
                if ( fNumer >= fDenom )
                {
                    fT = (decimal)1.0;
                    fS = (decimal)0.0;
                    fSqrDist = fA11+((decimal)2.0)*fB1+fC;
                }
                else
                {
                    fT = fNumer/fDenom;
                    fS = (decimal)1.0 - fT;
                    fSqrDist = fS*(fA00*fS+fA01*fT+((decimal)2.0)*fB0) +
                        fT*(fA01*fS+fA11*fT+((decimal)2.0)*fB1)+fC;
                }
            }
            else
            {
                fT = (decimal)0.0;
                if ( fTmp1 <= (decimal)0.0 )
                {
                    fS = (decimal)1.0;
                    fSqrDist = fA00+((decimal)2.0)*fB0+fC;
                }
                else if ( fB0 >= (decimal)0.0 )
                {
                    fS = (decimal)0.0;
                    fSqrDist = fC;
                }
                else
                {
                    fS = -fB0/fA00;
                    fSqrDist = fB0*fS+fC;
                }
            }
        }
        else  // region 1
        {
            fNumer = fA11 + fB1 - fA01 - fB0;
            if ( fNumer <= (decimal)0.0 )
            {
                fS = (decimal)0.0;
                fT = (decimal)1.0;
                fSqrDist = fA11+((decimal)2.0)*fB1+fC;
            }
            else
            {
                fDenom = fA00-2.0f*fA01+fA11;
                if ( fNumer >= fDenom )
                {
                    fS = (decimal)1.0;
                    fT = (decimal)0.0;
                    fSqrDist = fA00+((decimal)2.0)*fB0+fC;
                }
                else
                {
                    fS = fNumer/fDenom;
                    fT = (decimal)1.0 - fS;
                    fSqrDist = fS*(fA00*fS+fA01*fT+((decimal)2.0)*fB0) +
                        fT*(fA01*fS+fA11*fT+((decimal)2.0)*fB1)+fC;
                }
            }
        }
    }

    if ( pfSParam )
        *pfSParam = fS;

    if ( pfTParam )
        *pfTParam = fT;

    return (decimal) fabs(fSqrDist);
}
}; //fin namespace
using namespace core_mathlib;
#endif // __HMATHLIB__
