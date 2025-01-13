#include <PGM_HANDLER.h>
#include <Types.h>

#include <vector>

class Generator {
	private:

		unt N;
		unt scale;
		float width;
		float threshold = -1.0f;
		float (*profile)(float dist);
		Coordinate N2C(const unt n, const unt R);
		std::vector<Thread> RadonReadB;
		std::vector<Thread> RadonReadW;
		std::vector<Thread> threads;

	public:

		Generator(unt pN, unt pscale, float pwidth, float (&pprofile)(float));
		void Read(BWPGM& target, bool IncludeWhite, unt R);
		void calc(float darkness, const float pthreshold, bool chooseWhite, unt max_lines, unt R);
		void print(BWPGM& image, float intensity, unt R);
};
