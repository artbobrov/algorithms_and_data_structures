#include <iostream>
#include <string>
#include <list>
#include <utility>

template<class T>
class Observer {
public:
	virtual void handleEvent(const T &) = 0;
};

class SupervisedObject {
public:
	void add(Observer<SupervisedObject> &observer) {
		_observers.push_back(&observer);
	}

	void remove(Observer<SupervisedObject> &observer) {
		_observers.remove(&observer);
	}

	const std::string &get() const {
		return _string;
	}

	void set(std::string str) {
		_string = std::move(str);
		_notify();
	}
private:
	std::string _string = "";
	std::list<Observer<SupervisedObject> *> _observers{};

	void _notify() {
		for (auto &_observer : _observers) {
			_observer->handleEvent(*this);
		}
	}
};

class Reflector: public Observer<SupervisedObject> {
public:
	void handleEvent(const SupervisedObject &object) override {
		std::cout << "Value: " << object.get() << std::endl;
	}
};

class Counter: public Observer<SupervisedObject> {
public:
	void handleEvent(const SupervisedObject &object) override {
		std::cout << "Length: " << object.get().length() << std::endl;
	}
};

int main() {
	SupervisedObject supervisedObject;
	Reflector observerA;
	Counter observerB;

	supervisedObject.add(observerA);
	supervisedObject.set("Hello, World!");
	std::cout << std::endl;

	supervisedObject.remove(observerA);
	supervisedObject.add(observerB);
	supervisedObject.set("World, Hello!");
	std::cout << std::endl;

	return 0;
}