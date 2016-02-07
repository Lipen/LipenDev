#include <CL/cl.hpp>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::endl;


const int w = 1000;
const int h = 1000;
const double bound = 1.2;

const int frames_total = 200;  // delay is 0.05s + double duration to cycle
const int steps_total = 32;  // Colors amount
const double step_c_re = 0.0002;  // step_c_re*frames_total = c_re at the end
const double step_c_im = -0.004;
const float step_z_re = 2*bound / w;
const float step_z_im = 2*bound / h;
const double init_c_re = 0.305;
const double init_c_im = -0.3;
const float init_z_re = -bound;
const float init_z_im = -bound;

const bool clean_pre = true;
const bool clean_post = false;


int main() {
	system("mkdir _tmp");
	cout << "Pre-cleaning...";
	if (clean_pre) {
		system("rm -f _tmp/*.ppm");
		system("rm -f _tmp/*.png");
		cout << " done!" << '\n';
	} else {
		cout << " skipped." << '\n';
	}

// OpenCL setup
	std::vector<cl::Platform> all_platforms;
	std::vector<cl::Device> all_devices;

	cl::Platform::get(&all_platforms);
	cl::Platform default_platform = all_platforms[1];
	default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
	cl::Device default_device = all_devices[0];

	cout << "Platforms list:" << endl;
	for (auto& item : all_platforms) {
		cout << '\t' << item.getInfo<CL_PLATFORM_NAME>() << endl;
	}
	cout << "Devices list:" << endl;
	for (auto&& item : all_devices) {
		cout << "  " << item.getInfo<CL_DEVICE_NAME>() << endl;
		cout << "    \"CL_DEVICE_MAX_COMPUTE_UNITS\" = \t" << item.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << endl;
		cout << "    \"CL_DEVICE_MAX_WORK_GROUP_SIZE\" = " << item.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>() << endl;
	}

	cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>() <<"\nUsing device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<endl;

	std::ifstream cl_file("julia.cl");
	std::string cl_string(std::istreambuf_iterator<char>(cl_file), (std::istreambuf_iterator<char>()));

	cl::Context context({default_device});
	cl::CommandQueue queue(context, default_device);
	cl::Program::Sources source(1, {cl_string.c_str(), cl_string.length()+1});
	cl::Program program(context, source);
	if (program.build({default_device}) != CL_SUCCESS)
		cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << "\n";

	cl::Kernel kernel(program, "calculate_julia");
	kernel.setArg(1, w);
	kernel.setArg(2, steps_total);
	kernel.setArg(3, init_z_re);
	kernel.setArg(4, init_z_im);
	kernel.setArg(5, step_z_re);
	kernel.setArg(6, step_z_im);

cout << "Calculating...\n";

	for (int frame = 1; frame <= frames_total; frame++) {
		if (frame%10 == 0)
			cout << "Frame " << frame << " / " << frames_total << " ...\n";
	// Format file name
		char buf[32]; snprintf(buf, 32, "_tmp/julia%03d", frame);
		std::string filename(buf);  // Without extension

	// Pass some more arguments to the kernel
		float c_re = frame*step_c_re + init_c_re;
		float c_im = frame*step_c_im + init_c_im;
		kernel.setArg(7, c_re);
		kernel.setArg(8, c_im);

		cl::Buffer data_buf(context, CL_MEM_WRITE_ONLY, w*h*3);
		kernel.setArg(0, data_buf);

	// Run kernel
		queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(w, h), cl::NullRange);

	// Wait for kernel to finish and fetch it's work
		unsigned char* data = new unsigned char[w*h*3];
		queue.enqueueReadBuffer(data_buf, CL_TRUE, 0, w*h*3, data);

	// Write it down
		FILE* f = fopen((filename+".ppm").c_str(), "wb");
		fprintf(f, "P6 %d %d 255\n", w, h);
		fwrite(data, w*h*3, 1, f);

		fclose(f);
		delete data;

	// Add frame-tick label (now files are .png)
		std::stringstream cmd;
		cmd << "convert " << filename << ".ppm -gravity SouthEast -stroke none -fill lime -annotate 0 " << frame << " " << filename << ".png";
		system(cmd.str().c_str());
	}


	cout << "Converting into gif...";
	// system("convert -delay 5 _tmp/*.ppm julia_simple.gif");
	system("convert -delay 5 _tmp/*.png julia_simple.gif");
	cout << " done!\n";

	cout << "Reconverting into cyclic gif...";
	system("convert julia_simple.gif -coalesce ( -clone -2-1 ) -quiet -layers OptimizePlus -loop 0 julia_opt.gif");
	cout << " done!\n";

	cout << "Cleaning up...";
	if (clean_post) {
		system("rm _tmp/*.ppm");
		system("rm _tmp/*.png");
		system("rmdir _tmp");
		cout << " done!\n";
	} else {
		cout << " skipped.\n";
	}
}
