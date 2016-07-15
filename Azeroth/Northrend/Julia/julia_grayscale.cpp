#include <iostream>
#include <fstream>
#include <complex>
#include <cmath>

using std::cout;
using std::endl;
using complex = std::complex<double>;

const int w = 300;
const int h = 300;

const double bound = 1.5;
const int frames_total = 50;  // delay is 0.05s + double duration to cycle
const int step_n = 5;
const double z_threshold = 2.;  // Julia set threshold is 2
const double step_c_re = -0.005;  // step_c_re*frames_total = c_re at the end
const double step_c_im = 0.005;
const double step_z_re = 2*bound / w;
const double step_z_im = 2*bound / h;
const double init_c_re = 0;  // 0
const double init_c_im = 0.5;
const double init_z_re = -bound;
const double init_z_im = -bound;

const bool clean_pre = true;
const bool clean_post = false;


int main(/*int argc, char* argv[]*/) {
	system("mkdir _tmp");
	cout << "Pre-cleaning...";
	if (clean_pre) {
		system("rm -f _tmp/*.pgm");
		cout << " done!" << '\n';
	} else {
		cout << " skipped." << '\n';
	}

	cout << "Calculating..." << '\n';

	for (int frame = 1; frame <= frames_total; frame++) {
		complex c {frame*step_c_re + init_c_re, frame*step_c_im + init_c_im};

		char filename[64]; sprintf(filename, "_tmp/julia%03d.pgm", frame);
		std::ofstream fo(std::string(filename), std::ios::binary);

		fo << "P2\n" << w << ' ' << h << '\n' << 255 << '\n';

// cout << "Frame #" << frame << " ~> " << filename << '\n';

		for (int i = 0; i < w; i++) {
			for (int j = 0; j < h; j++) {
				complex z {i*step_z_re + init_z_re, j*step_z_im + init_z_im};
				int n = 255;

				while (std::abs(z) < z_threshold && n >= step_n) {
					z = std::pow(z, 2) + c;
					n -= step_n;
				}

				fo << n << ' ';
			}

			fo << '\n';
		}

		fo.close();
	}


	printf("Converting into gif...");
	system("convert -delay 5 _tmp/*.pgm julia_grayscale_simple.gif");
	printf(" done!\n");

	printf("Reconverting into cyclic gif...");
	system("convert julia_grayscale_simple.gif -coalesce ( -clone -2-1 ) -quiet -layers OptimizePlus -loop 0 julia_grayscale_opt.gif");
	printf(" done!\n");

	printf("Cleaning up...");
	if (clean_post) {
		system("rm _tmp/*.pgm");
		system("rmdir _tmp");
		printf(" done!\n");
	} else {
		printf(" skipped.\n");
	}

	return 0;
}
