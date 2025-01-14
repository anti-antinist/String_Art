#include <String_Art.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "PGM_HANDLER.h"
#include "Types.h"

float Generator::h;
float Generator::b;
float Generator::x1;
float Generator::x2;
float Generator::r1;
float Generator::r2;

float Generator::profile(float dist) {
	dist		 = std::abs(dist);
	float result = 0;
	if(dist <= x1 * b) {
		result = r2 * h - r1 * h / (x1 * b) * dist + r1 * h;
	} else if(dist <= b) {
		result = r2 * h - r2 * h / (x2 * b) * (dist - x1 * b);
	}
	return result;
}

Generator::Generator(unt pN, unt pscale, float pwidth, float ph, float pb, float px1, float pr1) {

	N	  = pN;
	scale = pscale;
	width = pwidth;
	h	  = ph;
	b	  = ph;
	x1	  = px1;
	x2	  = 1.0f - x1;
	r1	  = pr1;
	r2	  = 1.0f - r1;
}

Coordinate Generator::N2C(const unt n, const unt R) {
	return Coordinate(std::round(R * std::cos(n * 2 * M_PI / N) + R), std::round(R * std::sin(n * 2 * M_PI / N) + R));
}

void Generator::Read(BWPGM& target, bool IncludeWhite, unt R) {
	float h = profile(0);
	for(unt N0 = 0; N0 < N - 1; N0++) {
		for(unt N1 = N0 + 1; N1 < N; N1++) {
			Coordinate first(N2C(N0, R));
			Coordinate second(N2C(N1, R));
			Coordinate delta(second.x - first.x, second.y - first.y);
			std::vector<Pixel> coords(target.Thread(first, second, width, profile));

			float BRT = 0;
			float WRT = 0;
			for(auto& i : coords) {
				BRT += (scale - 1 - i.value) / (scale - 1) * (scale - 1 - target.get(i.coord));
			}
			BRT /= coords.size();
			if(IncludeWhite) {

				for(auto& i : coords) {
					WRT += (scale - 1 - i.value) / (scale - 1) * (target.get(i.coord));
				}
				WRT /= coords.size();
				RadonReadW.push_back(Thread(N0, N1, WRT, false));
			}

			RadonReadB.push_back(Thread(N0, N1, BRT, true));

			std::cout << N0 << " " << N1 << " " << BRT << " " << WRT << std::endl;
		}
	}
}

void Generator::calc(float intensity, bool chooseWhite, unt max_lines, unt R) {
	float threshold = b * h * (1.0f - r1 + x1) * (scale - 1);
	std::vector<Thread>* RadonRead;
	if(chooseWhite) {
		RadonRead = &RadonReadW;
	} else {
		RadonRead = &RadonReadB;
	}
	for(unt l = 0; l < max_lines; l++) {
		auto predicate = [&](auto& p) -> bool {
			return p.RT < threshold;
		};
		if(RadonRead->size() == 0) {
			break;
		}
		Thread best = *std::max_element(
			RadonRead->begin(), RadonRead->end(),
			[](const auto& a, const auto& b) {
				return a.RT < b.RT;
			});
		if(predicate(best)) {
			break;
		}
		std::cout << l << " " << RadonRead->size() << " " << ((best.isBlack) ? "Black" : "White") << " " << best.RT - threshold << " " << best.N0 << " " << best.N1 << std::endl;
		Coordinate start(N2C(best.N0, R));
		Coordinate end(N2C(best.N1, R));
		threads.push_back(best);

		for(unt j = 0; j < RadonRead->size();) {
			auto P = (RadonRead->begin() + j);
			unt N0 = P->N0, N1 = P->N1;
			std::pair<unt, unt> p = std::pair<unt, unt>(N0, N1);
			if((N0 < best.N0 && best.N1 - best.N0 + N1 - N0 > best.N1 - N0)
			   || (N0 > best.N0 && best.N1 - best.N0 + N1 - N0 > N1 - best.N0)
					  && (best.N1 - best.N0 + N1 - N0 > best.N1 - N0)) {
				j++;
				continue;
			}
			float area = 0.0f;
			Coordinate start1(N2C(N0, R));
			Coordinate end1(N2C(N1, R));
			float theta = 0;
			if(start == start1 && end == end1) {
				area = h * intensity * (scale - 1);
			} else {
				if(start.x - end.x != 0 && start1.x - end1.x != 0) {
					float m1 = (end.y - start.y) / (end.x - start.x);
					float m2 = (end1.y - start1.y) / (end1.x - start1.x);
					theta	 = std::atan(m1) - atan(m2);
				} else if(start.x - end.x == 0) {
					float m2 = (end1.y - start1.y) / (end1.x - start1.x);
					theta	 = M_PI / 2 - atan(m2);
				} else if(start1.x - end1.x == 0) {
					float m1 = (end.y - start.y) / (end.x - start.x);
					theta	 = std::atan(m1) - M_PI / 2;
				}
				if(theta != 0) {
					area  = 4 / std::abs(std::sin(theta));
					area *= h * intensity * (scale - 1);
					area /= std::sqrt((start1.x - end1.x) * (start1.x - end1.x) + (start1.y - end1.y) * (start1.y - end1.y)) * 2 * width;
				} else {
					area = h * intensity * (scale - 1);
				}
			}
			P->RT -= area;
			j++;
			if(predicate(*P)) {
				RadonRead->erase(P);
				j--;
			}
		}
	}
}

void Generator::print(BWPGM& image, float intensity, unt R) {
	for(auto& i : threads) {
		image.add(image.Thread(N2C(i.N0, R), N2C(i.N1, R), width, profile), (i.isBlack) ? -intensity : intensity);
	}
	threads.clear();
}