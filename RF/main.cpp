/* Copyright (c) 2016, Lipen */
#include "main.hpp"
#include "Robot.hpp"
#include "Ball.hpp"
#include "LTexture.hpp"


const double SCREEN_WIDTH = 800;
const double SCREEN_HEIGHT = 600;

const double MAP_WIDTH = 3000;
const double MAP_HEIGHT = 2250;
const double MAP_EDGE_RIGHT = 1400;  // ~ MAP_WIDTH/2 minus something
const double MAP_EDGE_LEFT = -MAP_EDGE_RIGHT;
const double MAP_EDGE_TOP = 1050;  // ~ MAP_HEIGHT/2 minus something
const double MAP_EDGE_BOT = -MAP_EDGE_TOP;

const double BALL_MASS = 10;
const double BALL_RADIUS = 23;
const double BALL_FRICTION = 0.995;
const double BALL_MAXSPEED = 2000;

const double ROBOT_MASS = 2000;
const double ROBOT_RADIUS = 75;  // 81
const double ROBOT_BASE = 50;
const double ROBOT_BASE_SPEED = 800;
const double ROBOT_BASE_ANGULAR_SPEED = 0.15;
const double ROBOT_THRESHOLD_MIN = 100;		// 100
const double ROBOT_THRESHOLD_MAX = 1000;	// 700
const double ROBOT_THRESHOLD_SLOPE = 1./(ROBOT_THRESHOLD_MAX-ROBOT_THRESHOLD_MIN);
const double ROBOT_THRESHOLD_INTER = ROBOT_THRESHOLD_SLOPE * ROBOT_THRESHOLD_MIN;

const double GATE_LEFT_X = MAP_EDGE_LEFT + 70;
const double GATE_RIGHT_X = MAP_EDGE_RIGHT - 70;
const double GATE_LEFT_TOP = 300;
const double GATE_LEFT_BOT = -300;
const double GATE_RIGHT_TOP = 300;
const double GATE_RIGHT_BOT = -300;

volatile bool RUNNING = true;
const int DT_MODELLER = 10;  	// 10
const int DT_DRAWER = 40;  		// 40
const int DT_STRATEGIER = 50;  	// 50
const int DT_LOADER = 100;  	//
const int DT_SAVER = 100;  		//

const double PI = 3.14159265358979323846;
const double TWO_PI = 6.28318530717958647693;
const double HALF_PI = 1.57079632679489661923;

const clock_used::time_point time_start = clock_used::now();
std::mutex mutex_control;
std::mutex mutex_data;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

std::map<int, Robot> robots;
Ball ball;
LTexture gBallTexture;
LTexture gRobotTexture;


double get_dist_squared(double x1, double y1, double x2, double y2) {
	double dx = x1 - x2;
	double dy = y1 - y2;
	return dx*dx + dy*dy;
}

/*NO INLINE FFS*/double get_dist(double x1, double y1, double x2, double y2) {
	return sqrt(get_dist_squared(x1, y1, x2, y2));
}

double get_dist_to_line(double x, double y, double x1, double y1, double x2, double y2) {
	return std::abs((x1-x2)*(y2-y) - (x2-x)*(y1-y2)) / sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

int map2scrX(double x) {
	return (x + MAP_WIDTH/2) * SCREEN_WIDTH / MAP_WIDTH;
}
int map2scrY(double y) {
	return (-y + MAP_HEIGHT/2) * SCREEN_HEIGHT / MAP_HEIGHT;
}

double scr2mapX(int i) {
	return i * MAP_WIDTH / SCREEN_WIDTH - MAP_WIDTH/2;
}
double scr2mapY(int j) {
	return -j * MAP_HEIGHT / SCREEN_HEIGHT + MAP_HEIGHT/2;
}

int random(int a, int b) {
	return (rand() % (b+1-a)) + a;
}

double logistic_linear(double x, double intersect_left, double threshold_right, double threshold_left/* = 0.0*/) {
	if (x > threshold_right) {
		return 1;
	}
	else if (x < threshold_left) {
		return 0;
	}
	double slope = (1 - intersect_left) / (threshold_right - threshold_left);
	return slope*x + 1 - slope*threshold_right;
}

double logistic_sigmoid(double x, double slope, double shift/* = 0.0*/) {
	return tanh( (x - shift) / slope )/2+.5;
}

void draw_circle(double x, double y, double r) {
	int n = TWO_PI / (0.3 * MAP_WIDTH/SCREEN_WIDTH) * r;  // 0.3
	std::vector<SDL_Point> v;

	for (int i = 0; i <= n; ++i) {
		int a = x + r * cos(i * TWO_PI / n);
		int b = y + r * sin(i * TWO_PI / n);

		if (a >= MAP_EDGE_LEFT && a <= MAP_EDGE_RIGHT && b >= MAP_EDGE_BOT && b <= MAP_EDGE_TOP)  v.push_back({map2scrX(a), map2scrY(b)});
	}

	SDL_Point* points = new SDL_Point[v.size()];
	for (size_t i = 0; i < v.size(); ++i) {
		points[i] = v[i];
	}

	// SDL_RenderDrawLines(gRenderer, points, v.size());
	SDL_RenderDrawPoints(gRenderer, points, v.size());

	delete points;
}

void normalize_angle(double &angle, double center/* = 0.0*/) {
	angle -= TWO_PI * floor( (angle + PI - center) / TWO_PI );
}

double normalized_angle(double angle, double center/* = 0.0*/) {
	return angle - TWO_PI * floor( (angle + PI - center) / TWO_PI );
}

void calc_gradient_at(double x, double y, double x1, double y1, double* Fx_, double* Fy_, double* U_) {
	/* F = -grad(U) */
	// TODO: Move to 'CONSTANTS' block
	double KSI = 30;
	double NYA = 20000;

	double d = get_dist(x, y, x1, y1);

	double method;
	double U;
	if (d > 500) {
		// method = -500 * KSI / d;
		// U = 500 * KSI * d;
		method = -KSI / d;
		U = KSI * d;
	}
	else {
		method = -KSI;
		U = KSI/2 * d*d;
	}

	double Fx = method * (x - x1);
	double Fy = method * (y - y1);


	for (auto&& item : robots) {
		// Distance from robot to obstacle:
		double rho = get_dist(x, y, item.second.x, item.second.y);

		if (rho < 160 && rho > 1) {
			double method2 = NYA / rho;
			// double method2 = NYA * (1/rho - 1/100)/(rho*rho*rho);
			Fx += method2 * (x - item.second.x);
			Fy += method2 * (y - item.second.y);
			U += NYA/2 * rho;
			// U += NYA/2 * (1/rho - 1/100);
			// U += NYA/2 * (1/rho - 1/100)*(1/rho - 1/100);
		}
	}

	double F = sqrt(Fx*Fx + Fy*Fy);

	*Fx_ = Fx;
	*Fy_ = Fy;
	*U_ = U;
}


void read_data(const char* filename) {
	std::lock_guard<std::mutex> locker(mutex_data);
	FILE* f = fopen(filename, "r");

	int n;
	fscanf(f, "%d", &n);  // Robots amount

	fscanf(f, "%lf %lf", &ball.x, &ball.y);  // Ball`s parameters

	for (int i = 0; i < n; ++i) {
		int id;
		fscanf(f, "%d", &id);

		Robot& r = robots[id];
		fscanf(f, "%lf %lf %lf", &r.x, &r.y, &r.angle);
	}

	fclose(f);
}

void write_control(const char* filename) {
	std::lock_guard<std::mutex> locker(mutex_control);
	FILE* f = fopen(filename, "w");

	for (auto&& item : robots) {
		fprintf(f, "%d %lf %lf\n", item.first, item.second.u_left, item.second.u_right);
	}

	fclose(f);
}

void detect_collisions() {
	for (auto i = robots.begin(); i != robots.end(); ++i) {
		ball.collide(i->second);
		for (auto j = std::next(i); j != robots.end(); ++j) {
			i->second.collide(j->second);
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


void strategier() {
	while (RUNNING) {
		// for (auto&& item : robots) {
		// 	item.second.apply_strategy_attack(ball.x, ball.y);
		// }

		// robots[0].apply_strategy_attack(ball.x, ball.y);
		// robots[3].apply_strategy_attack(ball.x, ball.y);
		// robots[0].apply_strategy_gradient(ball.x, ball.y);
		robots[1].apply_strategy_svyat_style(ball.x, ball.y);
		robots[2].apply_strategy_attack(ball.x, ball.y, GATE_RIGHT_X);
		robots[4].apply_strategy_attack(ball.x, ball.y, GATE_LEFT_X);
		robots[5].apply_strategy_attack(ball.x, ball.y, GATE_RIGHT_X);
		robots[6].apply_strategy_attack(ball.x, ball.y, GATE_LEFT_X);

		// cout << robots[4] << endl;

		// for (int i = 0; i < 10; ++i) {
		// 	robots[100+i].apply_strategy_gradient(ball.x, ball.y);
		// }

		std::this_thread::sleep_for(milliseconds(DT_STRATEGIER));
	}
}

void drawer() {
	while (RUNNING) {
		// Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xF0, 0xFF);
		SDL_RenderClear(gRenderer);

		/* DBG - COLORED VECTOR FIELD */
		// double x1 = ball.x;
		// double y1 = ball.y;

		// int STEP = 2;

		// for (int i = 0; i <= SCREEN_WIDTH; i += STEP) {
		// 	double x = scr2mapX(i);

		// 	for (int j = 0; j <= SCREEN_HEIGHT; j += STEP) {
		// 		double y = scr2mapY(j);

		// 		double Fx, Fy, U;
		// 		calc_gradient_at(x, y, x1, y1, &Fx, &Fy, &U);
		// 		double F = sqrt(Fx*Fx + Fy*Fy);

		// 		double uu = U / 500000. * 255;
		// 		// double uu = F / 2000. * 255;
		// 		int r = 255;
		// 		int g = (uu > 255) ? 255 : (uu < 0) ? 0 : uu;
		// 		int b = 0;

		// 		SDL_SetRenderDrawColor(gRenderer, r, g, b, 0xFF);
		// 		SDL_RenderDrawPoint(gRenderer, i, j);
		// 	}
		// }
		/* DBG END */

		/* DBG - COLOR PUNCH AREA */
		int STEP2 = 4;
		for (int i = 0; i <= SCREEN_WIDTH; i += STEP2) {
			double x = scr2mapX(i);

			for (int j = 0; j <= SCREEN_HEIGHT; j += STEP2) {
				double y = scr2mapY(j);

				double w = 120;
				double h = sqrt(ROBOT_RADIUS*ROBOT_RADIUS - w*w/4.);
				double l = 50;
				bool b = false;

				for (auto&& item : robots) {
					if (item.second.kick) {
						double phi = -item.second.angle;
						double x_ = (x-item.second.x)*cos(phi) - (y-item.second.y)*sin(phi);
						double y_ = (x-item.second.x)*sin(phi) + (y-item.second.y)*cos(phi);

						if (h <= x_ && x_ <= h+l && -w/2 <= y_ && y_ <= w/2) {
							b = true;
							break;
						}
					}
				}

				if (b) {
					SDL_SetRenderDrawColor(gRenderer, 0x00, 0xBB, 0x20, 0xFF);
					SDL_RenderDrawPoint(gRenderer, i, j);
				}
			}
		}
		/* DBG END */

		// Render green map edges
		SDL_Rect map_edges = {
			map2scrX(MAP_EDGE_LEFT),
			map2scrY(MAP_EDGE_TOP),
			(int)((MAP_EDGE_RIGHT - MAP_EDGE_LEFT) * SCREEN_WIDTH / MAP_WIDTH),
			(int)((MAP_EDGE_TOP - MAP_EDGE_BOT) * SCREEN_HEIGHT / MAP_HEIGHT)
		};
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0xC0, 0x00, 0xFF);
		SDL_RenderDrawRect(gRenderer, &map_edges);

		// Render gate line
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0xA0, 0xFF, 0xFF);
		SDL_RenderDrawLine(gRenderer,
			map2scrX(GATE_LEFT_X), map2scrY(GATE_LEFT_TOP),
			map2scrX(GATE_LEFT_X), map2scrY(GATE_LEFT_BOT)
		);
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x83, 0x05, 0xFF);
		SDL_RenderDrawLine(gRenderer,
			map2scrX(GATE_RIGHT_X), map2scrY(GATE_RIGHT_TOP),
			map2scrX(GATE_RIGHT_X), map2scrY(GATE_RIGHT_BOT)
		);

		// Render robots
		for (auto&& item : robots) {
			item.second.render();
		}
		// Render ball
		ball.render();

		// Update screen
		SDL_RenderPresent(gRenderer);

		// cout << "Drawer :: " << duration_cast<milliseconds>(clock_used::now() - time_start).count()/1000. << " s" << endl;
		std::this_thread::sleep_for(milliseconds(DT_DRAWER));
	}
}

void modeller() {
	auto time_modeller = clock_used::now();

	while (RUNNING) {
		double time_left = DT_MODELLER / 1000.;
		int runs = 0;

		while (time_left > 0 && runs < 100) {
			double t_min = time_left;
			Robot* robot_i = nullptr;
			Robot* robot_j = nullptr;

			for (auto i = robots.begin(); i != robots.end(); ++i) {
				for (auto j = std::next(i); j != robots.end(); ++j) {
					double t = i->second.when_collide(j->second, time_left);
					if (t < t_min) {
						t_min = t;
						robot_i = &i->second;
						robot_j = &j->second;
					}
				}
			}

			if (t_min < time_left) {
				// Simulate t_min seconds
				for (auto&& item : robots) {
					item.second.apply_u(t_min)
				}

				// TODO: Interact robot_i with robot_j (or with ball...)
				if (robot_i != nullptr && robot_j != nullptr) {
					//
				}

				time_left -= t_min;
				++runs;
			}
			else {
				// Simulate time_left seconds unprecisely

				time_left = 0.0;
				break;
			}
		}

		if (time_left > 0) {
			// Simulate time_left seconds unprecisely
		}

		// double dt = duration<double, std::micro>(clock_used::now() - time_modeller).count() / 1000000.;
		// time_modeller = clock_used::now();
		// // cout << "Modeller :: dt = " << std::fixed << std::setprecision(5) << dt*1000 << " ms" << endl;

		// // Kick the ball
		// for (auto&& item : robots)
		// 	if (item.second.kick)
		// 		item.second.punch();

		// // Update them all
		// for (auto&& item : robots)
		// 	item.second.apply_u(dt);
		// ball.update(dt);

		// // Detect collisitions AFTER applying u
		// detect_collisions();

		// std::this_thread::sleep_for(duration<double, std::milli>(DT_MODELLER));
	}
}

void loaded() {
	while (RUNNING) {
		read_data("data");

		std::this_thread::sleep_for(duration<double, std::milli>(DT_LOADER));
	}
}

void saver() {
	while (RUNNING) {
		write_control("control");

		std::this_thread::sleep_for(duration<double, std::milli>(DT_SAVER));
	}
}


int main(int argc, char* argv[]) {
	srand(time(0));

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

	/* DEBUG */
	robots[0] = {500, 0};  // Gradient FTW
	robots[3] = {-500, 500};  // Standby
	robots[1] = {MAP_EDGE_LEFT+1250, 500, 1.5};  // GC

	robots[2] = {400, -200};  // Blue forward
	robots[2].kick = true;

	robots[4] = {-500, -500};  // Yellow forward
	robots[4].kick = true;
	robots[4].is_blue = false;

	robots[5] = {200, 300};
	robots[5].kick = true;

	robots[6] = {900, -100};
	robots[6].kick = true;
	robots[6].is_blue = false;

	// for (int i = 0; i < 10; ++i) {
	// 	robots[100+i] = {random(MAP_EDGE_LEFT+100, MAP_EDGE_RIGHT-100), random(MAP_EDGE_BOT+100, MAP_EDGE_TOP-100)};
	// }
	cout << "TOTAL ROBOTS AMOUNT = " << robots.size() << endl;
	/* DEBUG */

	std::thread th_strata(strategier);
	std::thread th_draw(drawer);
	std::thread th_model(modeller);
	// std::thread th_load(loader);
	// std::thread th_save(saver);

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
	// th_load.join();
	// th_save.join();

	cout << "Releasing SDL" << endl;
	release_sdl();

	cout << "The end." << endl;
	return 0;
}
