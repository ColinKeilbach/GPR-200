/*
   Copyright 2020 Daniel S. Buckstein

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

/*
	GPRO-Graphics1.c/.cpp
	Main source file for GPRO-Graphics1 library.

	Modified by: Colin Keilbach
	Modified because: Graphic programming class
*/

#ifndef _GPRO_VECTOR_H_
#define _GPRO_VECTOR_H_


#ifdef __cplusplus
// DB: link C++ symbols as if they are C where possible
extern "C" {
#else	// !__cplusplus
// DB: forward declare C types... why?
//	-> in C++ you would instantiate one of these like so: 
//		vec3 someVector;
//	-> in C you do it like this: 
//		union vec3 someVector;
//	-> this forward declaration makes the first way possible in both languages!
typedef union vec3 vec3;
#endif	// __cplusplus


// DB: declare shorthand types

typedef float float3[3];		// 3 floats form the basis of a float vector
typedef float* floatv;			// generic float vector (pointer)
typedef float const* floatkv;	// generic constant float vector (pointer)


// DB: declare 3D vector
//	-> why union? all data in the union uses the same address... in this case: 
//		'v' and 'struct...' share the same address
//	-> this means you can have multiple names for the same stuff!

// vec3
//	A 3D vector data structure.
//		member v: array (pointer) version of data
//		members x, y, z: named components of vector
union vec3
{
	float3 v;
	struct { float x, y, z; };

#ifdef __cplusplus
	// DB: in C++ we can have convenient member functions
	//	-> e.g. constructors, operators

	explicit vec3();	// default ctor
	explicit vec3(float const xc, float const yc = 0.0f, float const zc = 0.0f);	// init ctor w one or more floats
	explicit vec3(float3 const vc);	// copy ctor w generic array of floats
	vec3(vec3 const& rh);	// copy ctor

	vec3& operator =(vec3 const& rh);	// assignment operator (copy other to this)

	vec3& operator +=(vec3 const& rh);	// addition assignment operator (add other to this)

	vec3 const operator +(vec3 const& rh) const;	// addition operator (get sum of this and another)

#endif	// __cplusplus
};


// DB: declare C functions (all equivalents of above C++ functions are here)
//	-> return pointers so you can chain operations (they just take pointers)

floatv vec3default(float3 v_out);	// default init
floatv vec3init(float3 v_out, float const xc, float const yc, float const zc);	// init w floats
floatv vec3copy(float3 v_out, float3 const v_rh);	// init w array of floats (same as assign and both copy ctors)

floatv vec3add(float3 v_lh_sum, float3 const v_rh);	// add other to lh vector

floatv vec3sum(float3 v_sum, float3 const v_lh, float3 const v_rh);	// get sum of lh and rh vector


#ifdef __cplusplus
// DB: end C linkage for C++ symbols
}
#endif	// __cplusplus


// DB: include inline definitions for this interface
#include "_inl/gproVector.inl"


#endif	// !_GPRO_VECTOR_H_

//Code by Colin Keilbach
#include <cmath>

vec3 operator *(vec3 vec, float t) {
	vec3 out(vec.x * t, vec.y * t, vec.x * t); //Multiplay vec3 by float value
	return out;
}

vec3 operator *(vec3 vec, vec3 t) {
	vec3 out(vec.x * t.x, vec.y * t.y, vec.x * t.y); //Multiplay vec3 by vec3
	return out;
}

vec3 operator /(vec3 vec, float t) { //Divide vec3 by float
	vec3 out(vec.x / t, vec.y / t, vec.z / t);
	return out;
}

vec3 operator -(vec3 u, vec3 v) { //subtract vec3 by vec3
	vec3 out(u.x - v.x, u.y - v.y, u.z - v.z);
	return out;
}

float dot(vec3 u, vec3 v) { //get the dot product of 2 vec3
	return (u.x * v.x + u.y * v.y + u.z * v.z);
}

class ray {
public:
	//Constructors
	ray() {}
	ray(const vec3& origin, const vec3& direction) {
		orig = origin;
		dir = direction;
	}

	//Get Points
	vec3 origin() const { return orig; }
	vec3 direction() const { return dir; }

	//Gets ray location at t (time)
	vec3 at(float t) const {
		return orig + dir * t;
	}
private:
	vec3 orig; //where the ray starts
	vec3 dir; //direction the ray is pointing
};

//Code from https://raytracing.github.io/books/RayTracingInOneWeekend.html#addingasphere/creatingourfirstraytracedimage
bool sphere(const vec3& center, double radius, const ray& r) {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction()) * 2.0f;
	float c = static_cast<float>(dot(oc, oc) - radius * radius);
	float discriminant = b * b - 4 * a * c;

	return (discriminant > 0);
}