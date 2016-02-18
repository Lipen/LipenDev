#include "main.hpp"
#include "LTexture.hpp"
#include "Ball.hpp"


Ball::Ball(double x, double y, double vx, double vy, double ax, double ay)
: x(x), y(y), vx(vx), vy(vy), ax(ax), ay(ay) {}


void Ball::update(double dt) {
	vx += ax * dt;
	vy += ay * dt;
	vx *= BALL_FRICTION;  // Friction
	vy *= BALL_FRICTION;
	if (vx*vx + vy*vy > BALL_MAXSPEED*BALL_MAXSPEED) {
		double va = atan2(vy, vx);
		vx = BALL_MAXSPEED * cos(va);
		vy = BALL_MAXSPEED * sin(va);
	}

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

void Ball::render() {
	double a = map2scrX(x);
	double b = map2scrY(y);

	gBallTexture.render(a, b);
}

void Ball::collide(const Robot &other) {
	double ds = get_dist_squared(*this, other);  // DistanceSquared
	double rs = radius + other.radius;  // RaduisesSum

	if (ds < rs*rs) {
		// cout << "COLLISION WITH BALL" << endl;
		double dx = other.x - x;
		double dy = other.y - y;
		double alpha = atan2(dy, dx);  // Between ball and robot
		double beta = atan2(vy, vx);   // Angle of ball`s velocity
		double da = alpha - beta;

		double vnx = vx * cos(da);  // Norm component
		double vny = vy * cos(da);
		double vtx = vx * sin(da);  // Tangent component
		double vty = vy * sin(da);

		/* u2 = (2m1v1 + v2(m2-m1)) / (m1+m2) */  // Norm component
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

