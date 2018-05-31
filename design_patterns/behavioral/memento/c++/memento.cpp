#include <iostream>
#include <string>

class Memento {
public:
	explicit Memento(int health): _health(health) {}

	int state() { return _health; }
private:
	int _health;
};

class Originator {
public:
	virtual Memento *memento() = 0;
	virtual void setMemento(Memento *memento) = 0;
};

class Player: public Originator {
public:
	explicit Player(int health = 100): _health(health) {}

	void getHurt(int hurt) {
		_health -= hurt;
	}

	void getCure(int cure) {
		_health += cure;
	}

	void printPulse() {
		if (_health > 70)
			std::cout << "Green" << std::endl;

		if (_health <= 70 && _health > 40)
			std::cout << "Yellow" << std::endl;

		if (_health <= 40)
			std::cout << "Red" << std::endl;
	}

	Memento *memento() override {
		return new Memento(_health);
	}
	void setMemento(Memento *memento) override {
		_health = memento->state();
	}
private:
	int _health;
};

class GameUtils {
private:
	Memento *_memento;

public:
	void saveState(Originator *originator) {
		_memento = originator->memento();

		std::cout << "Save state" << std::endl;
	}

	void loadState(Originator *originator) {
		originator->setMemento(_memento);

		std::cout << "Load State" << std::endl;
	}
};

int main() {
	auto *gameUtils = new GameUtils();
	auto *player = new Player();

	player->getHurt(20);
	player->getHurt(30);
	player->getHurt(20);
	player->printPulse();

	gameUtils->saveState(player);

	player->getCure(30);
	player->printPulse();

	gameUtils->loadState(player);

	player->printPulse();

	delete gameUtils;
	delete player;

	return 0;
}