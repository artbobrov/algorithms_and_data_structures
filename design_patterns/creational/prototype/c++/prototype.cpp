#include <iostream>
#include <string>

struct Hero {
	std::string name;
	int health = 0;

	explicit Hero(std::string name): name(std::move(name)) {}

	Hero *clone() {
		return new Hero(name);
	}
	friend std::ostream &operator<<(std::ostream &os, const Hero &hero) {
		os << "Hero name: " << hero.name << ", Health: " << hero.health << ";";
		return os;
	}
};

int main() {
	Hero *hero = new Hero("Tom");
	hero->health = 100;
	std::cout << *hero << std::endl;
	Hero *new_tom = hero->clone();

	delete hero;
	new_tom->health = 50;
	std::cout << *new_tom << std::endl;

	delete new_tom;
	return 0;
}