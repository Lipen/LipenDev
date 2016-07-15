#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <cassert>
#include <type_traits>
#include <chrono>
#include <functional>
#include <random>

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std::chrono;
using std::cout;
using std::endl;
using std::string;

struct Component;
class Entity;
class Manager;
class Game;

using FrameTime = float;
using ComponentID = size_t;
using Group = size_t;

constexpr unsigned int window_width{ 1200 }, window_height{ 600 };
constexpr float ft_step{ .1f }, ft_slice{ .1f };
constexpr unsigned int fps_limit{ 240 };

constexpr float block_width{ 60.f }, block_height{ 20.f };
constexpr float dino_width{ 80.f }, dino_height{ 80.f },
				dino_baseline{ 400.f },
				dino_jump_speed{ 1.3f },
				dino_jump_acceleration{ .004f },
				dino_descent_acceleration{ 0.008f };
constexpr float cactus_width{ 64.f }, cactus_height{ 118.f },
				cactus_baseline{ 400.f },
				cactus_speed{ .5f },
				cactus_gap_min{ 200.f }, cactus_gap_max{ window_width };

constexpr ComponentID max_components{ 32 };
using ComponentBitset = std::bitset<max_components>;
using ComponentArray = std::array<Component*, max_components>;

constexpr Group max_groups{ 32 };
using GroupBitset = std::bitset<max_groups>;


inline ComponentID get_unique_component_id() noexcept {
	static ComponentID last_ID{ 0 };
	return last_ID++;
}

template <typename T>
inline ComponentID get_component_type_id() noexcept {
	static_assert(std::is_base_of<Component, T>::value,
				  "T must inherit from Component");

	static ComponentID type_ID{ get_unique_component_id() };
	return type_ID;
}

float random(float low, float high) {
	return low + (double)rand() / RAND_MAX * (high - low);
}


struct Component {
	Entity* entity;

	virtual void init() {}
	virtual void update(FrameTime) {}
	virtual void draw() {}

	virtual ~Component() {}
};

class Entity {
	Manager& manager;

	bool alive = true;
	std::vector<std::unique_ptr<Component>> components;
	ComponentArray component_array;
	ComponentBitset component_bitset;

	GroupBitset group_bitset;

 public:
	explicit Entity(Manager& manager) : manager(manager) {}

	void update(FrameTime dt) {
		for (auto& c : components) c->update(dt);
	}

	void draw() {
		for (auto& c : components) c->draw();
	}

	bool is_alive() const { return alive; }
	void destroy() { alive = false; }

	bool has_group(Group group) const noexcept { return group_bitset[group]; }
	void add_group(Group group) noexcept;
	void del_group(Group group) noexcept { group_bitset[group] = false; }

	template <typename T>
	bool has_component() const {
		return component_bitset[get_component_type_id<T>()];
	}

	template <typename T, typename... TArgs>
	T& add_component(TArgs&&... args) {
		assert(!has_component<T>());

		T* c = new T(std::forward<TArgs>(args)...);
		c->entity = this;
		std::unique_ptr<Component> uptr{c};
		components.emplace_back(std::move(uptr));

		component_array[get_component_type_id<T>()] = c;
		component_bitset[get_component_type_id<T>()] = true;

		c->init();
		return *c;
	}

	template <typename T>
	T& get_component() const {
		assert(has_component<T>());

		auto ptr = component_array[get_component_type_id<T>()];

		return *reinterpret_cast<T*>(ptr);
	}
};

class Manager {
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, max_groups> grouped_entities;

 public:
	Manager() = default;
	// Manager() { cout << "Manager created" << endl; }

	void update(FrameTime dt) {
		for (auto& e : entities) e->update(dt);
	}

	void draw() {
		for (auto& e : entities) e->draw();
	}

	void add_to_group(Entity* entity, Group group) {
		grouped_entities[group].emplace_back(entity);
	}

	std::vector<Entity*>& get_entities_by_group(Group group) {
		return grouped_entities[group];
	}

	void refresh() {
		for (Group i = 0; i < max_groups; ++i) {
			auto& v = grouped_entities[i];

			v.erase(std::remove_if(begin(v), end(v), [i](Entity* entity) {
				return !entity->is_alive() || !entity->has_group(i);
			}), end(v));
		}

		entities.erase(std::remove_if(begin(entities), end(entities),
			[](const std::unique_ptr<Entity>& entity) {
				return !entity->is_alive();
			}), end(entities));
	}

	Entity& add_entity() {
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uptr{e};
		entities.emplace_back(std::move(uptr));

		return *e;
	}
};

void Entity::add_group(Group group) noexcept {
	group_bitset[group] = true;
	manager.add_to_group(this, group);
}


struct CPosition : Component {
	Vector2f position;

	CPosition() = default;
	explicit CPosition(const Vector2f& pos) : position(pos) {}

	float x() const noexcept { return position.x; }
	float y() const noexcept { return position.y; }
};

struct CPhysics : Component {
	CPosition* position{ nullptr };
	Vector2f velocity, acceleration, half_size;

	std::function<void(const Vector2f&)> on_outofbounds;

	explicit CPhysics(const Vector2f& half_size) : half_size(half_size) {}

	void init() override {
		position = &entity->get_component<CPosition>();
	}

	void update(FrameTime dt) override {
		velocity += acceleration * dt;
		position->position += velocity * dt;

		if (on_outofbounds == nullptr) return;

		if (left() < 0)
			on_outofbounds(Vector2f{ 1.f, 0.f });
		else if (right() > window_width)
			on_outofbounds(Vector2f{ -1.f, 0.f });

		if (top() < 0)
			on_outofbounds(Vector2f{ 0.f, 1.f });
		else if (bottom() > window_height)
			on_outofbounds(Vector2f{ 0.f, -1.f });
	}

	float x() const noexcept { return position->x(); }
	float y() const noexcept { return position->y(); }
	float left() const noexcept   { return x() - half_size.x; }
	float right() const noexcept  { return x() + half_size.x; }
	float top() const noexcept    { return y() - half_size.y; }
	float bottom() const noexcept { return y() + half_size.y; }
};

struct CCircle : Component {
	Game* game{ nullptr };
	CPosition* position{ nullptr };
	CircleShape shape;
	float radius;

	CCircle(Game* game, float radius) : game(game), radius(radius) {}

	void init() override {
		position = &entity->get_component<CPosition>();

		shape.setRadius(radius);
		shape.setFillColor(Color::Red);
		shape.setOrigin(radius, radius);
	}

	void update(FrameTime) override {
		shape.setPosition(position->position);
	}

	void draw() override;
};

struct CRectangle : Component {
	Game* game{ nullptr };
	CPosition* position{ nullptr };
	RectangleShape shape;
	Vector2f size;

	CRectangle(Game* game, const Vector2f& half_size)
	: game(game), size(half_size * 2.f)
	{}

	void init() override {
		position = &entity->get_component<CPosition>();

		shape.setSize(size);
		shape.setFillColor(Color::Red);
		shape.setOrigin(size.x / 2.f, size.y / 2.f);
	}

	void update(FrameTime) override {
		shape.setPosition(position->position);
	}

	void draw() override;
};

struct CSprite : Component {
	Game* game{ nullptr };
	CPosition* position{ nullptr };
	Texture texture;
	Sprite sprite;
	Vector2f size;

	CSprite(Game* game, const Vector2f& half_size, string texture_path)
	: game(game), size(half_size * 2.f)
	{
		texture.loadFromFile(texture_path);
	}

	void init() override {
		position = &entity->get_component<CPosition>();

		sprite.setTexture(texture);
		sprite.setTextureRect({0, 0, static_cast<int>(size.x), static_cast<int>(size.y)});
		sprite.setOrigin(size / 2.f);
		// sprite.setScale(size.x / texture.getSize().x, size.y / texture.getSize().y);
	}

	void update(FrameTime) override {
		sprite.setPosition(position->position);
	}

	void draw() override;

	void set_frame(int row, int column) {
		sprite.setTextureRect({static_cast<int>(size.x) * column,
							   static_cast<int>(size.y) * row,
							   static_cast<int>(size.x),
							   static_cast<int>(size.y)});
	}
};

struct CJumpable : Component {
	Game* game{ nullptr };
	CPhysics* physics{ nullptr };
	bool is_jumping{ false };

	explicit CJumpable(Game* game) : game(game) {}

	void init() override {
		physics = &entity->get_component<CPhysics>();
	}

	void update(FrameTime) override {
		if (is_jumping) {
			if (physics->position->position.y > dino_baseline) {
				is_jumping = false;
				physics->velocity = Vector2f{};
				physics->acceleration = Vector2f{};
				physics->position->position.y = dino_baseline;

				if (entity->has_component<CSprite>()) {
					auto& sprite = entity->get_component<CSprite>();
					sprite.set_frame(0, 0);
				}
			}
		}
	}

	void jump() {
		if (!is_jumping) {
			is_jumping = true;
			physics->velocity = Vector2f{0.f, -dino_jump_speed};
			physics->acceleration = Vector2f{0.f, dino_jump_acceleration};

			if (entity->has_component<CSprite>()) {
				auto& sprite = entity->get_component<CSprite>();
				sprite.set_frame(0, 1);
			}
		}
	}

	void descent() {
		if (is_jumping) {
			physics->acceleration = Vector2f{0.f, dino_descent_acceleration};
		}
	}
};

struct CSpawner : Component {
	Game* game{ nullptr };
	Group spawned_group;
	float random_distance = 0.f;

	CSpawner(Game* game, Group group) : game(game), spawned_group(group) {}

	void update(FrameTime) override;
	void spawn();
};

struct CReleasableLeft : Component {
	CPosition* position{ nullptr };
	float release_position_x;

	explicit CReleasableLeft(float dist) : release_position_x(dist) {}

	void init() override {
		position = &entity->get_component<CPosition>();
	}

	void update(FrameTime) override {
		if (position->x() < release_position_x) {
			entity->destroy();
		}
	}
};

struct Game {
	enum GameGroup : Group {
		GDino,
		GCactus,
		GCactusSpawner,
		GPterodactyl,
	};

	RenderWindow window{ {window_width, window_height}, "Dino" };
	FrameTime last_ft{ 0.f }, current_slice{ 0.f };
	bool running{ false };
	int cactuses_to_spawn{ 0 };
	Manager manager;

	Game() {
		window.setFramerateLimit(fps_limit);

		create_dino({ 100.f, dino_baseline });
		create_cactus_spawner();

		Image icon;
		icon.loadFromFile("dinosty.png");
		Vector2u icon_size = icon.getSize();
		window.setIcon(icon_size.x, icon_size.y, icon.getPixelsPtr());
	}

	void run() {
		running = true;

		while (running) {
			auto time_point1 = system_clock::now();

			window.clear(Color::Black);

			input_phase();
			update_phase();
			draw_phase();

			auto time_point2 = system_clock::now();
			auto elapsed_time = time_point2 - time_point1;
			FrameTime ft = duration_cast<duration<float, std::milli>>(elapsed_time).count();

			last_ft = ft;

			auto ft_seconds = ft / 1000.f;
			auto fps = 1.f / ft_seconds;

			window.setTitle("FrameTime: " + std::to_string(ft) + "\tFPS: " + std::to_string(fps));
		}
	}

	void input_phase() {
		Event event;

		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
				running = false;
				break;
			}
			else if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Escape) {
					window.close();
					running = false;
					break;
				}
				else if (event.key.code == Keyboard::Up || event.key.code == Keyboard::Space) {
					for (auto& dino_entity : manager.get_entities_by_group(GameGroup::GDino)) {
						dino_entity->get_component<CJumpable>().jump();
					}
				}
				else if (event.key.code == Keyboard::Down) {
					for (auto& dino_entity : manager.get_entities_by_group(GameGroup::GDino)) {
						dino_entity->get_component<CJumpable>().descent();
					}
				}
			}
		}
	}

	void update_phase() {
		current_slice += last_ft;

		for(; current_slice >= ft_slice; current_slice -= ft_slice) {
			manager.refresh();
			manager.update(ft_step);
		}

		for (; cactuses_to_spawn > 0; --cactuses_to_spawn) {
			auto& ent = create_cactus();
			ent.get_component<CPhysics>().velocity.x = -cactus_speed;
		}
	}

	void draw_phase() {
		manager.draw();
		window.display();
	}

	Entity& create_dino(const Vector2f& position) {
		Vector2f half_size{ dino_width / 2.f, dino_height / 2.f };
		auto& entity = manager.add_entity();

		entity.add_component<CPosition>(position);
		entity.add_component<CPhysics>(half_size);
		entity.add_component<CSprite>(this, half_size, "dino.png");
		entity.add_component<CJumpable>(this);

		entity.add_group(GameGroup::GDino);

		return entity;
	}

	Entity& create_cactus_spawner() {
		auto& entity = manager.add_entity();

		entity.add_component<CSpawner>(this, GameGroup::GCactus);

		entity.add_group(GameGroup::GCactusSpawner);

		return entity;
	}

	Entity& create_cactus() {
		Vector2f position{ window_width, cactus_baseline };
		Vector2f half_size{ cactus_width / 2.f, cactus_height / 2.f };
		auto& entity = manager.add_entity();

		entity.add_component<CPosition>(position);
		entity.add_component<CPhysics>(half_size);
		entity.add_component<CSprite>(this, half_size, "cactus.png");
		entity.add_component<CReleasableLeft>(-300.f);

		entity.add_group(GameGroup::GCactus);

		return entity;
	}

	void render(const Drawable& drawable) {
		window.draw(drawable);
	}
};

void CCircle::draw()    { game->render(shape); }
void CRectangle::draw() { game->render(shape); }
void CSprite::draw()    { game->render(sprite); }

void CSpawner::update(FrameTime) {
	const std::vector<Entity*>& entities = game->manager.get_entities_by_group(spawned_group);

	if (game->cactuses_to_spawn == 0) {
		if (entities.size() > 0) {
			float d = std::abs(window_width - entities.back()->get_component<CPosition>().x());
			if (d >= random_distance) {
				spawn();
			}
		}
		else {
			spawn();
		}
	}
}

void CSpawner::spawn() {
	game->cactuses_to_spawn++;

	random_distance = random(cactus_gap_min, cactus_gap_max);
}


int main() {
	srand(duration_cast<std::chrono::seconds>(system_clock::now().time_since_epoch()).count());
	Game{}.run();
	cout << "The end." << endl;
}
