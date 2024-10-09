
// IBTISAM ADIL

#include "galba.h"
#include "raylib.h"
#include <vector>
#include <list>
#include <chrono>
#include <thread>

int SCORE = 0;

// Entity Class
class Entity {
public:
	bool dead = false;

	virtual void update() = 0;
	virtual void render() = 0;
};

// Mole Class
class Mole : public Entity {
public:
	Vector2 position = {};
	Mole(float mole_position_x = 0, float mole_position_y = 0)
		: position{ mole_position_x, mole_position_y } {
	}
	void update() override {

	}

	void render() override {

		DrawEllipse(static_cast<int>(position.x) + 50, static_cast<int>(position.y) + 110, 55, 35, DARKGRAY);
		DrawEllipse(static_cast<int>(position.x) + 50, static_cast<int>(position.y) + 80, 50, 30, BROWN);
		DrawEllipse(static_cast<int>(position.x) + 50, static_cast<int>(position.y) + 60, 50, 30, BROWN);

		Rectangle rect = { position.x + 10, position.y + 40, 80.0f, 50.0f };
		DrawRectangleRounded(rect, 0.4f, 10, BROWN);

		DrawCircle(static_cast<int>(position.x) + 50, static_cast<int>(position.y) + 40, 30, DARKBROWN);
		DrawCircle(static_cast<int>(position.x) + 30, static_cast<int>(position.y) + 25, 15, BROWN);
		DrawCircle(static_cast<int>(position.x) + 70, static_cast<int>(position.y) + 25, 15, BROWN);
		DrawCircle(static_cast<int>(position.x) + 40, static_cast<int>(position.y) + 35, 10, WHITE);
		DrawCircle(static_cast<int>(position.x) + 60, static_cast<int>(position.y) + 35, 10, WHITE);
		DrawCircle(static_cast<int>(position.x) + 40, static_cast<int>(position.y) + 35, 5, BLACK);
		DrawCircle(static_cast<int>(position.x) + 60, static_cast<int>(position.y) + 35, 5, BLACK);
		DrawCircle(static_cast<int>(position.x) + 38, static_cast<int>(position.y) + 35, 3, BLACK);
		DrawCircle(static_cast<int>(position.x) + 62, static_cast<int>(position.y) + 35, 3, BLACK);
		DrawCircle(static_cast<int>(position.x) + 50, static_cast<int>(position.y) + 50, 10, RED);

		DrawLine(static_cast<int>(position.x) + 40, static_cast<int>(position.y) + 60, static_cast<int>(position.x) + 60, static_cast<int>(position.y) + 60, BLACK);
		DrawLine(static_cast<int>(position.x) + 45, static_cast<int>(position.y) + 65, static_cast<int>(position.x) + 55, static_cast<int>(position.y) + 65, BLACK);
		DrawLine(static_cast<int>(position.x) + 25, static_cast<int>(position.y) + 85, static_cast<int>(position.x) + 35, static_cast<int>(position.y) + 85, BLACK);
		DrawLine(static_cast<int>(position.x) + 65, static_cast<int>(position.y) + 85, static_cast<int>(position.x) + 75, static_cast<int>(position.y) + 85, BLACK);

		DrawEllipse(static_cast<int>(position.x) + 30, static_cast<int>(position.y) + 85, 15, 5, DARKBROWN);
		DrawEllipse(static_cast<int>(position.x) + 70, static_cast<int>(position.y) + 85, 15, 5, DARKBROWN);

	}
};

// Level Class
class Level {
private:
	std::list<Mole> all_moles;
	std::vector<Entity*> all_entities;

public:
	bool isHitting = false;

	void add_entity(const Mole& mole);
	void remove_all_dead_entities();
	void update();
	void render();
	void show_time();
	void draw_hammer();
	void display_score();
	bool is_mouse_over_mole(Vector2 position_of_mouse, Mole mole);
};

void Level::add_entity(const Mole& mole) {
	all_moles.push_back(mole);
	all_entities.push_back(&all_moles.back());
}

void Level::remove_all_dead_entities() {
	auto new_last_entity = std::remove_if(all_entities.begin(), all_entities.end(),
		[](const Entity* e) -> bool { return e->dead; });
	all_entities.erase(new_last_entity, all_entities.end());

	all_moles.remove_if([](const Mole& m) -> bool { return m.dead; });
}

void Level::update() {
	static float mole_spawn_time = 0.0f;
	float frame_time = GetFrameTime();
	mole_spawn_time += frame_time;

	if (mole_spawn_time >= 2.0f) {
		for (Mole& mole : all_moles) {
			mole.dead = true;
		}
		float random_x = random_float_in_range(20, 1080);
		float random_y = random_float_in_range(20, 600);
		Mole new_mole(random_x, random_y);
		add_entity(new_mole);

		mole_spawn_time = 0.0f;
	}

	if (is_button_pressed(Button::Left)) {
		isHitting = true;
		Vector2 mouse_position = { static_cast<float>(get_mouse_x()), static_cast<float>(get_mouse_y()) };

		for (Mole& mole : all_moles) {
			if (!mole.dead && is_mouse_over_mole(mouse_position, mole)) {
				mole.dead = true;
				SCORE++;
				break;
			}
		}
	}
	else {
		isHitting = false;
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

void Level::show_time() {
	float time = static_cast<float>(GetTime());
	std::string timeText = "Time: " + std::to_string(time);
	draw_text(20, 15, white, timeText);
}

void Level::draw_hammer() {

	int mouse_x = get_mouse_x();
	int mouse_y = get_mouse_y();

	int handle_width = 10;
	int handle_height = 50;

	int face_width = 30;
	int face_height = 15;

	Color handle_color = { 139, 69, 19, 255 };
	Color head_color = GRAY;

	int handle_y = isHitting ? mouse_y - 10 : mouse_y - handle_height - 20;
	int face_y = isHitting ? mouse_y - 10 : mouse_y - handle_height - face_height - 25;

	if (isHitting) {
		DrawRectanglePro(
			Rectangle{ (float)(mouse_x - handle_width / 2), (float)handle_y, (float)handle_width, (float)handle_height },
			Vector2{ 0.0f, (float)handle_height },
			45.0f,
			handle_color
		);

		DrawRectanglePro(
			Rectangle{ (float)(mouse_x - face_width / 2), (float)face_y, (float)face_width, (float)face_height },
			Vector2{ 0.0f, (float)face_height },
			45.0f,
			head_color
		);

		DrawCircle(mouse_x, face_y - 7, 7, head_color);
	}
	else {
		DrawRectangle(
			mouse_x - handle_width / 2, handle_y, handle_width, handle_height, handle_color
		);

		DrawRectangle(
			mouse_x - face_width / 2, handle_y - face_height, face_width, face_height, head_color
		);

		DrawCircle(mouse_x, handle_y - face_height / 2, 7, head_color);
	}
}

void Level::display_score() {
	std::string score = "Score: " + std::to_string(SCORE);
	draw_text(1100, 15, white, score);
}

bool Level::is_mouse_over_mole(Vector2 mouse_position, Mole mole) {
	Vector2 mole_center = { mole.position.x + 60, mole.position.y + 60 };
	float distance_x = mouse_position.x - mole_center.x;
	float distance_y = mouse_position.y - mole_center.y;

	return (distance_x * distance_x + distance_y * distance_y) < (60 * 60);
}


int main() {
	Level level;
	initialize(1280, 720, "Window");

	while (is_window_open()) {
		level.show_time();
		level.update();
		clear_window();
		level.render();
		level.draw_hammer();
		level.display_score();
		display();
	}

	return 0;
}
