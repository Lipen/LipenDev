#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "Ball.hpp"


class Robot {
 public:
	volatile double x, y;
	double angle;
	double u_left = 0.0, u_right = 0.0;
	double vx = 0.0, vy = 0.0;
	double radius = ROBOT_RADIUS;
	bool active = true;
	double __a = 0, __b = 0, __r = 0;

	Robot();
	Robot(double x, double y, double angle = 0.0);

	void set_u(double ul, double ur);
	void apply_u(double dt);
	void render();
	void collide(Robot &other);
	void apply_strategy_attack(double x1, double y1);
	void apply_strategy_goalkeeper(double x1, double y1);
	void apply_strategy_gradient(double x1, double y1);
	void apply_strategy_svyat_style(double x1, double y1);

	friend std::ostream& operator<< (std::ostream &o, const Robot &r) {
		return o << "[Robot: x=" << std::fixed << std::setprecision(1) << r.x << ", y=" << r.y << ", ang=" << std::setprecision(3) << r.angle << "]";
	}
};

#endif // ROBOT_HPP
