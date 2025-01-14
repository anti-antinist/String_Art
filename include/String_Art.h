#include <PGM_HANDLER.h>
#include <Types.h>

#include <vector>

class Generator {
	private:

		unt N;
		unt scale;
		float width;
		static float h;
		static float b;
		static float x1;
		static float x2;
		static float r1;
		static float r2;
		static float profile(float dist);
		Coordinate N2C(const unt n, const unt R);
		std::vector<Thread> RadonReadB;
		std::vector<Thread> RadonReadW;
		std::vector<Thread> threads;

	public:

		Generator(unt pN, unt pscale, float pwidth, float ph, float pb, float px1, float pr1);
		void Read(BWPGM& target, bool IncludeWhite, unt R);
		void calc(float darkness, bool chooseWhite, unt max_lines, unt R);
		void print(BWPGM& image, float intensity, unt R);
};
