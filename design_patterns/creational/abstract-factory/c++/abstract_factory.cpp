#include <iostream>
#include <string>

struct Castle {
	virtual std::string description() = 0;
};
struct King {
	virtual std::string description() = 0;
};
struct Army {
	virtual std::string description() = 0;
};

struct ElfCastle : Castle {
	std::string description() override {
		return "This is elf castle";;
	}
};
struct ElfKing : King {
	std::string description() override {
		return "This is elf king";;
	}
};
struct ElfArmy : Army {
	std::string description() override {
		return "This is elf army";;
	}
};

struct OrcCastle : Castle {
	std::string description() override {
		return "This is orc castle";
	}
};

struct OrcKing : King {
	std::string description() override {
		return "This is orc king";
	}
};

struct OrcArmy : Army {
	std::string description() override {
		return "This is orc army";
	}
};

class KingdomFactory {
public:
	virtual Castle *createCastle() = 0;
	virtual King *createKing() = 0;
	virtual Army *createArmy() = 0;
};

class OrcKingdomFactory : public KingdomFactory {
public:
	Castle *createCastle() override {
		return new OrcCastle();
	}
	King *createKing() override {
		return new OrcKing();
	}
	Army *createArmy() override {
		return new OrcArmy();
	}
};

class ElfKingdomFactory : public KingdomFactory {
public:
	Castle *createCastle() override {
		return new ElfCastle();
	}
	King *createKing() override {
		return new ElfKing();
	}
	Army *createArmy() override {
		return new ElfArmy();
	}
};

int main() {
	KingdomFactory *factory = new ElfKingdomFactory();
	//	KingdomFactory *factory = new OrcKingdomFactory();
	auto *castle = factory->createCastle();
	auto *king = factory->createKing();
	auto *army = factory->createArmy();
	std::cout << castle->description() << std::endl;
	std::cout << king->description() << std::endl;
	std::cout << army->description() << std::endl;
	delete castle;
	delete king;
	delete army;
	delete factory;
	return 0;
}