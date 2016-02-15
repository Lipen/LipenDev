#ifndef BALL_HPP
#define BALL_HPP

#include "Robot.hpp"


class Ball {
 public:
	double x, y;
	double vx, vy, ax, ay;
	double radius = BALL_RADIUS;

	explicit Ball(double x = 0.0, double y = 0.0, double vx = 0.0, double vy = 0.0, double ax = 0.0, double ay = 0.0);

	void update(double dt);
	void render();
	void collide(const Robot &other);

	friend std::ostream& operator<< (std::ostream &o, const Ball &b) {
		return o << "[Ball: x = " << std::fixed << std::setprecision(1) << b.x << ", y = " << b.y << "; v = " << std::setprecision(2) << b.vx << " / " << b.vy << "; a = " << b.ax << " / " << b.ay << "]";
	}
};

#endif // BALL_HPP
