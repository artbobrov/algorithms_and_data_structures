#include <iostream>
#include <map>
#include <utility>
#include <string>

struct SpecialityCoffee {
	SpecialityCoffee(std::string origin): origin(std::move(origin)) {}

	virtual std::string description() { return origin; };
private:
	std::string origin;
};

class Menu {
public:

	SpecialityCoffee *lookup(std::string origin) {
		if (auto search = coffeeAvailable.find(origin);search == coffeeAvailable.end())
			coffeeAvailable[origin] = new SpecialityCoffee(origin);

		return coffeeAvailable[origin];
	}
private:
	std::map<std::string, SpecialityCoffee *> coffeeAvailable;
};

class CoffeeShop {
public:

	virtual ~CoffeeShop() {
		for (auto &pair: orders)
			delete pair.second;
	}

	void takeOrder(std::string origin, int table) {
		orders[table] = menu.lookup(std::move(origin));
	}

	void serve() {
		for (auto &pair: orders) {
			std::cout << "Serving " << pair.first << " to table " << pair.second->description() << std::endl;
		}
	}
private:
	Menu menu;
	std::map<int, SpecialityCoffee *> orders;
};

int main() {
	auto coffeshop = CoffeeShop();
	coffeshop.takeOrder("Latte", 1);
	coffeshop.takeOrder("Black coffe", 3);
	coffeshop.serve();
	return 0;
}