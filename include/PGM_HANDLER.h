#pragma once
#include <Types.h>

#include <fstream>
#include <string>
#include <vector>

class BWPGM {
	private:

		unt ID(Coordinate);
		bool check(Coordinate);
		void set(Coordinate, float);

		unt width, height;
		std::vector<uns> img;
		uns scale = 0;

	public:

		BWPGM(Coordinate, std::string, std::ios_base::openmode, uns);
		std::vector<Pixel> Thread(Coordinate, Coordinate, float, float(float));
		std::vector<Coordinate> line(Coordinate, Coordinate, bool);
		void read();
		void print(std::string filename = "");
		void add(std::vector<Pixel>, float);
		void makeCircle(unt R);
		uns get(Coordinate);
		Coordinate size() {
			return Coordinate(this->width, this->height);
		};
		std::fstream file;
};