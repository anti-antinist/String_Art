#include <PGM_HANDLER.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>

Coordinate::Coordinate(float px, float py) {
	x = px;
	y = py;
}

Pixel::Pixel(Coordinate pcoord, float pvalue) {
	coord = pcoord;
	value = pvalue;
}

BWPGM::BWPGM(Coordinate size, std::string filename,
			 std::ios_base::openmode mode, unsigned short pscale, unsigned short default_val) {
	width  = size.x;
	height = size.y;
	img.resize(size.x * size.y, default_val);
	file.open(filename, mode);
	if(mode == std::ios_base::out) {
		file << "P5" << std::endl;
		file << size.x << " " << size.y << std::endl;
		file << pscale - 1 << std::endl;
	}
	scale = pscale;
}

unsigned int BWPGM::ID(Coordinate coord) {
	return coord.x + (height - 1 - coord.y) * width;
}

bool BWPGM::check(Coordinate coord) {
	if(coord.x < 0 || coord.x >= width || coord.y < 0 || coord.y >= height) {
		return false;
	}
	return true;
}

void BWPGM::set(Coordinate coord, float i) {
	if(!check(coord)) {
		return;
	}
	if(i <= 0) {
		i = 0;
	} else if(i >= scale)
		i = scale - 1;
	img.at(ID(coord)) = i;
}

std::vector<Coordinate> BWPGM::line(Coordinate first, Coordinate second,
									bool to_check) {
	first			 = Coordinate(std::round(first.x), std::round(first.y));
	second			 = Coordinate(std::round(second.x), std::round(second.y));
	Coordinate delta = Coordinate(second.x - first.x, second.y - first.y);
	std::vector<Coordinate> result;
	if(delta.x == 0) {
		if(first.y > second.y) {
			std::swap(first.x, second.x);
			std::swap(first.y, second.y);
			delta.x = -delta.x;
			delta.y = -delta.y;
		}
		for(unsigned short y = 0; y <= delta.y; y++) {
			Coordinate coord(std::round(first.x), round(y + first.y));
			if(!to_check) {
				result.push_back(coord);
			} else if(check(coord)) {
				result.push_back(coord);
			}
		}
	} else if(std::abs(delta.x) >= std::abs(delta.y)) {
		if(first.x > second.x) {
			std::swap(first.x, second.x);
			std::swap(first.y, second.y);
			delta.x = -delta.x;
			delta.y = -delta.y;
		}
		float m		 = delta.y / delta.x;
		bool falling = m < 0.0f;
		float y		 = first.y;
		for(unsigned short i = 0; i < delta.x; i++) {
			Coordinate coord(std::round(i + first.x), std::round(y));
			if(!to_check) {
				result.push_back(coord);
			} else if(check(coord)) {
				result.push_back(coord);
			}
			float p = first.y + (i + 1) * m;
			if(falling) {
				if(p - (y - 1) <= y - p) {
					y--;
				}
			} else {
				if(y + 1 - p <= p - y) {
					y++;
				}
			}
		}
	} else if(std::abs(delta.x) < std::abs(delta.y)) {
		if(first.y > second.y) {
			std::swap(first.x, second.x);
			std::swap(first.y, second.y);
			delta.x = -delta.x;
			delta.y = -delta.y;
		}
		float m		 = delta.y / delta.x;
		float x		 = first.x;
		bool falling = delta.x < 0.0f;
		for(unsigned short i = 0; i <= delta.y; i++) {
			Coordinate coord(std::round(x), std::round(first.y + i));
			if(!to_check) {
				result.push_back(coord);
			} else if(check(coord)) {
				result.push_back(coord);
			}
			float p = (i + 1) / m + first.x;
			if(falling) {
				if(p - (x - 1) <= x - p) {
					x--;
				}
			} else {
				if(x + 1 - p <= p - x) {
					x++;
				}
			}
		}
	}
	return result;
}

std::vector<Pixel> BWPGM::Thread(Coordinate first, Coordinate second, float normal_dist, float(profile)(float)) {
	first  = Coordinate(std::round(first.x), std::round(first.y));
	second = Coordinate(std::round(second.x), std::round(second.y));
	assert(check(first) && check(second));
	Coordinate delta(second.x - first.x, second.y - first.y);
	std::vector<Pixel> result;
	auto push_points = [&](std::vector<Coordinate>& coords) -> void {
		const int constant_1 = second.x * first.y - second.y * first.x,
				  constant_2 = sqrt(delta.y * delta.y + delta.x * delta.x);
		for(auto& i : coords) {
			if(!check(i)) {
				continue;
			}
			float distance = (i.x * delta.y - i.y * delta.x + constant_1) / constant_2;
			result.push_back(Pixel(i, profile(distance)));
		}
	};
	if(normal_dist < 1.0f) {
		std::vector<Coordinate> Line(line(first, second, false));
		push_points(Line);
	} else if(delta.x == 0) {
		std::vector<Coordinate> points;
		int left_end  = std::ceil(float(first.x) - normal_dist);
		int right_end = std::floor(float(first.x) + normal_dist);
		for(int i = left_end; i <= right_end; i++) {
			Coordinate start(i, first.y);
			Coordinate end(i, second.y);
			std::vector<Coordinate> Line(line(start, end, false));
			points.insert(points.end(), Line.begin(), Line.end());
		}
		push_points(points);
	} else {
		std::vector<Coordinate> points;
		float m = delta.y / delta.x;
		Coordinate first_start(first.x + (normal_dist * m) / sqrt(1 + m * m),
							   first.y - normal_dist / sqrt(1 + m * m));
		Coordinate first_end(first.x - (normal_dist * m) / sqrt(1 + m * m),
							 first.y + normal_dist / sqrt(1 + m * m));
		Coordinate second_start(second.x + (normal_dist * m) / sqrt(1 + m * m),
								second.y - normal_dist / sqrt(1 + m * m));
		Coordinate second_end(second.x - (normal_dist * m) / sqrt(1 + m * m),
							  second.y + normal_dist / sqrt(1 + m * m));
		std::vector<Coordinate> Line4(line(first_start, second_start, false));
		std::vector<Coordinate> Line2(line(first_end, second_end, false));
		std::vector<Coordinate> Line1(line(first_start, first_end, false));
		std::vector<Coordinate> Line3(line(second_start, second_end, false));

		points.insert(points.end(), Line1.begin(), Line1.end());
		points.insert(points.end(), Line2.begin(), Line2.end());
		points.insert(points.end(), Line3.begin(), Line3.end());
		points.insert(points.end(), Line4.begin(), Line4.end());

		unsigned short max_y = std::round(std::max({first_end.y, second_end.y}));
		unsigned short min_y = std::round(std::min({first_start.y, second_start.y}));
		if(std::round(std::min({first_start.y, second_start.y})) < 0) {
			min_y = 0;
		}
		for(unsigned short i = min_y; i <= max_y; i++) {
			std::vector<Coordinate> scanline;
			std::copy_if(points.begin(), points.end(), std::back_inserter(scanline),
						 [&](Coordinate coord) {
							 return coord.y == i;
						 });
			if(scanline.size() == 0) continue;
			auto max = *std::max_element(scanline.begin(), scanline.end(), [](auto& a, auto& b) -> bool {
				return a.x < b.x;
			});
			auto min = *std::min_element(scanline.begin(), scanline.end(), [](auto& a, auto& b) -> bool {
				return a.x < b.x;
			});
			Coordinate start(min.x, i);
			Coordinate end(max.x, i);
			std::vector<Coordinate> Line(line(start, end, false));
			points.insert(points.end(), Line.begin(), Line.end());
		}

		push_points(points);
	}
	return result;
}

void BWPGM::read() {
	std::string line;
	std::getline(file, line);
	std::getline(file, line);

	width =
		atoi(std::string(line.begin(), line.begin() + line.find(' ')).c_str());
	height =
		atoi(std::string(line.begin() + line.find(' ') + 1, line.end()).c_str());

	std::getline(file, line);
	scale			 = atoi(line.c_str());
	std::string data = "";
	img.resize(height * width);
	for(unsigned int i = 0; i < width * height; i++) {
		uint8_t value;
		file.read(reinterpret_cast<char*>(&value), sizeof(uint8_t));
		img[i] = value;
	}
	file.close();
}

void BWPGM::print(std::string filename) {
	if(filename == "") {
		for(unsigned int i = 0; i < img.size(); i++) {
			uint8_t value = img[i];
			file.write(reinterpret_cast<char*>(&value), sizeof(uint8_t));
		}
		file.close();
	} else {
		std::fstream file1;
		file1.open(filename, std::ios_base::out);
		file1 << "P5" << std::endl;
		file1 << width << " " << width << std::endl;
		file1 << scale << std::endl;
		for(unsigned int i = 0; i < img.size(); i++) {
			file1 << img.at(i) << " ";
		}
		file1.close();
	}
}

void BWPGM::add(std::vector<Pixel> arr, float tint) {
	if(tint < 0.0f)
		for(auto& pixel : arr) {
			set(pixel.coord, img[ID(pixel.coord)] * (1 + tint * pixel.value));
		}
	else
		for(auto& pixel : arr) {
			set(pixel.coord, img[ID(pixel.coord)] + tint * pixel.value * (scale - 1));
		}
}
unsigned short BWPGM::get(Coordinate coord) {
	assert(check(coord));
	return img[ID(coord)];
}
