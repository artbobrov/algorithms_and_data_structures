#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct Shape {
	virtual void draw(std::string fillColor) = 0;
};

struct Square: public Shape {
	void draw(std::string fillColor) override {
		std::cout << "Drawing a square with color " << fillColor << std::endl;
	}
};

struct Circle: public Shape {
	void draw(std::string fillColor) override {
		std::cout << "Drawing a circle with color " << fillColor << std::endl;
	}
};

class Canvas: public Shape {
public:
	Canvas(std::vector<Shape *> shapes): shapes(std::move(shapes)) {}

	virtual ~Canvas() {
		for (auto &shape: shapes)
			delete shape;
	}
	void draw(std::string fillColor) override {
		for (auto &shape: shapes)
			shape->draw(fillColor);
	}
private:
	std::vector<Shape *> shapes;
};

int main() {
	auto canvas = Canvas({new Circle, new Square});
	canvas.draw("red");

	return 0;
}