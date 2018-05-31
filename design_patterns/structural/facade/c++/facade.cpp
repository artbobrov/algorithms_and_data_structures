#include <iostream>

class SubSystem1 {
public:
	void operation() {
		std::cout << "SubSystem1 operation..." << std::endl;
	}
};

class SubSystem2 {
public:
	void operation() {
		std::cout << "SubSystem2 operation..." << std::endl;
	}
};

class Facade {
public:
	Facade() {
		_subSystem1 = new SubSystem1();
		_subSystem2 = new SubSystem2();
	}

	~Facade() {
		delete _subSystem1;
		delete _subSystem2;
	}

	void operationWrapper() {
		_subSystem1->operation();
		_subSystem2->operation();
	}
private:
	SubSystem1 *_subSystem1;
	SubSystem2 *_subSystem2;
};

int main() {
	auto *facade = new Facade;
	facade->operationWrapper();

	delete facade;
	return 0;
}