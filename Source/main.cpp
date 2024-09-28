#include"galba.h"

#include <vector>
#include <list>

// #------#
// | MATH |
// #------#

//constexpr float PI = 3.1415926535f;

struct Vector2
{
	float x;
	float y;
};

Vector2 random_direction()
{
	float angle = random_float_01() * 2 * PI;
	return Vector2{ cosf(angle), sinf(angle) };
}

// #----------#
// | ENTITIES |
// #----------#

class Entity
{
public:
	Vector2 position = {};
	bool dead = false;

	// TODO: Implement, update, render and more if necessary
};

// TODO: Goat class goes here!
struct Goat : public Entity {
	int goat_radius = 16;
	float goat_speed = 2.0f;
	Color goat_color = white;
};

//Goat goat;

// TODO: Bush class goes here!
struct Bush {
	/*int goat_radius = 16;
	float goat_speed = 2.0f;
	Color goat_color = white;*/
};

// #-----------#
// |   LEVEL   |
// #-----------#

class Level
{
	// Implement all functions outside this class.
private:
	std::list<Bush>  all_bushes   = {};
	std::list<Goat>     all_goats = {};

	//NOTE: Here ^^^ we must use std::list because when you push_back on a vector the memory might move and all the pointers become invalid

	std::vector<Entity*> all_entities = {};
public:

	// Careful, 'goat' and 'bush' are parameters and will be invalid at the end of the function (don't do &goat, &bush).

	void add_entity(const Goat& goat);
	// void add_entity(const Bush& bush); // TODO: Implement, add 'bush' to all_bushes and add a pointer to it to all_entities

	void remove_all_dead_entities();

	void init();
	void update();
	void render();
};

void Level::add_entity(const Goat& goat) {
	all_goats.push_back(goat);
	all_entities.push_back(&all_goats.back());
}

void Level::remove_all_dead_entities() {
	// We need to get rid of all the pointers that point to dead entities.
	auto new_last_entity = std::remove_if(all_entities.begin(), all_entities.end(), [](const Entity* e) -> bool { return e->dead; });
	all_entities.erase(new_last_entity, all_entities.end());

	// But we also need to get rid of all the actual entities that are dead.

	all_goats.remove_if([](const Goat& g) -> bool { return g.dead; });
	 //all_bushes.remove_if([](const Bush& b) -> bool { return b.dead; });
}

void Level::init()
{
	// TODO: Implement
}

void Level::update()
{
	// TODO: Implement (Hint: handle input here)

	for (Entity* entity : all_entities)
	{
		entity->update(); // Hint: Make sure you make this function (and render()) virtual in the base class.
	}

	remove_all_dead_entities();
}

void Level::render()
{
	// TODO: Implement

	for (Entity* entity : all_entities)
	{
		entity->render();
	}
}

int main(int argc, char** argv)
{
	// Don't edit the code inside this function!
	Level level;

	initialize(1280, 720, "Window");

	level.init();

	while (is_window_open())
	{
		level.update();

		clear_window();
		level.render();
		display();
	}
	return 0;
}