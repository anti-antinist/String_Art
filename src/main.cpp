#include <PGM_HANDLER.h>
#include <String_Art.h>

#include <cassert>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include "Types.h"

uns N;
const uns scale = 256;
float intensity;

float h;
float b;
float x1;
float r1;
unt max_linesb, max_linesw;
std::string target_name, destination_name;

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
int main(int argc, const char** argv) {
	target_name		 = std::string(argv[1]);
	destination_name = std::string(argv[2]);
	N				 = std::atoi(argv[3]);
	intensity		 = std::atof(argv[4]);
	h				 = std::atof(argv[5]);
	b				 = std::atof(argv[6]);
	x1				 = std::atof(argv[7]);
	r1				 = std::atof(argv[8]);
	max_linesb		 = std::atof(argv[9]);
	max_linesw		 = std::atof(argv[10]);

	BWPGM target(Coordinate(0, 0), target_name,
				 std::ios_base::in, scale);
	BWPGM image(Coordinate(500, 500), destination_name,
				std::ios_base::out, scale);

	assert(target.file.is_open() && image.file.is_open());
	target.read();

	const unt R = target.size().x / 2.0f - 1.0f;

	Generator string_art(N, scale, b, h, b, x1, r1);
	string_art.Read(target, true, R);
	string_art.calc(intensity, false, max_linesb, R);
	string_art.calc(intensity, true, max_linesw, R);
	string_art.print(image, intensity, image.size().x / 2.0f - 1);

	image.print();
	return 0;
}