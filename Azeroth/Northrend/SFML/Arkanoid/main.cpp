#include <vector>
#include <chrono>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std::chrono;

using FrameTime = float;

constexpr int window_width{800}, window_height{600};
constexpr float ball_radius{10.}, ball_velocity{.8};
constexpr float paddle_width{100.}, paddle_height{20.}, paddle_velocity{.6};
constexpr float brick_width{60.}, brick_height{20.};
constexpr int count_bricks_x{11}, count_bricks_y{5};
constexpr float ft_step{.5}, ft_slice{.5};


struct Ball {
	CircleShape shape;
	Vector2f velocity{-ball_velocity, -ball_velocity};

	Ball(float x, float y) {
		shape.setPosition(x, y);
		shape.setRadius(ball_radius);
		shape.setFillColor(Color::Red);
		shape.setOrigin(ball_radius, ball_radius);
	}


	void update(FrameTime dt) {
		shape.move(velocity * dt);

		if (left() < 0)
			velocity.x = ball_velocity;
		else if (right() > window_width)
			velocity.x = -ball_velocity;

		if (top() < 0)
			velocity.y = ball_velocity;
		else if (bottom() > window_height)
			velocity.y = -ball_velocity;
	}

	// Getters
	float x() const { return shape.getPosition().x; }
	float y() const { return shape.getPosition().y; }
	float left() const { return x() - shape.getRadius(); }
	float right() const { return x() + shape.getRadius(); }
	float top() const { return y() - shape.getRadius(); }
	float bottom() const { return y() + shape.getRadius(); }
};

struct Paddle {
	RectangleShape shape;
	Vector2f velocity;

	Paddle(float x, float y) {
		shape.setPosition(x, y);
		shape.setSize({paddle_width, paddle_height});
		shape.setFillColor(Color::Red);
		shape.setOrigin(paddle_width / 2., paddle_height / 2.);
	}


	void update(FrameTime dt) {
		shape.move(velocity * dt);

		if (Keyboard::isKeyPressed(Keyboard::Key::Left) && left() > 0)
			velocity.x = -paddle_velocity;
		else if (Keyboard::isKeyPressed(Keyboard::Key::Right) && right() < window_width)
			velocity.x = paddle_velocity;
		else
			velocity.x = 0;
	}

	// Getters
	float x() const { return shape.getPosition().x; }
	float y() const { return shape.getPosition().y; }
	float left() const { return x() - shape.getSize().x / 2.; }
	float right() const { return x() + shape.getSize().x / 2.; }
	float top() const { return y() - shape.getSize().y / 2.; }
	float bottom() const { return y() + shape.getSize().y / 2.; }
};

struct Brick {
	RectangleShape shape;
	bool destroyed{false};

	Brick(float x, float y) {
		shape.setPosition(x, y);
		shape.setSize({brick_width, brick_height});
		shape.setFillColor(Color::Yellow);
		shape.setOrigin(brick_width / 2., brick_height / 2.);
	}


	// Getters
	float x() const { return shape.getPosition().x; }
	float y() const { return shape.getPosition().y; }
	float left() const { return x() - shape.getSize().x / 2.; }
	float right() const { return x() + shape.getSize().x / 2.; }
	float top() const { return y() - shape.getSize().y / 2.; }
	float bottom() const { return y() + shape.getSize().y / 2.; }
};


template<typename T, typename U>
bool is_intersecting(const T& a, const U& b) {
	return a.right() >= b.left() && a.left() <= b.right() &&
			a.bottom() >= b.top() && a.top() <= b.bottom();
}

void test_collisions(Paddle& paddle, Ball& ball) {
	// If no intersection - leave instantly
	if (!is_intersecting(paddle, ball))
		return;

	// Otherwise 'push' the ball upwards
	ball.velocity.y = -ball_velocity;

	// Change direction depending on paddle's hitted zone
	if (ball.x() < paddle.x())
		ball.velocity.x = -ball_velocity;  // to the left
	else
		ball.velocity.x = ball_velocity;  // to the right
}

void test_collisions(Brick& brick, Ball& ball) {
	// If no intersection - leave instantly
	if (!is_intersecting(brick, ball))
		return;

	brick.destroyed = true;

	float overlap_left = ball.right() - brick.left();
	float overlap_right = brick.right() - ball.left();
	float overlap_top = ball.bottom() - brick.top();
	float overlap_bottom = brick.bottom() - ball.top();

	bool ball_from_left = std::abs(overlap_left) < std::abs(overlap_right);
	bool ball_from_top = std::abs(overlap_top) < std::abs(overlap_bottom);

	float min_overlap_x = ball_from_left ? overlap_left : overlap_right;
	float min_overlap_y = ball_from_top ? overlap_top : overlap_bottom;

	if (std::abs(min_overlap_x) < std::abs(min_overlap_y))
		ball.velocity.x = ball_from_left ? -ball_velocity : ball_velocity;
	else
		ball.velocity.y = ball_from_top ? -ball_velocity : ball_velocity;
}


int main() {
	Ball ball{window_width / 2, window_height / 2};
	Paddle paddle{window_width / 2, window_height - 50};
	std::vector<Brick> bricks;

	for (int i = 0; i < count_bricks_x; ++i)
		for (int j = 0; j < count_bricks_y; ++j)
			bricks.emplace_back((i + 1) * (brick_width + 3) + 22,
								(j + 2) * (brick_height + 3));

	RenderWindow window{{window_width, window_height}, "Arkanoid"};
	window.setFramerateLimit(15);

	FrameTime last_ft = 0.0;
	FrameTime current_ft = 0.0;


	while (true) {
		auto time_point1 = system_clock::now();

		// Clear screen
		window.clear(Color::Black);

		// Process events
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
				return 0;
			}
		}

		// Process exit by Escape press
		if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
			break;

		current_ft += last_ft;

		for (; current_ft >= ft_slice; current_ft -= ft_slice) {
			// Update
			ball.update(ft_step);
			paddle.update(ft_step);

			// Collisions check
			test_collisions(paddle, ball);
			for (auto& brick : bricks)
				test_collisions(brick, ball);

			// Remove destroyed bricks
			bricks.erase(std::remove_if(begin(bricks), end(bricks), [](const Brick& brick) { return brick.destroyed; }), end(bricks));
		}

		// Render
		window.draw(ball.shape);
		window.draw(paddle.shape);
		for (auto& brick : bricks)
			window.draw(brick.shape);
		window.display();

		// Time magic
		auto time_point2 = system_clock::now();
		auto elapsed_time = time_point2 - time_point1;

		FrameTime ft = duration_cast<duration<float, std::milli>>(elapsed_time).count();
		last_ft = ft;

		auto ft_seconds = ft / 1000.;
		auto fps = 1. / ft_seconds;

		window.setTitle("FT: " + std::to_string(ft) + "\tFPS: " + std::to_string(fps));
	}
}
