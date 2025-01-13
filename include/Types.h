#pragma once

#define unt unsigned int
#define uns unsigned short

struct Pixel;
struct Coordinate;
struct Coordinate;

struct Thread {
	public:

		Thread(unt pN0, unt pN1, float pRT, bool pisBlack) {
			N0		= pN0;
			N1		= pN1;
			RT		= pRT;
			isBlack = pisBlack;
		}
		unt N0	 = 0,
			N1	 = 0;
		float RT = 0;
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