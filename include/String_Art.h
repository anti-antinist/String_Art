#include <PGM_HANDLER.h>
#include <Types.h>

#include <vector>

class Generator {
	private:

		unsigned int N;
		unsigned int scale;
		float width;
		static float h;
		static float b;
		static float x1;
		static float x2;
		static float r1;
		static float r2;
		static float profile(float dist);
		Coordinate N2C(const unsigned int n, const unsigned int R);
		std::vector<Thread> RadonReadB;
		std::vector<Thread> RadonReadW;
		std::vector<Thread> threads;

	public:

		Generator(unsigned int pN, unsigned int pscale, float pwidth, float ph, float pb, float px1, float pr1);
		void Read(BWPGM& target, bool IncludeWhite, unsigned int R);
		void calc(float darkness, bool chooseWhite, unsigned int max_lines, unsigned int R);
		void print(BWPGM& image, float intensity, unsigned int R);
};
