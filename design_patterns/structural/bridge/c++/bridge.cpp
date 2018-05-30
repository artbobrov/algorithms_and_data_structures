#include <iostream>
#include <string>

class AbstractInterface {
public:
	virtual void functionality() = 0;
};

class OS {
public:
	virtual void extraFunctionality() = 0;
};

class Bridge: public AbstractInterface {
protected:
	OS *implementation;

public:
	explicit Bridge(OS *backend) {
		implementation = backend;
	}
};

class UseCase1: public Bridge {
public:
	explicit UseCase1(OS *backend)
		: Bridge(backend) {}

	void functionality() override {
		std::cout << "UseCase1 on ";
		implementation->extraFunctionality();
	}
};

class UseCase2: public Bridge {
public:
	explicit UseCase2(OS *backend)
		: Bridge(backend) {}

	void functionality() override {
		std::cout << "UseCase2 on ";
		implementation->extraFunctionality();
	}
};

struct Windows: public OS {
	void extraFunctionality() override {
		std::cout << "Windows!" << std::endl;
	}
};

struct Linux: public OS {
	void extraFunctionality() override {
		std::cout << "Linux!" << std::endl;
	}
};

int main() {
	AbstractInterface *useCase = nullptr;
	OS *windows = new Windows;
	OS *linux = new Linux;

	useCase = new UseCase1(windows);
	useCase->functionality();

	delete useCase;
	useCase = new UseCase1(linux);
	useCase->functionality();

	delete useCase;
	useCase = new UseCase2(windows);
	useCase->functionality();

	delete useCase;
	useCase = new UseCase2(linux);
	useCase->functionality();
	delete useCase;

	delete linux;
	delete windows;

	return 0;
}