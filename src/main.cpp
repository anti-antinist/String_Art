#include <PGM_HANDLER.h>
#include <String_Art.h>

#include <cassert>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

unsigned short N;
const unsigned short scale = 256;
float intensity;

float h, b, x1, r1;
unsigned int max_linesp, max_liness;
bool default_val = false;

std::string target_name, destination_name;

int main(int argc, const char** argv) {
	target_name		 = std::string(argv[1]);
	destination_name = std::string(argv[2]);
	N				 = std::atoi(argv[3]);
	intensity		 = std::atof(argv[4]);
	h				 = std::atof(argv[5]);
	b				 = std::atof(argv[6]);
	x1				 = std::atof(argv[7]);
	r1				 = std::atof(argv[8]);
	max_linesp		 = std::atoi(argv[9]);
	max_liness		 = std::atoi(argv[10]);
	default_val		 = std::stoi(argv[11]);

	BWPGM target(Coordinate(0, 0), target_name,
				 std::ios_base::in, scale);
	BWPGM image(Coordinate(500, 500), destination_name,
				std::ios_base::out, scale, default_val * (scale - 1));

	assert(target.file.is_open() && image.file.is_open());
	target.read();

	const unsigned short R = target.size().x / 2.0f - 1;
	Generator string_art(N, scale, b, h, b, x1, r1);
	string_art.Read(target, true, R);
	string_art.calc(intensity, !default_val, max_linesp, R);
	string_art.calc(intensity, default_val, max_liness, R);
	string_art.print(image, intensity, image.size().x / 2.0f - 1);

	image.print();
	return 0;
}