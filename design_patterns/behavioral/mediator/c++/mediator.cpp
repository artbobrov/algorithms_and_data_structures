#include <iostream>
#include <string>

class Person;

class Mediator {
public:
	virtual void send(const std::string &msg, Person *person) = 0;

	virtual ~Mediator() = default;

	void buildRelation(Person *lhs, Person *rhs) {
		mediatedA = lhs;
		mediatedB = rhs;
	}

protected:
	Person *mediatedA = nullptr;
	Person *mediatedB = nullptr;
};

class ConcreteMediator: public Mediator {
public:
	void send(const std::string &message, Person *person) override {
		if (person == mediatedA) {
			std::cout << message << "<-Mediator->PersonB" << std::endl;
		} else if (person == mediatedB) {
			std::cout << message << "<-Mediator->PersonA" << std::endl;
		}
	}
};

class Person {
public:
	explicit Person(Mediator *mediator): _mediator(mediator) {}
	virtual void sendMessage(const std::string &msg) = 0;
	virtual ~Person() = default;
protected:
	Mediator *_mediator;
};

class ConcretePersonA: public Person {
public:
	explicit ConcretePersonA(Mediator *md): Person(md) {}

	void sendMessage(const std::string &msg) override {
		_mediator->send(msg, this);
	}

};

class ConcretePersonB: public Person {
public:
	explicit ConcretePersonB(Mediator *mediator): Person(mediator) {}

	void sendMessage(const std::string &message) override {
		_mediator->send(message, this);
	}
};

int main() {
	Mediator *mediator = new ConcreteMediator();
	Person *personA = new ConcretePersonA(mediator);
	Person *personB = new ConcretePersonB(mediator);

	mediator->buildRelation(personA, personB);
	personA->sendMessage("PersonA");
	personB->sendMessage("PersonB");

	delete personA;
	delete personB;
	delete mediator;

	return 0;
}