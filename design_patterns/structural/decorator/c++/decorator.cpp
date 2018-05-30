#include <iostream>

struct Coffee {
	virtual unsigned int cost() = 0;
	virtual std::string ingredients() = 0;
};

struct BlackCoffee: public Coffee {
	unsigned int cost() override {
		return 30;
	}
	std::string ingredients() override {
		return "water + coffee";
	}
};

class CoffeeDecorator: public Coffee {
public:
	CoffeeDecorator(Coffee *decoratedCoffe): decoratedCoffe(decoratedCoffe) {}

	virtual ~CoffeeDecorator() {
		delete decoratedCoffe;
	}

	unsigned int cost() override {
		return decoratedCoffe->cost();
	}
	std::string ingredients() override {
		return decoratedCoffe->ingredients();
	}
private:
	Coffee *decoratedCoffe;
};

class Milk: public CoffeeDecorator {
public:
	Milk(Coffee *decoratedCoffe): CoffeeDecorator(decoratedCoffe) {}
	unsigned int cost() override {
		return CoffeeDecorator::cost() + 20;
	}
	std::string ingredients() override {
		return CoffeeDecorator::ingredients() + " + milk";
	}
};

class Sugar: public CoffeeDecorator {
public:
	Sugar(Coffee *decoratedCoffe): CoffeeDecorator(decoratedCoffe) {}

private:
	unsigned int cost() override {
		return CoffeeDecorator::cost() + 10;
	}
	std::string ingredients() override {
		return CoffeeDecorator::ingredients() + " + sugar";
	}
};

int main() {
	Coffee *coffee = new BlackCoffee;

	coffee = new Milk(coffee);
	coffee = new Sugar(coffee);

	std::cout << "Cost: " << coffee->cost() << " Ingredients: " << coffee->ingredients() << std::endl;

	delete coffee;
	return 0;
}