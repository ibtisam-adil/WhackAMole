#include "galba.h"
#include "raylib.h"
#include <vector>
#include <list>
#include <cmath>

Vector2 random_direction() {
	float angle = random_float_01() * 2 * PI;
	return Vector2{ cosf(angle), sinf(angle) };
}

// Entity Class
class Entity {
public:
	Vector2 position = {};
	bool dead = false;

	virtual void update() = 0;
	virtual void render() = 0;
};

// TODO: Goat class goes here!
class Goat : public Entity {
public:
	Vector2 position;
	Vector2 direction;
	Color goat_color;
	int goat_radius = 16;
	float goat_speed = random_float_in_range(2.0f, 6.0f);

	Goat(float x = 0, float y = 0, Color color = white)
		: position{ x, y }, goat_color(color), direction(random_direction()) {
	}

	void update() override {
		int window_width = get_window_width();
		int window_height = get_window_height();

		position.x += direction.x * goat_speed;
		position.y += direction.y * goat_speed;

		if (position.x >= window_width - goat_radius || position.x <= goat_radius) {
			direction.x *= -1.0f;
		}
		if (position.y >= window_height - goat_radius || position.y <= goat_radius) {
			direction.y *= -1.0f;
		}
	}

	void render() override {
		draw_circle(round_to_int(position.x), round_to_int(position.y), goat_radius, goat_color);
	}
};

// TODO: Bush class goes here!
class Bush : public Entity {
public:
	Vector2 position;
	int starting_bush_size = 20;
	int target_bush_size = 40;
	int current_bush_size;
	int frames_passed = 0;
	int growth_duration = 600;
	float size_per_frame;
	int die_counter = 0;

	Color bush_color = green;

	Bush(float x = 0, float y = 0)
		: position{ x, y },
		current_bush_size(starting_bush_size),
		size_per_frame((target_bush_size - starting_bush_size) / static_cast<float>(growth_duration))
	{}

	void update() override {
		if (frames_passed < growth_duration) {
			current_bush_size = starting_bush_size + static_cast<int>(frames_passed * size_per_frame);
			frames_passed++;
		}
		else {
			current_bush_size = target_bush_size;
			bush_color = red;
			int state_red_duration = 120;
			if (die_counter < state_red_duration) {
				die_counter++;
			}
			else if (die_counter == state_red_duration) {
				dead = true;
			}
		}
	}

	void render() override {
		draw_circle(round_to_int(position.x), round_to_int(position.y), current_bush_size, bush_color);
	}
};

class Level {
private:
	std::list<Goat> all_goats;
	std::list<Bush> all_bushes;
	std::vector<Entity*> all_entities;

public:
	void add_entity(const Goat& goat);
	void add_entity(const Bush& bush);
	void remove_all_dead_entities();
	void update();
	void render();
};

void Level::add_entity(const Goat& goat) {
	all_goats.push_back(goat);
	all_entities.push_back(&all_goats.back());
}

void Level::add_entity(const Bush& bush) {
	all_bushes.push_back(bush);
	all_entities.push_back(&all_bushes.back());
}

void Level::remove_all_dead_entities() {
	auto new_last_entity = std::remove_if(all_entities.begin(), all_entities.end(),
		[](const Entity* e) -> bool { return e->dead; });
	all_entities.erase(new_last_entity, all_entities.end());

	all_goats.remove_if([](const Goat& g) -> bool { return g.dead; });
	all_bushes.remove_if([](const Bush& b) -> bool { return b.dead; });
}

void Level::update() {
	if (is_button_pressed(Button::Right)) {
		Goat new_goat(static_cast<float>(get_mouse_x()), static_cast<float>(get_mouse_y()));
		add_entity(new_goat);
	}

	if (is_button_pressed(Button::Left)) {
		Bush new_bush(static_cast<float>(get_mouse_x()), static_cast<float>(get_mouse_y()));
		add_entity(new_bush);
	}

	for (Entity* entity : all_entities) {
		entity->update();
	}
	remove_all_dead_entities();
}

void Level::render() {
	for (Entity* entity : all_entities) {
		entity->render();
	}
}

int main() {
	Level level;
	initialize(1280, 720, "Window");

	while (is_window_open()) {
		level.update();
		clear_window();
		level.render();
		display();
	}

	return 0;
}
