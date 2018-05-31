#include <iostream>
#include <string>
class DoorOperator {
public:
	virtual std::string open(std::string door) = 0;
};

class Macbook: public DoorOperator {
public:
	std::string open(std::string door) override {
		return "Macbook: Affirmative. Opened " + door;
	}

};

class CurrentComputer: public DoorOperator {

public:
	virtual ~CurrentComputer() {
		delete computer;
	}

	bool authenticate(std::string password) {
		if (password != "pass")
			return false;
		computer = new Macbook();
		return true;
	}

	std::string open(std::string door) override {
		if (computer == nullptr)
			return "Access Denied. Try to authenticate";

		return computer->open(std::move(door));
	}
private:
	Macbook *computer;
};

int main() {
	auto computer = CurrentComputer();
	auto door = "front door";
	std::cout << computer.open(door) << std::endl;
	computer.authenticate("pass");
	std::cout << computer.open(door) << std::endl;;
	return 0;
}