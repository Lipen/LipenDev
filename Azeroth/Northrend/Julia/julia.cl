// #pragma OPENCL_EXTENSION cl_khr_fp64 : enable

typedef float double;
// typedef double2 Complex;
typedef float2 Complex;

Complex multiply(Complex a, Complex b) {
    return (Complex)(a.s0*b.s0-a.s1*b.s1, a.s1*b.s0+a.s0*b.s1);
}

void kernel calculate_julia(global unsigned char* data, int w, int steps_total, double init_z_re, double init_z_im, double step_z_re, double step_z_im, double c_re, double c_im) {
	int i = get_global_id(0);
	int j = get_global_id(1);

	Complex c = (Complex)(c_re, c_im);
	Complex z = (Complex)(i*step_z_re + init_z_im, j*step_z_im + init_z_im);
	int n = 0;

	while (z.s0*z.s0+z.s1*z.s1 < 4. && n++ < 1.2*steps_total) {
		z = multiply(z, z) + c;
	}

	unsigned char r_, g_, b_;
	double r, g, b;
	// double h = ((n + 142) % 256) / 255.;
	double h = ((int)(n*256./steps_total + 142) % 256) / 255.;
	double s = 1.;
	double v = 1. * (n < 1.2*steps_total);

	if (s == 0)
		r = g = b = v;
	else {
		if (h == 1)  h = 0;

		int i = h * 6;
		double f = h * 6 - i;
		double p = v * (1 - s);
		double q = v * (1 - f * s);
		double t = v * (1 - (1 - f) * s);

		switch (i % 6){
			case 0: r = v; g = t; b = p; break;
			case 1: r = q; g = v; b = p; break;
			case 2: r = p; g = v; b = t; break;
			case 3: r = p; g = q; b = v; break;
			case 4: r = t; g = p; b = v; break;
			case 5: r = v; g = p; b = q; break;
		}
	}

	r_ = 255 * r;
	g_ = 255 * g;
	b_ = 255 * b;

	int x = i + w*j;
	data[3*x+0] = r_;
	data[3*x+1] = g_;
	data[3*x+2] = b_;
}
