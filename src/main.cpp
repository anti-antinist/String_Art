#include <PGM_HANDLER.h>
#include <String_Art.h>

#include <cassert>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "Types.h"

uns N;
const uns scale = 256;
float intensity;

float h;
float b;
float x1;
float x2;
float r1;
float r2;
float profile(float dist) {
	dist		 = std::abs(dist);
	float result = 0;
	if(dist <= x1 * b) {
		result = r2 * h - r1 * h / (x1 * b) * dist + r1 * h;
	} else if(dist <= b) {
		result = r2 * h - r2 * h / (x2 * b) * (dist - x1 * b);
	}
	return result;
}
float profile1(float dist) {
	dist		 = std::abs(dist);
	float result = 0;
	if(dist <= b) {
		result = 1.0f;
	} else if(dist > b) {
		result = 0.0f;
	}
	return result;
}
int main(int argc, char* argv[]) {

	assert(argc == 9);
	N					 = atoi(argv[1]);
	intensity			 = atof(argv[2]);
	h					 = atof(argv[3]);
	b					 = atof(argv[4]);
	x1					 = atof(argv[5]);
	r1					 = atof(argv[6]);
	const unt max_linesb = atof(argv[7]);
	const unt max_linesw = atof(argv[8]);
	/*N					 = 28;
	intensity			 = 0.5f;
	h					 = 1.0f;
	b					 = 1.708f;
	x1					 = 1.0f;
	r1					 = 1.0f;
	const unt max_linesb = 10'000;
	const unt max_linesw = 1'000;*/

	BWPGM target(Coordinate(0, 0), "../Images/target.pgm",
				 std::ios_base::in, scale);

	target.read();
	BWPGM image(Coordinate(500, 500), "../Images/image.pgm",
				std::ios_base::out, scale);

	const unt R = target.size().x / 2.0f - 1.0f;
	Generator string_art(N, scale, b, profile);
	string_art.Read(target, true, R);
	const float threshold = b * h * (r2 + x1) * (scale - 1);
	string_art.calc(intensity, threshold, false, max_linesb, R);
	string_art.calc(intensity, threshold, true, max_linesw, R);
	std::cout << threshold << std::endl;
	string_art.print(image, intensity, image.size().x / 2.0f - 1);
	image.print();
	return 0;
}