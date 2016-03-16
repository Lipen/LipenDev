#ifndef MAIN_HPP
#define MAIN_HPP

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <chrono>
#include <cmath>
#include <vector>
#include <map>
#include <iterator>
#include "mingw.thread.h"
#include "mingw.mutex.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std::chrono;
using std::cout;
using std::endl;
using std::string;
using clock_used = high_resolution_clock;  // steady_clock

extern const double SCREEN_WIDTH;
extern const double SCREEN_HEIGHT;

extern const double MAP_WIDTH;
extern const double MAP_HEIGHT;
extern const double MAP_EDGE_RIGHT;
extern const double MAP_EDGE_LEFT;
extern const double MAP_EDGE_TOP;
extern const double MAP_EDGE_BOT;

extern const double BALL_MASS;
extern const double BALL_RADIUS;
extern const double BALL_FRICTION;
extern const double BALL_MAXSPEED;

extern const double ROBOT_MASS;
extern const double ROBOT_RADIUS;
extern const double ROBOT_BASE;
extern const double ROBOT_BASE_SPEED;
extern const double ROBOT_BASE_ANGULAR_SPEED;
extern const double ROBOT_THRESHOLD_MIN;
extern const double ROBOT_THRESHOLD_MAX;
extern const double ROBOT_THRESHOLD_SLOPE;
extern const double ROBOT_THRESHOLD_INTER;

extern const double GATE_LEFT_X;
extern const double GATE_RIGHT_X;
extern const double GATE_LEFT_TOP;
extern const double GATE_LEFT_BOT;
extern const double GATE_RIGHT_TOP;
extern const double GATE_RIGHT_BOT;

extern volatile bool RUNNING;
extern const int DT_MODELLER;
extern const int DT_DRAWER;
extern const int DT_STATEGIER;
extern const int DT_LOADER;
extern const int DT_SAVER;

extern const double PI;
extern const double TWO_PI;
extern const double HALF_PI;

extern const clock_used::time_point time_start;
extern std::mutex mutex_control;
extern std::mutex mutex_data;

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;

class Robot;
class Ball;
class LTexture;

extern std::map<int, Robot> robots;
extern Ball ball;
extern LTexture gBallTexture;
extern LTexture gRobotTexture;


template<typename T, typename U>
double get_dist_squared(const T &a, const U &b) {
	double ax = a.x, ay = a.y, bx = b.x, by = b.y;
	double dx = ax - bx, dy = ay - by;
	return dx*dx + dy*dy;
}
double get_dist_squared(double x1, double y1, double x2, double y2);

template<typename T, typename U>
inline double get_dist(const T &a, const U &b) {
	return sqrt(get_dist_squared(a, b));
}
double get_dist(double x1, double y1, double x2, double y2);

double get_dist_to_line(double x, double y, double x1, double y1, double x2, double y2);

int map2scrX(double x);
int map2scrY(double y);

double scr2mapX(int x);
double scr2mapY(int y);

int random(int a, int b);

double logistic_linear(double x, double intersect_left, double threshold_right, double threshold_left = 0.0);
double logistic_sigmoid(double x, double slope, double shift = 0.0);

void draw_circle(double x, double y, double r);
void normalize_angle(double &angle, double center = 0.0);
double normalized_angle(double angle, double center = 0.0);
void calc_gradient_at(double x, double y, double x1, double y1, double* Fx, double* Fy, double* U);

template<typename T>
int sign(T x) {
	return (x > 0) ? 1 : (x < 0) ? -1 : 0;
}
template<typename T>
int sign_(T x) {
	return (x < 0) ? -1 : 1;
}

template<typename T, typename U>
T constrain(const T& x, U&& a, U&& b) {
	return (x > a) ? a : (x < b) ? b : x;
}

#endif // MAIN_HPP
