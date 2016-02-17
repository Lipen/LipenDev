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
	double IBRAGIM = 70;  // Distance before ball
	x1 -= IBRAGIM * cos(kappa);
	y1 -= IBRAGIM * sin(kappa);
	/* DBG */

	double x2 = x, y2 = y;  // Robot`s coordinates aliases
	double k = (0 - y1) / (MAP_EDGE_LEFT - x1);  // Slope of ball direction

	/* (a, b) is a trajectory center */
	double a = ( x1*x1*-k + 2*x1*y1 - 2*x1*y2 + y1*y1*k - 2*y1*y2*k + x2*x2*k + y2*y2*k ) / ( 2 * (-x1*k + y1 + x2*k - y2) );
	double b = (x1 - a) / k + y1;

	double chi = atan2(b-y2, a-x2);  // Between robot and circle center
	double tg = chi + HALF_PI * sign((x2-x1)*(0-y1) - (y2-y1)*(MAP_EDGE_LEFT-x1));  // Transfer to proper tangent
	normalize_angle(tg);  // ?
	double theta = atan2(y1 - y2, x1 - x2);  // Between robot and ball
	double d = get_dist(x2, y2, x1, y1);     // Between robot and ball
	double da = theta - tg;
	normalize_angle(da);
	double gamma = (d > ROBOT_THRESHOLD_MAX) ? 1 : (d < ROBOT_THRESHOLD_MIN) ? 0 : ROBOT_THRESHOLD_SLOPE * d + ROBOT_THRESHOLD_INTER;  // How much to skew from tangent ('tg') to to-the-ball-direction ('theta')

	double alpha = tg + da * gamma - angle;  // Delta alpha for p-regulator
	normalize_angle(alpha);

	// cout << "k = " << k << ", a = " << a << ", b=" << b << endl;
	__a = a;
	__b = b;
	__r = get_dist(x2, y2, a, b);

	double rho = d; //get_dist_to_line(x, y, x1, y1, MAP_EDGE_LEFT, 0);
	double ISCANDER = (rho > 500) ? 1 : (0.3 + (1-0.3)/(500-0)*rho);  // Slower

	double base_u = 70 * ISCANDER;
	double ul = base_u - PID_P * alpha;
	double ur = base_u + PID_P * alpha;

	// Do not forget to set u
	set_u(ul, ur);
}

void Robot::apply_strategy_goalkeeper(double x1, double y1) {
	double rho = get_dist_to_line(x, y, MAP_EDGE_LEFT+150, 300, MAP_EDGE_LEFT+150, -300);

	double ISABELL;  // dura
	if (y1 > 300)
		ISABELL = 300 - y;
	else if (y1 < -300)
		ISABELL = -300 - y;
	else
		ISABELL = y1 - y;

	bool lefter = x < MAP_EDGE_LEFT+150;  // Levee li?
	double BORIS = 0.5*rho;  // Line-keeper

	double base_u = 1 * ISABELL;
	double KKK = (angle > 0 && angle < PI) ? 1 : -1;
	double ul = KKK*(base_u + ISABELL*((lefter) ? BORIS : 0));
	double ur = KKK*(base_u + ISABELL*((lefter) ? 0 : BORIS));

	// cout << "Isa = " << ISABELL << ", Boria = " << BORIS << ", ul = " << ul << ", ur = " << ur << ", base_u = " << base_u << endl;

	set_u(ul, ur);
}

void Robot::apply_strategy_gradient(double x1, double y1) {
	double K1 = 20;
	double K2 = 17;

	double d = get_dist(x, y, x1, y1);
	double dFdx = -K1 * (x - x1) / d;
	double dFdy = -K1 * (y - y1) / d;

	for (auto&& item : robots) {
		if (&item.second != this) {
			double dist_obstacle = get_dist(x,y, item.second.x,item.second.y);
			if (dist_obstacle < 300)
				dFdx += K2 * (x - item.second.x) / dist_obstacle;
		}
	}

	double ksi = atan2(dFdy, dFdx);  // Needed angle
	double alpha = ksi - angle;  // Delta
	normalize_angle(alpha);

	double PAUL = 50;
	double base_u = 70 * ( (d > 500) ? 1 : (0.4+(1-0.4)/(500-0)*d) );
	double ul = base_u - PAUL * alpha;
	double ur = base_u + PAUL * alpha;

	// cout << "dFdx = " << dFdx << ", dFdy = " << dFdy << endl;
	// cout << "ksi = " << ksi << ", angle = " << angle << ", alpha = " << alpha << endl;

	set_u(ul, ur);
}

void Robot::apply_strategy_svyat_style(double x1, double y1) {
	double DIST_MAX = 100;
	double h = x - (MAP_EDGE_LEFT + 150);
	double d = get_dist(x, y, MAP_EDGE_LEFT+150, y1);
	double y_desired = constrain(y1, 300, -300);
	double dy = y_desired - y;

	double theta = (h > DIST_MAX) ? PI : (h < -DIST_MAX) ? 0 : (HALF_PI + HALF_PI * h/DIST_MAX * sign_(dy));
	double alpha = theta - angle;

	double P = 40;
	double base_u = 80 * ((d > 200) ? 1 : (0.1+(1-0.1)/(200-0)*d)) * sign(dy);
	double ul = base_u - P*alpha;
	double ur = base_u + P*alpha;

	set_u(ul, ur);
}
