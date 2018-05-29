#include <iostream>
#include <cassert>

class Singleton {
public:
	static Singleton &instance() {
		static Singleton _instance;
		return _instance;
	}
private:
	Singleton() = default;
	Singleton(const Singleton &another) = default;
};

int main() {
	auto &instance1 = Singleton::instance();
	auto &instance2 = Singleton::instance();
	std::cout << "instance1: " << &instance1 << std::endl;
	std::cout << "instance2: " << &instance2 << std::endl;
	assert(&instance1 == &instance2);
	return 0;
}