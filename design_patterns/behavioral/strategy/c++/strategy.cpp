#include <iostream>
#include <string>
#include <algorithm>
class PrintStrategy {
public:
	virtual void print(std::string) = 0;
};
class Printer {
public:
	Printer(PrintStrategy *strategy): strategy(strategy) {}

	~Printer() { delete strategy; }

	void print(std::string message) {
		strategy->print(std::move(message));
	}
private:
	PrintStrategy *strategy;
};

class UpperCaseStrategy: public PrintStrategy {
public:
	void print(std::string message) override {
		std::transform(message.begin(), message.end(), message.begin(),
		               [](unsigned char c) { return std::toupper(c); });
		std::cout << message << std::endl;
	}
};

class LowerCaseStrategy: public PrintStrategy {
public:
	void print(std::string message) override {
		std::transform(message.begin(), message.end(), message.begin(),
		               [](unsigned char c) { return std::tolower(c); });
		std::cout << message << std::endl;
	}
};
int main() {
	auto lower = Printer(new LowerCaseStrategy());
	lower.print("HELLO world");
	auto upper = Printer(new UpperCaseStrategy);
	upper.print("some STUFF");
	return 0;
}