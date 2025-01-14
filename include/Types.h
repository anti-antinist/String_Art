#pragma once

struct Pixel;
struct Coordinate;
struct Coordinate;

struct Thread {
	public:

		Thread(unsigned short pN0, unsigned short pN1, float pRT, bool pisBlack) {
			N0		= pN0;
			N1		= pN1;
			RT		= pRT;
			isBlack = pisBlack;
		}
		unsigned short N0 = 0,
					   N1 = 0;
		float RT		  = 0;
		bool isBlack;
};

struct Coordinate {
	public:

		float x = 0, y = 0;
		Coordinate(float px, float py);
		Coordinate() = default;
		bool operator==(const Coordinate coord) {
			return this->x == coord.x && this->y == coord.y;
		}
};

struct Pixel {
	public:

		Coordinate coord;
		float value;
		Pixel(Coordinate, float);
};