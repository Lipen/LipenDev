#include <stdio.h>
#include <stdlib.h>

const int w = 500;
const int h = 500;

const double bound = 1.5;
const int frames_total = 100;
const int step_n = 8;
const double step_c_re = 0.005;
const double step_c_im = -0.0005;
double step_w_re;
double step_w_im;

const int clean_pre = 1;
const int clean_post = 0;


int main() {
	step_w_re = 2. * bound / w;
	step_w_im = 2. * bound / h;

	FILE* fo;
	char filename[64];

	double c_re = 0.0;
	double c_im = 0.0;
	double z_re, z_im, t;
	int i, j, n;
	int frame;

	system("mkdir _tmp");
	printf("Pre-cleaning...");
	if (clean_pre) {
		system("rm -f _tmp/*.pgm");
		printf(" done!\n");
	} else {
		printf(" skipped.\n");
	}


	for (frame = 1; frame <= frames_total; frame++) {
		sprintf(filename, "_tmp/julia%03d.pgm", frame);
		if (frame % 20 == 0)
			printf("Frame #%d :: filename: %s\n", frame, filename);

		if (!(fo = fopen(filename, "w")))
			perror("Bad fopen");

		fprintf(fo, "P2\n%d %d\n255\n", w, h);

		for (i = 0; i < w; i++) {
			for (j = 0; j < h; j++) {
				z_re = step_w_re * i - bound;
				z_im = step_w_im * j - bound;
				n = 255;

				while ((z_re*z_re+z_im*z_im < 100.) && n >= step_n) {
					t = z_re*z_re - z_im*z_im + c_re;
					z_im = 2*z_re*z_im + c_im;
					z_re = t;
					n -= step_n;
				}

				fprintf(fo, "%d ", n);
			}

			fprintf(fo, "\n");
		}

		fclose(fo);
		c_re += step_c_re;
		c_im += step_c_im;
	}


	printf("Converting into gif...");
	system("convert -delay 5 _tmp/*.pgm julia_simple.gif");
	printf(" done!\n");

	printf("Reconverting into cyclic gif...");
	system("convert julia_simple.gif -coalesce ( -clone -2-1 ) -quiet -layers OptimizePlus -loop 0 julia_opt.gif");
	printf(" done!\n");

	printf("Cleaning up...");
	if (clean_post) {
		system("rm _tmp/*.pgm");
		printf(" done!\n");
	} else {
		printf(" skipped.\n");
	}

	return 0;
}
