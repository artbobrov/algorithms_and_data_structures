#include <iostream>

class Handle {
public:
	virtual ~Handle() = default;

	virtual void handleRequest() = 0;

	Handle *successor() const {
		return _succ;
	}
protected:
	Handle(Handle *succ = nullptr): _succ(succ) {}
private:
	Handle *_succ;
};

class ConcreteHandleA: public Handle {
public:
	ConcreteHandleA(Handle *succ = nullptr): Handle(succ) {}

	void handleRequest() override {
		if (this->successor() != nullptr) {
			std::cout << "ConcreteHandleA Successor" << std::endl;
			successor()->handleRequest();
		} else {
			std::cout << "ConcreteHandleA handleRequest" << std::endl;
		}
	}
};

class ConcreteHandleB: public Handle {
public:
	ConcreteHandleB(Handle *succ = nullptr): Handle(succ) {}

	void handleRequest() override {
		if (this->successor() != nullptr) {
			std::cout << "ConcreteHandleB Successor" << std::endl;
			successor()->handleRequest();
		} else {
			std::cout << "ConcreteHandleB handleRequest" << std::endl;
		}
	}
};

int main() {
	Handle *handler1 = new ConcreteHandleA();
	Handle *handler2 = new ConcreteHandleB(handler1);
	handler2->handleRequest();

	delete handler1;
	delete handler2;

	return 0;
}