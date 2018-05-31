#include <iostream>

class AbstractClass {
public:
	virtual void TemplateMethod() {
		primitiveOperation1();
		primitiveOperation2();
	}

	virtual ~AbstractClass() = default;
protected:
	virtual void primitiveOperation1() = 0;
	virtual void primitiveOperation2() = 0;
	AbstractClass() = default;
};

class ConcreteClass1: public AbstractClass {
public:
	void primitiveOperation1() override {
		std::cout << "ConcreteClass1::primitiveOperation1" << std::endl;
	}

	void primitiveOperation2() override {
		std::cout << "ConcreteClass1::primitiveOperation2" << std::endl;
	}
};

class ConcreteClass2: public AbstractClass {
public:
	void primitiveOperation1() override {
		std::cout << "ConcreteClass2::primitiveOperation1" << std::endl;
	}

	void primitiveOperation2() override {
		std::cout << "ConcreteClass2::primitiveOperation2" << std::endl;
	}
};

int main() {
	AbstractClass *p1 = new ConcreteClass1();
	AbstractClass *p2 = new ConcreteClass2();
	p1->TemplateMethod();
	p2->TemplateMethod();

	delete p1;
	delete p2;

	return 0;
}