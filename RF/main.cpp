/* Copyright (c) 2016, Lipen */
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <exception>
#include "mingw.thread.h"
#include "mingw.mutex.h"

using std::cout;
using std::endl;
using std::string;
using namespace std::chrono;

#define MAP_WIDTH 5000
#define MAP_HEIGHT 1000
#define MAP_WIDTH_PIXEL 100
#define MAP_HEIGHT_PIXEL 20
#define DT_MODELLER 100
#define DT_DRAWER 500
#define DT_STATEGIER 50

const double PI = 3.1415926;
const double TWO_PI = 6.2831853;
const auto time_start = steady_clock::now();
std::mutex mutex_control;
std::mutex mutex_data;


class Robot {
 public:
	double x, y, angle;
	double u_left = 0, u_right = 0;

	Robot(double x, double y, double angle)
	: x(x), y(y), angle(angle)
	{}

	void set_u(double ul, double ur) {
		u_left = ul;
		u_right = ur;
	}

	void apply_u(double dt) {
		double base_speed = 100;
		double base_ang_speed = 0.5;
		double base = 50;

		double ul = u_left / 100.;
		double ur = u_right / 100.;
		double v = (ul + ur) / 2. * base_speed;
		double w = (ur - ul) * base_speed / base * base_ang_speed;

		x += v * dt * cos(angle);
		y += v * dt * sin(angle);
		angle += w * dt;

		// Normalize angle:
		// angle = angle - TWO_PI * floor( (angle + PI - center) / TWO_PI )
		angle -= TWO_PI * floor((angle + PI)/TWO_PI);
	}

	friend std::ostream& operator<< (std::ostream &o, const Robot &r) {
		return o << "[Robot: x=" << std::fixed << std::setprecision(1) << r.x << ", y=" << r.y << ", ang=" << std::setprecision(3) << r.angle << "]";
	}
};


class Ball {
 public:
	double x, y, vx, vy, ax, ay;

	explicit Ball(double x, double y, double vx, double vy, double ax, double ay)
	: x(x), y(y), vx(vx), vy(vy), ax(ax), ay(ay)
	{}

	void update(double dt) {
		vx += ax * dt;
		vy += ay * dt;
		vx *= 0.995;  // Friction
		vy *= 0.995;
		x += vx * dt;
		y += vy * dt;
	}

	friend std::ostream& operator<< (std::ostream &o, const Ball &b) {
		return o << "[Ball: x = " << std::fixed << std::setprecision(1) << b.x << ", y = " << b.y << "; v = " << std::setprecision(2) << b.vx << " / " << b.vy << "; a = " << b.ax << " / " << b.ay << "]";
	}
};


void read_data(string filename, std::vector<Robot> &robots, Ball* &ball) {
	mutex_data.lock();
	std::ifstream fi(filename);

	int n;
	fi >> n;

	double x, y, vx, vy, ax, ay;  // Ball`s parameters
	fi >> x >> y >> vx >> vy >> ax >> ay;
	ball = new Ball(x, y, vx, vy, ax, ay);

	for (int i = 0; i < n; ++i) {
		double x, y, angle;
		fi >> x >> y >> angle;
		robots.emplace_back(x, y, angle);
	}

	fi.close();
	mutex_data.unlock();
}

void read_control(string filename, std::vector<Robot> &robots) {
	mutex_control.lock();
	std::ifstream fi(filename);

	for (auto&& r : robots) {
		double ul, ur;
		fi >> ul >> ur;
		r.set_u(ul, ur);
	}

	fi.close();
	mutex_control.unlock();
}

void write_data(string filename, std::vector<Robot> &robots, Ball* &ball) {
	mutex_data.lock();
	std::ofstream fo(filename);

	fo << robots.size() << '\n';
	fo << ball->x << ' ' << ball->y << ' ' << ball->vx << ' ' << ball->vy << ' ' << ball->ax << ' ' << ball->ay << '\n';

	for (auto&& r : robots) {
		fo << r.x << ' ' << r.y << ' ' << r.angle << '\n';
	}

	fo.close();
	mutex_data.unlock();
}

void write_control(string filename, std::vector<Robot> &robots) {
	mutex_control.lock();
	std::ofstream fo(filename);

	for (auto&& r: robots) {
		fo << r.u_left << ' ' << r.u_right << '\n';
	}

	fo.close();
	mutex_control.unlock();
}

char get_symbol_from_angle(double angle) {
	if (angle <= PI/4 && angle >= -PI/4)
		return '>';
	if (angle >= PI/4 && angle <= 3*PI/4)
		return '^';
	if (angle <= -PI/4 && angle >= -3*PI/4)
		return 'v';
	if ((angle >= 3*PI/4 && angle <= PI) || (angle <= -3*PI/4 && angle >= -PI))
		return '<';

	return '?';
}

void modeller() {
	auto time_modeller = steady_clock::now();

	while (true) {
		double dt = duration<double, std::milli>(steady_clock::now() - time_modeller).count() / 1000.;
		time_modeller = steady_clock::now();
		cout << "Modeller :: dt = " << std::fixed << std::setprecision(1) << dt*1000 << " ms" << endl;

		std::vector<Robot> robots;
		Ball* ball = nullptr;
		read_data("data", robots, ball);
		read_control("control", robots);

		for (auto&& r : robots)
			r.apply_u(dt);
		ball->update(dt);

		write_data("data", robots, ball);

		std::this_thread::sleep_for(duration<double, std::milli>(DT_MODELLER));
	}
}

void drawer() {
	while (true) {
		std::vector<Robot> robots;
		Ball* ball = nullptr;
		read_data("data", robots, ball);

		std::vector<std::vector<char>> m(MAP_HEIGHT_PIXEL, std::vector<char>(MAP_WIDTH_PIXEL, '-'));

		for (auto&& r : robots) {
			int x = (r.x + MAP_WIDTH/2) * MAP_WIDTH_PIXEL / MAP_WIDTH;
			int y = (-r.y + MAP_HEIGHT/2) * MAP_HEIGHT_PIXEL / MAP_HEIGHT;
			if (x < MAP_WIDTH_PIXEL && x >= 0 && y < MAP_HEIGHT_PIXEL && y >= 0)
				m[y][x] = get_symbol_from_angle(r.angle);
		}

		/* Ball */ {
			int x = (ball->x + MAP_WIDTH/2) * MAP_WIDTH_PIXEL / MAP_WIDTH;
			int y = (-ball->y + MAP_HEIGHT/2) * MAP_HEIGHT_PIXEL / MAP_HEIGHT;
			if (x < MAP_WIDTH_PIXEL && x >= 0 && y < MAP_HEIGHT_PIXEL && y >= 0)
				m[y][x] = 'o';
		}

		std::stringstream ss;
		for (auto&& line : m) {
			for (char c : line)
				ss << c;
			ss << '\n';
		}
		system("cls");
		cout << ss.str();

	/* DEBUG */
	for (auto&& r : robots)
		cout << r << endl;
	cout << *ball << " :: " << ball << endl;
	/* DEBUG END */

		cout << "Drawer :: " << duration_cast<milliseconds>(steady_clock::now() - time_start).count()/1000. << " s" << endl;
		std::this_thread::sleep_for(milliseconds(DT_DRAWER));
	}
}


void strategier() {
	while (true) {
		std::vector<Robot> robots;
		Ball* ball = nullptr;
		read_data("data", robots, ball);

		// TODO: Strategy
		for (auto&& r : robots) {
			r.set_u(100, 50);
		}

		write_control("control", robots);

		std::this_thread::sleep_for(milliseconds(DT_STATEGIER));
	}
}


int main() {
	std::thread th_strata(strategier);
	std::thread th_draw(drawer);
	std::thread th_model(modeller);

	cout << "Waiting for threads..." << endl;
	th_strata.join();
	th_draw.join();
	th_model.join();

	cout << "Main end" << endl;
}
