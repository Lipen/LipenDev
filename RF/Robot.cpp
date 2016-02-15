#include "main.hpp"
#include "LTexture.hpp"
#include "Robot.hpp"


Robot::Robot() : x(0), y(0), angle(0) {}

Robot::Robot(double x, double y, double angle)
: x(x), y(y), angle(angle) {}


void Robot::set_u(double ul, double ur) {
	u_left = (ul > 100) ? 100 : (ul < -100) ? -100 : ul;
	u_right = (ur > 100) ? 100 : (ul < -100) ? -100 : ur;
}

void Robot::apply_u(double dt) {
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

void Robot::render() {
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

void Robot::collide(Robot &other) {
	double ds = get_dist_squared(*this, other);
	double rs = radius + other.radius;

	if (ds <= 1e-9) {
		cout << "HMMMM ... " << *this << " (" << this << ") AND " << other << "(" << &other << ")" << endl;
		system("ini");
	}

	if (ds < rs*rs) {
		double semi = sqrt(ds) / 2;
		// double semi = (ds - other.radius*other.radius + radius*radius) / (2 * sqrt(ds) + 0.00001);  // Distance from this to radical line
		double alpha = atan2(other.y - y + 1e-6, other.x - x);  // Between this and other robot

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

void Robot::apply_strategy_attack(double x1, double y1) {
	/* DBG */
	double kappa = atan2(0 - y1, MAP_EDGE_LEFT - x1);
	double rho_remmm = 70;
	x1 -= rho_remmm * cos(kappa);
	y1 -= rho_remmm * sin(kappa);
	/* DBG */

	double x2 = x, y2 = y;  // Robot`s coordinates aliases
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
	__r = get_dist(x2, y2, a, b);

	double base_u = 70;
	double ul = base_u - PID_P * alpha;
	double ur = base_u + PID_P * alpha;

	// Do not forget to set u
	set_u(ul, ur);
}
