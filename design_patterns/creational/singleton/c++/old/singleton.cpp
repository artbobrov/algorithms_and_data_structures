#include <iostream>
#include <cassert>

class Singleton {
public:
	static Singleton *instance() {
		if (!_instance)
			_instance = new Singleton;
		return _instance;
	}
protected:
	Singleton() = default;
private:
	static Singleton *_instance;
};
Singleton *Singleton::_instance = nullptr;

int main() {
	auto *instance1 = Singleton::instance();
	auto *instance2 = Singleton::instance();
	std::cout << "instance1: " << instance1 << std::endl;
	std::cout << "instance2: " << instance2 << std::endl;
	assert(instance1 == instance2);
	return 0;
}