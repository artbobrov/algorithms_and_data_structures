#include <iostream>
#include <vector>
class PlanetAlderaan;
class PlanetCoruscant;
class PlanetTatooine;
class MoonJedah;

class PlanetVisitor {
public:
	virtual void visit(PlanetAlderaan *) = 0;
	virtual void visit(PlanetCoruscant *) = 0;
	virtual void visit(PlanetTatooine *) = 0;
	virtual void visit(MoonJedah *) = 0;
};

class Planet {
public:
	virtual void accept(PlanetVisitor *visitor) = 0;
};

class MoonJedah: public Planet {
public:
	void accept(PlanetVisitor *visitor) override { visitor->visit(this); }
};

class PlanetAlderaan: public Planet {
public:
	void accept(PlanetVisitor *visitor) override { visitor->visit(this); }
};

class PlanetCoruscant: public Planet {
public:
	void accept(PlanetVisitor *visitor) override { visitor->visit(this); }
};

class PlanetTatooine: public Planet {
public:
	void accept(PlanetVisitor *visitor) override { visitor->visit(this); }
};

class NameVisitor: public PlanetVisitor {
public:
	void visit(PlanetAlderaan *alderaan) override {
		_name = "Alderaan";
	}
	void visit(PlanetCoruscant *coruscant) override {
		_name = "Coruscant";
	}
	void visit(PlanetTatooine *tatooine) override {
		_name = "Tatooine";
	}
	void visit(MoonJedah *jedah) override {
		_name = "Jedah";
	}
	std::string name() const { return _name; }
private:
	std::string _name = "";
};

int main() {
	std::vector<Planet *> planets = {new MoonJedah, new PlanetTatooine, new PlanetCoruscant, new PlanetAlderaan};

	for (auto &planet: planets) {
		auto visitor = NameVisitor();
		planet->accept(&visitor);
		std::cout << "Visitor name: " << visitor.name() << std::endl;

	}
	for (auto &planet: planets)
		delete planet;
	return 0;
}