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

#define MAP_WIDTH 3000
#define MAP_HEIGHT 2250
#define DT_MODELLER 10
#define DT_DRAWER 40
#define DT_STATEGIER 50
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAP_EDGE_RIGHT 1400  // ~ MAP_WIDTH/2 minus something
#define MAP_EDGE_LEFT (-MAP_EDGE_RIGHT)
#define MAP_EDGE_TOP 1050  // ~ MAP_HEIGHT/2 minus something
#define MAP_EDGE_BOT (-MAP_EDGE_TOP)
#define RADIUS_ROBOT 68
#define RADIUS_BALL 38

const double PI = 3.1415926;
const double TWO_PI = 6.2831853;
const auto time_start = steady_clock::now();
volatile bool RUNNING = true;
std::mutex mutex_control;
std::mutex mutex_data;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
class LTexture;
extern LTexture gBallTexture;
extern LTexture gRobotTexture;


template<typename T, typename U>
double get_dist_squared(T &a, U &b) {
	double ax = a.x, ay = a.y, bx = b.x, by = b.y;
	double dx = ax - bx, dy = ay - by;
	return dx*dx + dy*dy;
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


class Ball {
 public:
	double x, y, vx, vy, ax, ay;
	double radius = RADIUS_BALL;

	Ball(double x, double y, double vx, double vy, double ax, double ay)
	: x(x), y(y), vx(vx), vy(vy), ax(ax), ay(ay)
	{}

	void update(double dt) {
		vx += ax * dt;
		vy += ay * dt;
		vx *= 0.999;  // Friction
		vy *= 0.999;
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
		gBallTexture.render(
			(x + MAP_WIDTH/2) * SCREEN_WIDTH / MAP_WIDTH,
			(-y + MAP_HEIGHT/2) * SCREEN_HEIGHT / MAP_HEIGHT );
	}

	friend std::ostream& operator<< (std::ostream &o, const Ball &b) {
		return o << "[Ball: x = " << std::fixed << std::setprecision(1) << b.x << ", y = " << b.y << "; v = " << std::setprecision(2) << b.vx << " / " << b.vy << "; a = " << b.ax << " / " << b.ay << "]";
	}
};


class Robot {
 public:
	double x, y, angle;
	double u_left = 0, u_right = 0;
	double radius = RADIUS_ROBOT;

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
		double base = 100;

		double ul = u_left / 100.;
		double ur = u_right / 100.;
		double v = (ul + ur) / 2. * base_speed;
		double w = (ur - ul) * base_speed / base * base_ang_speed;

		x += v * dt * cos(angle);
		y += v * dt * sin(angle);

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
		// Normalize angle:
		// angle = angle - TWO_PI * floor( (angle + PI - center) / TWO_PI )
		angle -= TWO_PI * floor((angle + PI)/TWO_PI);
	}

	void render() {
		gRobotTexture.render(
			(x + MAP_WIDTH/2) * SCREEN_WIDTH / MAP_WIDTH,
			(-y + MAP_HEIGHT/2) * SCREEN_HEIGHT / MAP_HEIGHT,
			NULL,
			angle * -180. / PI + 90);
	}

	void collide(Ball* &ball) {
		double ds = get_dist_squared(*this, *ball);  // DistanceSquared
		double rs = radius + ball->radius;  // RaduisesSum

		if (ds < rs*rs) {
			cout << "COLLISION WITH BALL" << endl;
		}
	}

	void collide(Robot &other) {
		double ds = get_dist_squared(*this, other);
		double rs = radius + other.radius;

		if (ds < rs*rs) {
			cout << "ROBOTS COLLISION" << endl;
		}
	}

	friend std::ostream& operator<< (std::ostream &o, const Robot &r) {
		return o << "[Robot: x=" << std::fixed << std::setprecision(1) << r.x << ", y=" << r.y << ", ang=" << std::setprecision(3) << r.angle << "]";
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

void detect_collisions(std::vector<Robot> &robots, Ball* &ball) {
	for (size_t i = 0; i < robots.size(); ++i) {
		robots[i].collide(ball);
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
	auto time_modeller = steady_clock::now();

	while (RUNNING) {
		double dt = duration<double, std::milli>(steady_clock::now() - time_modeller).count() / 1000.;
		time_modeller = steady_clock::now();
		// cout << "Modeller :: dt = " << std::fixed << std::setprecision(1) << dt*1000 << " ms" << endl;

		std::vector<Robot> robots;
		Ball* ball = nullptr;
		read_data("data", robots, ball);
		read_control("control", robots);

		for (auto&& r : robots)
			r.apply_u(dt);
		ball->update(dt);

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

		// std::vector<std::vector<char>> m(MAP_HEIGHT_PIXEL, std::vector<char>(MAP_WIDTH_PIXEL, '-'));

		// for (auto&& r : robots) {
		// 	int x = (r.x + MAP_WIDTH/2) * MAP_WIDTH_PIXEL / MAP_WIDTH;
		// 	int y = (-r.y + MAP_HEIGHT/2) * MAP_HEIGHT_PIXEL / MAP_HEIGHT;
		// 	if (x < MAP_WIDTH_PIXEL && x >= 0 && y < MAP_HEIGHT_PIXEL && y >= 0)
		// 		m[y][x] = get_symbol_from_angle(r.angle);
		// }

		// /* Ball */ {
		// 	int x = (ball->x + MAP_WIDTH/2) * MAP_WIDTH_PIXEL / MAP_WIDTH;
		// 	int y = (-ball->y + MAP_HEIGHT/2) * MAP_HEIGHT_PIXEL / MAP_HEIGHT;
		// 	if (x < MAP_WIDTH_PIXEL && x >= 0 && y < MAP_HEIGHT_PIXEL && y >= 0)
		// 		m[y][x] = 'o';
		// }

		// std::stringstream ss;
		// for (auto&& line : m) {
		// 	for (char c : line)
		// 		ss << c;
		// 	ss << '\n';
		// }
		// system("cls");
		// cout << ss.str();

		/* DEBUG */
		// for (auto&& r : robots)
		// 	cout << r << endl;
		// cout << *ball << " :: " << ball << endl;
		/* DEBUG END */

	// =====================================================================
		// // Clear screen
		// SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		// SDL_RenderClear(gRenderer);

		// // Render red filled quad
		// SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
		// SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
		// SDL_RenderFillRect(gRenderer, &fillRect);

		// // Render green outlined quad
		// SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
		// SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
		// SDL_RenderDrawRect(gRenderer, &outlineRect);

		// // Draw blue horizontal line
		// SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF );
		// SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

		// // Draw vertical line of yellow dots
		// SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
		// for (int i = 0; i < SCREEN_HEIGHT; i += 4) {
		// 	SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH / 2, i);
		// }

		// // Update screen
		// SDL_RenderPresent(gRenderer);
	// =====================================================================

	// =====================================================================
		// Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xF0, 0xFF);
		SDL_RenderClear(gRenderer);

		// Render green map edges
		SDL_Rect map_edges = {
			(MAP_EDGE_LEFT + MAP_WIDTH/2) * SCREEN_WIDTH / MAP_WIDTH,
			(MAP_EDGE_TOP + MAP_HEIGHT/2) * SCREEN_HEIGHT / MAP_HEIGHT,
			(MAP_EDGE_RIGHT - MAP_EDGE_LEFT) * SCREEN_WIDTH / MAP_WIDTH,
			(MAP_EDGE_BOT - MAP_EDGE_TOP) * SCREEN_HEIGHT / MAP_HEIGHT
		};
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0xE0, 0x00, 0xFF);
		SDL_RenderDrawRect(gRenderer, &map_edges);

		ball->render();

		for (auto&& r : robots) {
			r.render();
		}

		// Update screen
		SDL_RenderPresent(gRenderer);
	// =====================================================================

		// cout << "Drawer :: " << duration_cast<milliseconds>(steady_clock::now() - time_start).count()/1000. << " s" << endl;
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
			r.set_u(100, 50);
		}

		write_control("control", robots);

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