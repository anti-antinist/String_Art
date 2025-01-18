#pragma once
#include <Types.h>

#include <fstream>
#include <string>
#include <vector>

class BWPGM {
	private:

		unsigned int ID(Coordinate);
		bool check(Coordinate);
		void set(Coordinate, float);

		unsigned int width, height;
		std::vector<unsigned short> img;
		unsigned short scale = 0;

	public:

		BWPGM(Coordinate, std::string, std::ios_base::openmode, unsigned short, unsigned short = 0);
		std::vector<Pixel> Thread(Coordinate, Coordinate, float, float(float));
		std::vector<Coordinate> line(Coordinate, Coordinate, bool);
		void read();
		void print(std::string filename = "");
		void add(std::vector<Pixel>, float);
		unsigned short get(Coordinate);
		Coordinate size() {
			return Coordinate(this->width, this->height);
		};
		std::fstream file;
};