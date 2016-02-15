/* Copyright (c) 2016, Lipen */
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <cmath>
#include "mingw.thread.h"
#include "mingw.mutex.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std::chrono;
using std::cout;
using std::endl;
using std::string;
using clock_used = high_resolution_clock;  // steady_clock

const double SCREEN_WIDTH = 800;
const double SCREEN_HEIGHT = 600;
const double MAP_WIDTH = 3000;
const double MAP_HEIGHT = 2250;
const double MAP_EDGE_RIGHT = 1400;  // ~ MAP_WIDTH/2 minus something
const double MAP_EDGE_LEFT = -MAP_EDGE_RIGHT;
const double MAP_EDGE_TOP = 1050;  // ~ MAP_HEIGHT/2 minus something
const double MAP_EDGE_BOT = -MAP_EDGE_TOP;

const double BALL_MASS = 10;
const double BALL_RADIUS = 30;
const double BALL_FRICTION = 0.999;

const double ROBOT_MASS = 1000;
const double ROBOT_RADIUS = 70;
const double ROBOT_BASE = 50;
const double ROBOT_BASE_SPEED = 800;
const double ROBOT_BASE_ANGULAR_SPEED = 0.1;
const double ROBOT_THRESHOLD_MIN = 100;
const double ROBOT_THRESHOLD_MAX = 700;
const double ROBOT_THRESHOLD_SLOPE = 1./(ROBOT_THRESHOLD_MAX-ROBOT_THRESHOLD_MIN);
const double ROBOT_THRESHOLD_INTER = ROBOT_THRESHOLD_SLOPE * ROBOT_THRESHOLD_MIN;

volatile bool RUNNING = true;
const double PID_P = 100;
const int DT_MODELLER = 10;   // 10
const int DT_DRAWER = 40;     // 40
const int DT_STATEGIER = 50;  // 50

const double PI = 3.14159265358979323846;
const double TWO_PI = 6.28318530717958647693;
const double HALF_PI = 1.57079632679489661923;

const auto time_start = clock_used::now();
std::mutex mutex_control;
std::mutex mutex_data;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

class LTexture;
extern LTexture gBallTexture;
extern LTexture gRobotTexture;


template<typename T, typename U>
double get_dist_squared(const T &a, const U &b) {
	double ax = a.x, ay = a.y, bx = b.x, by = b.y;
	double dx = ax - bx, dy = ay - by;
	return dx*dx + dy*dy;
}

double get_dist_squared(double x1, double y1, double x2, double y2) {
	double dx = x1 - x2;
	double dy = y1 - y2;
	return dx*dx + dy*dy;
}

template<typename T, typename U>
double get_dist(const T &a, const U &b) {
	return sqrt(get_dist_squared(a, b));
}

double get_dist(double x1, double y1, double x2, double y2) {
	double dx = x1 - x2;
	double dy = y1 - y2;
	return sqrt(dx*dx + dy*dy);
}

void draw_circle(double x, double y, double r) {
	int n = 40;  // 30
	SDL_Point* points = new SDL_Point[n+1];

	for (int i = 0; i <= n; ++i) {
		int a = x + r * cos(i * TWO_PI / n);  // i * 360/n * PI / 180
		int b = y + r * sin(i * TWO_PI / n);
		points[i] = { a, b };
	}

	SDL_RenderDrawLines(gRenderer, points, n);
	// SDL_RenderDrawPoints(gRenderer, points, n);

	delete points;
}

void normalize_angle(double &angle, double center = 0.0) {
	angle -= TWO_PI * floor( (angle + PI - center) / TWO_PI );
}

template<typename T>
int sign(T x) {
	if (x > 0)
		return 1;
	else if (x < 0)
		return -1;
	return 0;
}


class LTexture {
	SDL_Texture* texture;
	int width, height;

 public:
	LTexture(): texture(NULL), width(0), height(0) {}

	~LTexture() {
		free();
	}

	void free() {
		// Free texture if it exists
		if (texture != NULL) {
			SDL_DestroyTexture(texture);
			texture = NULL;
			width = 0;
			height = 0;
		}
	}

	bool load_from_file(string path) {
		// Get rid of preexisting texture
		free();

		// The final texture
		SDL_Texture* new_texture = NULL;

		// Load image at specified path
		SDL_Surface* loaded_surface = IMG_Load(path.c_str());
		if (loaded_surface == NULL) {
			cout << "Bad load image(\"" << path << "\"): " << IMG_GetError() << endl;
		}
		else {
			// Color key image
			// SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0xFF, 0, 0xFF));

			// Create texture from surface pixels
	        new_texture = SDL_CreateTextureFromSurface(gRenderer, loaded_surface);
			if (new_texture == NULL) {
				cout << "Bad texture create (\"" << path << "\"): " << SDL_GetError() << endl;
			}
			else {
				// Get image dimensions
				width = loaded_surface->w;
				height = loaded_surface->h;
			}

			// Get rid of old loaded surface
			SDL_FreeSurface(loaded_surface);
		}

		// Return success
		texture = new_texture;
		return texture != NULL;
	}

	void setColor(Uint8 red, Uint8 green, Uint8 blue) {
		// Modulate texture rgb
		SDL_SetTextureColorMod(texture, red, green, blue);
	}

	void setBlendMode(SDL_BlendMode blending) {
		// Set blending function
		SDL_SetTextureBlendMode(texture, blending);
	}

	void setAlpha(Uint8 alpha) {
		// Modulate texture alpha
		SDL_SetTextureAlphaMod(texture, alpha);
	}

	int getWidth() {
		return width;
	}

	int getHeight() {
		return height;
	}

	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE) {
		// cout << "Render at (" << x << ", " << y << ") [RAW COORDS]" << endl;

		// Set rendering space and render to screen
		SDL_Rect render_quad = { x-width/2, y-width/2, width, height };

		// Set clip rendering dimensions
		if (clip != NULL) {
			render_quad.w = clip->w;
			render_quad.h = clip->h;
		}

		// Render to screen
		SDL_RenderCopyEx(gRenderer, texture, clip, &render_quad, angle, center, flip);
	}
};

LTexture gBallTexture;
LTexture gRobotTexture;


class Robot {
 public:
	volatile double x, y;
	double angle;
	volatile double u_left = 0, u_right = 0;
	volatile double vx = 0, vy = 0;
	volatile double radius = ROBOT_RADIUS;

	volatile double __a = 0, __b = 0, __r = 0;

	Robot(double x, double y, double angle)
	: x(x), y(y), angle(angle)
	{}

	void set_u(double ul, double ur) {
		u_left = (ul > 100) ? 100 : (ul < -100) ? -100 : ul;
		u_right = (ur > 100) ? 100 : (ul < -100) ? -100 : ur;
	}

	void apply_u(double dt) {
		double base_speed = ROBOT_BASE_SPEED;
		double base_ang_speed = ROBOT_BASE_ANGULAR_SPEED;
		double base = ROBOT_BASE;

		double ul = u_left / 100.;
		double ur = u_right / 100.;
		double v = (ul + ur) / 2. * base_speed;
		double w = (ur - ul) * base_speed / base * base_ang_speed;

		vx = v * dt * cos(angle);
		vy = v * dt * sin(angle);

		x += vx;
		y += vy;

		if (x > MAP_EDGE_RIGHT) {
			x = 2*MAP_EDGE_RIGHT - x;
		}
		if (x < MAP_EDGE_LEFT) {
			x = 2*MAP_EDGE_LEFT - x;
		}
		if (y > MAP_EDGE_TOP) {
			y = 2*MAP_EDGE_TOP - y;
		}
		if (y < MAP_EDGE_BOT) {
			y = 2*MAP_EDGE_BOT - y;
		}

		angle += w * dt;
		normalize_angle(angle);
	}

	void render() {
		double x_ = (x + MAP_WIDTH/2) * SCREEN_WIDTH / MAP_WIDTH;
		double y_ = (-y + MAP_HEIGHT/2) * SCREEN_HEIGHT / MAP_HEIGHT;

		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
		draw_circle(x_, y_, radius * SCREEN_WIDTH / MAP_WIDTH);

		/* THAT TRICKY CIRCLES */
		double a_ = (__a + MAP_WIDTH/2) * SCREEN_WIDTH / MAP_WIDTH;
		double b_ = (-__b + MAP_HEIGHT/2) * SCREEN_HEIGHT / MAP_HEIGHT;
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0xE0, 0xFF);
		draw_circle(a_, b_, __r * SCREEN_WIDTH / MAP_WIDTH);
		/**/

		gRobotTexture.render(x_, y_, NULL, angle * -180. / PI + 90);
	}

	void collide(Robot &other) {
		double ds = get_dist_squared(*this, other);
		double rs = radius + other.radius;

		if (ds <= 1e-9) {
			cout << "HMMMM ... " << *this << " (" << this << ") AND " << other << "(" << &other << ")" << endl;
		}

		if (ds < rs*rs) {
			double semi = sqrt(ds) / 2;
			// double semi = (ds - other.radius*other.radius + radius*radius) / (2 * sqrt(ds) + 0.00001);  // Distance from this to radical line
			double alpha = atan2(other.y - y, other.x - x);  // Between this and other robot

			double dr_this = radius - semi;  // Distance to shift away from collision semipoint
			x -= dr_this * cos(alpha);
			y -= dr_this * sin(alpha);

			double dr_other = other.radius - (sqrt(ds) - semi);
			other.x += dr_other * cos(alpha);
			other.y += dr_other * sin(alpha);

			if (dr_this <= 0.0 || dr_other <= 0 || semi <= 1e-9) {
				cout << "COLLISION :: Shifted this for " << dr_this << ", other for " << dr_other << "\tSEMI = " << semi << ", dist = " << sqrt(ds) << ", ds = " << ds << ", alpha = " << alpha << endl;
				cout << "THIS: " << *this << endl;
				cout << "OTHER: " << other << endl;
			}
		}
	}

	void apply_strategy_attack(double x1, double y1 /*Ball* &ball*/) {
		// double x1 = ball->get_x(), y1 = ball->get_y();
		double x2 = x, y2 = y;
		double k = (0 - y1) / (MAP_EDGE_LEFT - x1);  // Slope of ball direction

		/* (a, b) is a trajectory center */
		double a = ( x1*x1*-k + 2*x1*y1 - 2*x1*y2 + y1*y1*k - 2*y1*y2*k + x2*x2*k + y2*y2*k ) / ( 2 * (-x1*k + y1 + x2*k - y2) );
		double b = (x1 - a) / k + y1;

		double chi = atan2(b-y2, a-x2);  // Between robot and circle center
		double tg = chi + sign((x2-x1)*(0-y1) - (y2-y1)*(MAP_EDGE_LEFT-x1)) * HALF_PI;  // Transfer to proper tangent
		normalize_angle(tg);  // ?
		double theta = atan2(y1 - y2, x1 - x2);  // Between robot and ball
		double d = get_dist(x2, y2, x1, y1);     // Between robot and ball
		double da = theta - tg;
		double gamma = (d > ROBOT_THRESHOLD_MAX) ? 1 : (d < ROBOT_THRESHOLD_MIN) ? 0 : ROBOT_THRESHOLD_SLOPE * d + ROBOT_THRESHOLD_INTER;

		double alpha = tg + da * gamma - angle;  // Delta alpha for p-regulator
		normalize_angle(alpha);

		// cout << "k = " << k << ", a = " << a << ", b=" << b << endl;
		__a = a;
		__b = b;
		__r = sqrt((x2-a)*(x2-a) + (y2-b)*(y2-b));

		double base_u = 70;
		double ul = base_u - PID_P * alpha;
		double ur = base_u + PID_P * alpha;

		// Do not forget to set u
		set_u(ul, ur);
	}

	friend std::ostream& operator<< (std::ostream &o, const Robot &r) {
		return o << "[Robot: x=" << std::fixed << std::setprecision(1) << r.x << ", y=" << r.y << ", ang=" << std::setprecision(3) << r.angle << ", r=" << r.radius << "]";
	}
};


class Ball {
 public:
	volatile double x, y, vx, vy, ax, ay;
	volatile double radius = BALL_RADIUS;

	Ball(double x, double y, double vx, double vy, double ax, double ay)
	: x(x), y(y), vx(vx), vy(vy), ax(ax), ay(ay)
	{}

	double get_x() {
		return x;
	}
	double get_y() {
		return y;
	}

	void update(double dt) {
		vx += ax * dt;
		vy += ay * dt;
		vx *= BALL_FRICTION;  // Friction
		vy *= BALL_FRICTION;
		x += vx * dt;
		y += vy * dt;

		if (x > MAP_EDGE_RIGHT) {
			x = 2*MAP_EDGE_RIGHT - x;
			vx *= -1;
		}
		if (x < MAP_EDGE_LEFT) {
			x = 2*MAP_EDGE_LEFT - x;
			vx *= -1;
		}
		if (y > MAP_EDGE_TOP) {
			y = 2*MAP_EDGE_TOP - y;
			vy *= -1;
		}
		if (y < MAP_EDGE_BOT) {
			y = 2*MAP_EDGE_BOT - y;
			vy *= -1;
		}
	}

	void render() {
		double a = (x + MAP_WIDTH/2) * SCREEN_WIDTH / MAP_WIDTH;
		double b = (-y + MAP_HEIGHT/2) * SCREEN_HEIGHT / MAP_HEIGHT;

		gBallTexture.render(a, b);
	}

	void collide(const Robot &other) {
		double ds = get_dist_squared(*this, other);  // DistanceSquared
		double rs = radius + other.radius;  // RaduisesSum

		if (ds < rs*rs) {
			// cout << "COLLISION WITH BALL" << endl;
			double dx = other.x - x;
			double dy = other.y - y;
			double alpha = atan2(dy, dx);  // Between ball and robot
			double beta = atan2(vy, vx);   // Angle of ball`s velocity
			double da = alpha - beta;

			double vnx = vx * cos(da);  // Norm
			double vny = vy * cos(da);
			double vtx = vx * sin(da);  // Tangent
			double vty = vy * sin(da);

			/* u2 = (2m1v1 + v2(m2-m1)) / (m1+m2) */  // Norm
			double ux = (2*ROBOT_MASS*other.vx + vnx*(BALL_MASS - ROBOT_MASS)) / (ROBOT_MASS + BALL_MASS);
			double uy = (2*ROBOT_MASS*other.vy + vny*(BALL_MASS - ROBOT_MASS)) / (ROBOT_MASS + BALL_MASS);
			vx = ux + vtx;
			vy = uy + vty;

			double rho;
			if (ds < radius*radius)
				rho = 2*other.radius - sqrt(ds);
			else
				rho = rs;
			// cout << "RS = " << rs << ", robot.R = " << other.radius << ", dist = " << sqrt(ds) << ", rho = " << rho << endl;
			x = other.x + rho * cos(alpha + PI);
			y = other.y + rho * sin(alpha + PI);
		}
	}

	friend std::ostream& operator<< (std::ostream &o, const Ball &b) {
		return o << "[Ball: x = " << std::fixed << std::setprecision(1) << b.x << ", y = " << b.y << "; v = " << std::setprecision(2) << b.vx << " / " << b.vy << "; a = " << b.ax << " / " << b.ay << "]";
	}
};


void read_data(string filename, std::vector<Robot> &robots, Ball* &ball) {
	std::lock_guard<std::mutex> locker(mutex_data);
	std::ifstream fi(filename);

	int n;
	fi >> n;

	double x, y, vx, vy, ax, ay;  // Ball`s parameters
	fi >> x >> y >> vx >> vy >> ax >> ay;
	ball = new Ball(x, y, vx, vy, ax, ay);

	for (int i = 0; i < n; ++i) {
		double x, y, angle, _a, _b, _r;
		fi >> x >> y >> angle/**/ >> _a >> _b >> _r;
		robots.emplace_back(x, y, angle);
		/* THAT TRICKY CIRCLES: */
		robots[i].__a = _a;
		robots[i].__b = _b;
		robots[i].__r = _r;
	}

	fi.close();
}

void read_control(string filename, std::vector<Robot> &robots) {
	std::lock_guard<std::mutex> locker(mutex_control);
	std::ifstream fi(filename);

	for (auto&& r : robots) {
		double ul, ur;
		fi >> ul >> ur;
		r.set_u(ul, ur);
	}

	fi.close();
}

void write_data(string filename, std::vector<Robot> &robots, Ball* &ball) {
	std::lock_guard<std::mutex> locker(mutex_data);
	std::ofstream fo(filename);

	fo << robots.size() << '\n';
	fo << ball->x << ' ' << ball->y << ' ' << ball->vx << ' ' << ball->vy << ' ' << ball->ax << ' ' << ball->ay << '\n';

	for (auto&& r : robots) {
		fo << r.x << ' ' << r.y << ' ' << r.angle /**/<<' '<<r.__a<<' '<<r.__b<<' '<<r.__r/**/ << '\n';
	}

	fo.close();
}

void write_control(string filename, std::vector<Robot> &robots) {
	std::lock_guard<std::mutex> locker(mutex_control);
	std::ofstream fo(filename);

	for (auto&& r: robots) {
		fo << r.u_left << ' ' << r.u_right << '\n';
	}

	fo.close();
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

void detect_collisions(std::vector<Robot> &robots, Ball* &ball) {
	for (size_t i = 0; i < robots.size(); ++i) {
		ball->collide(robots[i]);

		for (size_t j = i+1; j < robots.size(); ++j) {
			robots[i].collide(robots[j]);
		}
	}
}


int initialize_sdl() {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "Bad SDL init: " << SDL_GetError() << endl;
		success = false;
	}
	else {
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			cout << "Warning: Linear texture filtering not enabled!" << endl;
		}

		gWindow = SDL_CreateWindow("Drawer :: Map", 500, 80/*SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED*/, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			cout << "Bad SDL window create: " << SDL_GetError() << endl;
			success = false;
		}
		else {
			// Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE/*SDL_RENDERER_ACCELERATED*/);
			if (gRenderer == NULL) {
				cout << "Bad renderer create: " << SDL_GetError() << endl;
				success = false;
			}
			else {
				// Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				// Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					cout << "Bad sdl img init: " << SDL_GetError() << endl;
					success = false;
				}
			}
		}
	}

	if (!success) {
		cout << "Bad SDL init" << endl;
		return -1;
	}

	return 0;
}

void release_sdl() {
	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	gWindow = NULL;
	gRenderer = NULL;
	IMG_Quit();
	SDL_Quit();
}

int load_media() {
	if (!gBallTexture.load_from_file("ball.png")) {
		cout << "Bad texture load: " << SDL_GetError() << endl;
		return -1;
	}

	if (!gRobotTexture.load_from_file("robot.png")) {
		cout << "Bad texture load: " << SDL_GetError() << endl;
		return -1;
	}

	return 0;
}


void modeller() {
	auto time_modeller = clock_used::now();

	while (RUNNING) {
		double dt = duration<double, std::milli>(clock_used::now() - time_modeller).count() / 1000.;
		time_modeller = clock_used::now();
		// cout << "Modeller :: dt = " << std::fixed << std::setprecision(1) << dt*1000 << " ms" << endl;

		std::vector<Robot> robots;
		Ball* ball = nullptr;
		read_data("data", robots, ball);
		read_control("control", robots);

		for (auto&& r : robots)
			r.apply_u(dt);
		ball->update(dt);

		// Detect collisitions AFTER applying u
		detect_collisions(robots, ball);

		write_data("data", robots, ball);

		std::this_thread::sleep_for(duration<double, std::milli>(DT_MODELLER));
	}
}

void drawer() {
	while (RUNNING) {
		std::vector<Robot> robots;
		Ball* ball = nullptr;
		read_data("data", robots, ball);

		// Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xF0, 0xFF);
		SDL_RenderClear(gRenderer);

		// Render green map edges
		SDL_Rect map_edges = {
			(int)((MAP_EDGE_LEFT + MAP_WIDTH/2) * SCREEN_WIDTH / MAP_WIDTH),
			(int)((MAP_EDGE_TOP + MAP_HEIGHT/2) * SCREEN_HEIGHT / MAP_HEIGHT),
			(int)((MAP_EDGE_RIGHT - MAP_EDGE_LEFT) * SCREEN_WIDTH / MAP_WIDTH),
			(int)((MAP_EDGE_BOT - MAP_EDGE_TOP) * SCREEN_HEIGHT / MAP_HEIGHT)
		};
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0xC0, 0x00, 0xFF);
		SDL_RenderDrawRect(gRenderer, &map_edges);

		for (auto&& r : robots) {
			r.render();
		}

		ball->render();

		// Update screen
		SDL_RenderPresent(gRenderer);

		// cout << "Drawer :: " << duration_cast<milliseconds>(clock_used::now() - time_start).count()/1000. << " s" << endl;
		std::this_thread::sleep_for(milliseconds(DT_DRAWER));
	}
}

void strategier() {
	while (RUNNING) {
		std::vector<Robot> robots;
		Ball* ball = nullptr;
		read_data("data", robots, ball);

		// TODO: Strategy
		for (auto&& r : robots) {
			r.apply_strategy_attack(ball->x, ball->y);
		}

		write_control("control", robots);
		/* REMOVE?: */
		write_data("data", robots, ball);

		std::this_thread::sleep_for(milliseconds(DT_STATEGIER));
	}
}


int main(int argc, char* argv[]) {
	if (argc > 1) {
		cout << "Arguments:\n";
		for (int i = 1; i < argc; ++i) {
			cout << '\t' << string(argv[i]) << '\n';
		}
	}

	if (initialize_sdl())
		return -1;
	if (load_media())
		return -2;

	std::thread th_strata(strategier);
	std::thread th_draw(drawer);
	std::thread th_model(modeller);

	cout << "Main now reading events..." << endl;
	SDL_Event e;
	while (RUNNING) {
		SDL_WaitEvent(&e);

		if (e.type == SDL_QUIT) {
			cout << "Close button pressed" << endl;
			RUNNING = false;
		}
		else if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.sym == SDLK_ESCAPE) {
				cout << "ESC pressed" << endl;
				RUNNING = false;
			}
			else if (e.key.keysym.sym == SDLK_r) {
				cout << "Resetting" << endl;
				system("ini");
			}
		}
	}

	cout << "Waiting for threads to finish..." << endl;
	th_strata.join();
	th_draw.join();
	th_model.join();

	cout << "Releasing SDL" << endl;
	release_sdl();

	cout << "The end." << endl;
	return 0;
}
