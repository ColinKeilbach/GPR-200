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
	gproVector.h
	Interface for vectors. Sets an example for C and C++ compatible headers.

	Modified by: Colin Keilbach
	Modified because: Lab for Graphics 1
*/

#include <stdio.h>
#include <stdlib.h>


#include "gpro/gpro-math/gproVector.h"


void testVector()
{
	// test array vector initializers and functions
	float3 av, bv, cv, dv;
	vec3default(av);								// -> a = (0, 0, 0)
	vec3init(bv, 1.0f, 2.0f, 3.0f);					// -> b = (1, 2, 3)
	vec3copy(dv, vec3init(cv, 4.0f, 5.0f, 6.0f));	// -> d = c = (4, 5, 6)
	vec3copy(av, dv);								// a = d			-> a = (4, 5, 6)
	vec3add(dv, bv);								// d += b			-> d = (4 + 1, 5 + 2, 6 + 3) = (5, 7, 9)
	vec3sum(dv, bv, bv);							// d = b + b		-> d = (1 + 1, 2 + 2, 3 + 3) = (2, 4, 6)
	vec3add(vec3sum(dv, cv, bv), av);				// d = c + b + a	-> d = (4 + 1 + 4, 5 + 2 + 5, 6 + 3 + 6) = (9, 12, 15)

#ifdef __cplusplus
	// test all constructors and operators
	vec3 a, b(1.0f, 2.0f, 3.0f), c(cv), d(c);		// default; init; copy array; copy
	a = d;											// assign						-> a = (4, 5, 6)
	d += b;											// add assign					-> d = (5, 7, 9)
	d = b + b;										// sum, init, assign			-> d = (2, 4, 6)
	d = c + b + a;									// sum, init, sum, init, assign	-> d = (9, 12, 15)
#endif	// __cplusplus
}

//Code by Daniel Buckstein
#ifdef __cplusplus
//C++ file io includes
#include <fstream>
#include <string>
#include <iostream>
#include <fstream>
#else // !__cplusplus
// C file io includes
#include <stdio.h>
#endif //__cplusplus

using namespace std;

vec3 colorRay(const ray& r) {
	if (sphere(vec3(0, 0, -1), 0.5, r)) { //Setting the origin vec3.z to anything but 0 does not render any red
		                                  //When set to 0 the entire image turns red
		return vec3(1, 0, 0);
	}
	float length = sqrt((r.direction().x * r.direction().x) + (r.direction().y * r.direction().y) + (r.direction().z * r.direction().z)); //Get length
	vec3 unitDirection = r.direction() / length; //Get unit direction
	float t = static_cast<float>((unitDirection.y + 1) * 0.5);
	return vec3(1.0f, 1.0f, 1.0f) * (1.0f - t) + vec3(0.5f, 0.7f, 1.0f) * t;
}

int main(int const argc, char const* const argv[])
{
	//Code by Daniel Buckstein
#ifdef __cplusplus
	std::ofstream file("openpls.txt"); //opens a file in in write mode in C++
	std::string test = "hello";        //Create string
	file << test << std::endl;
	file.close();
#else // !__cplusplus
// C file io includes

#endif //__cplusplus

	testVector();

	printf("\n\n");
	system("pause");

	//Code by Colin Keilbach

	//File stream for making image
	ofstream imageOut;
	imageOut.open("image.pmm");

	//Image Size
	const float aspect_ratio = 16.0f / 9.0f;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio); //Sets height based off of width

	//Render
	imageOut << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	//Camera
	float viewport_height = 2.0;
	float viewport_width = aspect_ratio * viewport_height;
	float focal_length = 1.0f;

	vec3 origin(0, 0, 0);
	vec3 horizontal(viewport_width, 0, 0);
	vec3 vertical(0, viewport_height, 0);
	vec3 lower_left_corner = origin - vertical / 2 - horizontal / 2 - vec3(0, 0, focal_length);

	//P3 means the colore are in ASCII
	// 255 for max color

	// Changing the symbols and numbers in the for loops changes the 
	// patterns and colors you get

	for (int j = image_height - 1; j >= 0; --j) {
		cout << "Scanlines left: " << j << endl; //Lines left
		for (int i = 0; i < image_width; ++i) {
			float u = float(i) / (image_width - 1);
			float v = float(i) / (image_height - 1);

			ray r(origin, lower_left_corner + horizontal * u + vertical * v - origin); //ray moves from the origin through the camera

			vec3 color = colorRay(r);

			//Setting up colors
			int ir = static_cast<int>(255.999 * color.x);
			int ig = static_cast<int>(255.999 * color.y);
			int ib = static_cast<int>(255.999 * color.z);

			//Image output
			imageOut << ir << ' ' << ig << ' ' << ib << "\n";
		}
		system("cls"); //Clear the console for progress
	}
	cout << "Done!\n";

	system("pause");
}
