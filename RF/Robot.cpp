#include "main.hpp"
#include "LTexture.hpp"
#include "Robot.hpp"


Robot::Robot() : x(0), y(0), angle(0) {
	cout << "Robot::Robot() ~ empty ctor call" << endl;
}

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

void Robot::punch() {
	double w = 120;
	double h = sqrt(ROBOT_RADIUS*ROBOT_RADIUS - w*w/4);
	double l = 64;

	double dx = ball.x - x;
	double dy = ball.y - y;
	double x_ = dx*cos(angle) + dy*sin(angle);
	double y_ = -dx*sin(angle) + dy*cos(angle);

	if (h <= x_ && x_ <= h+l && -w/2 <= y_ && y_ <= w/2) {
		double theta = atan2(dy, dx);  // Between ball and robot
		double beta = atan2(ball.vy, ball.vy);  // Angle of ball`s velocity
		double da = theta - beta;

		double vnx = ball.vx * cos(da);  // Norm component
		double vny = ball.vy * cos(da);
		double vtx = ball.vx * sin(da);  // Tangent component
		double vty = ball.vy * sin(da);

		/* u2 = (2m1v1 + v2(m2-m1)) / (m1+m2) */  // Norm component
		double ux = (2*ROBOT_MASS*vx + vnx*(BALL_MASS - ROBOT_MASS)) / (ROBOT_MASS + BALL_MASS);
		double uy = (2*ROBOT_MASS*vy + vny*(BALL_MASS - ROBOT_MASS)) / (ROBOT_MASS + BALL_MASS);

		// Add extra velocity from kicker
		double vextra = 800;
		ux += vextra * cos(angle);
		uy += vextra * sin(angle);

		ball.vx = ux + vtx;
		ball.vy = uy + vty;
	}
}

void Robot::render() {
	if (is_blue)
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0xD2, 0xFF, 0xFF);
	else
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x9A, 0x00, 0xFF);
	draw_circle(x, y, radius);

	/* THAT TRICKY CIRCLES */
	if (is_blue)
		SDL_SetRenderDrawColor(gRenderer, 0x49, 0x39, 0xFF, 0xFF);
	else
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x4D, 0x00, 0xFF);
	draw_circle(__a, __b, __r);
	/**/

	gRobotTexture.render(map2scrX(x), map2scrY(y), NULL, -angle*180/PI + 90.);
}

void Robot::collide(Robot &other) {
	double ds = get_dist_squared(*this, other);
	double rs = radius + other.radius;

	if (ds <= 1e-9) {
		cout << "HMMMM ... " << *this << " (" << this << ") AND " << other << "(" << &other << ")" << endl;
	}

	if (ds < rs*rs) {
		// double semi = (ds - other.radius*other.radius + radius*radius) / (2 * sqrt(ds) + 0.00001);  // Distance from 'this' to radical line
		double semi = sqrt(ds) / 2;
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

void Robot::apply_strategy_attack(double x1, double y1, double Gx, double Gy) {
	/* DBG */
	double kappa = atan2(Gy - y1, Gx - x1);
	double IBRAGIM = 80;  // Distance before ball
	x1 -= IBRAGIM * cos(kappa);
	y1 -= IBRAGIM * sin(kappa);
	/* DBG */

	double x2 = x, y2 = y;  // Robot`s coordinates aliases
	double k = (Gy - y1 + 1e-6) / (Gx - x1);  // Slope of ball direction

	/* (a, b) is a trajectory center */
	double a = ( x1*x1*-k + 2*x1*y1 - 2*x1*y2 + y1*y1*k - 2*y1*y2*k + x2*x2*k + y2*y2*k ) / ( 2 * (-x1*k + y1 + x2*k - y2) );
	double b = (x1 - a) / k + y1;

	double chi = atan2(b-y2, a-x2);  // Between robot and circle center
	double tg = chi + HALF_PI * sign((x2-x1)*(Gy-y1) - (y2-y1)*(Gx-x1));  // Transfer to proper tangent
	normalize_angle(tg);  // ?
	double theta = atan2(y1 - y2, x1 - x2);  // Between robot and ball
	double d = get_dist(x2, y2, x1, y1);  // Between robot and ball
	double da = theta - tg;
	normalize_angle(da);
	double gamma = (d > ROBOT_THRESHOLD_MAX) ? 1 : (d < ROBOT_THRESHOLD_MIN) ? 0 : ROBOT_THRESHOLD_SLOPE * d + ROBOT_THRESHOLD_INTER;  // How much to skew from tangent ('tg') to to-the-ball-direction ('theta')

	double alpha = tg + da * gamma - angle;  // Delta alpha for p-regulator
	normalize_angle(alpha);

	// cout << "k = " << k << ", a = " << a << ", b=" << b << endl;
	// cout << "tg = " << tg << ", gamma = " << gamma << ", alpha = " << alpha << endl;
	__a = a;
	__b = b;
	__r = get_dist(x2, y2, a, b);

	double len = normalized_angle(atan2(y1 - b, x1 - a) - atan2(y - b, x - a), PI) * __r;
	double P = 100;
	// double ISCANDER = logistic_linear(len, 0.2, 300);  // Slower as closer
	double ISCANDER = logistic_sigmoid(len, 200, 150);
	double base_u = 80 * ISCANDER;
	double ul = base_u - P * alpha;
	double ur = base_u + P * alpha;

	// Do not forget to set u
	set_u(ul, ur);
}

void Robot::apply_strategy_goalkeeper(double x1, double y1) {
	double rho = get_dist_to_line(x, y, GATE_LEFT_X, GATE_LEFT_TOP, GATE_LEFT_X, GATE_LEFT_BOT);

	double ISABELL;  // dura
	if (y1 > GATE_LEFT_TOP)
		ISABELL = GATE_LEFT_TOP - y;
	else if (y1 < GATE_LEFT_BOT)
		ISABELL = GATE_LEFT_BOT - y;
	else
		ISABELL = y1 - y;

	bool lefter = x < GATE_LEFT_X;
	double BORIS = 0.5*rho;  // Line-keeper

	double base_u = 1 * ISABELL;
	double KKK = (angle > 0 && angle < PI) ? 1 : -1;
	double ul = KKK*(base_u + ISABELL*((lefter) ? BORIS : 0));
	double ur = KKK*(base_u + ISABELL*((lefter) ? 0 : BORIS));

	// cout << "Isa = " << ISABELL << ", Boria = " << BORIS << ", ul = " << ul << ", ur = " << ur << ", base_u = " << base_u << endl;

	set_u(ul, ur);
}

void Robot::apply_strategy_gradient(double x1, double y1) {
	double Fx, Fy, U;
	calc_gradient_at(x, y, x1, y1, &Fx, &Fy, &U);
	double F = sqrt(Fx*Fx + Fy*Fy);

	// cout << "F = {" << Fx << ", " << Fy << "} == " << F << ", U = " << U << endl;

	double ksi = atan2(Fy, Fx);  // Needed angle
	double alpha = ksi - angle;  // Delta
	normalize_angle(alpha);

	double PAUL = 100;
	double d = get_dist(x, y, x1, y1);
	double base_u = 80 * logistic_linear(d, 0.4, 200);
	double ul = base_u - PAUL * alpha;
	double ur = base_u + PAUL * alpha;

	// cout << "ksi = " << ksi << ", angle = " << angle << ", alpha = " << alpha << endl;

	set_u(ul, ur);
}

void Robot::apply_strategy_svyat_style(double x1, double y1) {
	double DIST_MAX = 100;
	double h = x - GATE_LEFT_X;
	double d = get_dist(x, y, GATE_LEFT_X, y1);
	double y_desired = constrain(y1, GATE_LEFT_TOP, GATE_LEFT_BOT);
	double dy = y_desired - y;

	double theta = (h > DIST_MAX) ? PI : (h < -DIST_MAX) ? 0 : (HALF_PI + HALF_PI * h/DIST_MAX * sign_(dy));
	// WORKAROUND ANDREI STYLE
	if (h > DIST_MAX && dy < 0)
		theta = 0;
	//
	double alpha = theta - angle;
	normalize_angle(alpha);

	double P = 40;
	double base_u = 80 * logistic_linear(d, 0.05, 100) * sign(dy);
	// if (h < DIST_MAX && h > -DIST_MAX)  base_u *= sign(dy);
	double ul = base_u - P*alpha;
	double ur = base_u + P*alpha;

	set_u(ul, ur);
}
