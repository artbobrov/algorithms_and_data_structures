#include <iostream>

struct Rectangle {
	virtual void draw() = 0;
};

struct LegacyRectangle {
	LegacyRectangle(int x1, int y1, int x2, int y2) {
		_x1 = x1;
		_y1 = y1;
		_x2 = x2;
		_y2 = y2;
		std::cout << "LegacyRectangle(x1, y1, x2, y2)" << std::endl;
	}
	void oldDraw() {
		std::cout << "LegacyRectangle:  oldDraw()." << std::endl;
	}
private:
	int _x1;
	int _y1;
	int _x2;
	int _y2;
};

struct RectangleAdapter: public Rectangle, private LegacyRectangle {
	RectangleAdapter(int x, int y, int w, int h):
		LegacyRectangle(x, y, x + w, y + h) {
		std::cout << "RectangleAdapter(x, y, x + w, x + h)\n";
	}

	void draw() override {
		std::cout << "RectangleAdapter: draw().\n";
		oldDraw();
	}
};

int main() {
	int x = 20, y = 50, w = 300, h = 200;
	Rectangle *rectangle = new RectangleAdapter(x, y, w, h);
	rectangle->draw();

	delete rectangle;
	return 0;
}