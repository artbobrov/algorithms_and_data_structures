#include <iostream>
#include <string>

enum class HairType {
	Bold, Short, Medium, Long
};

enum class HairColor {
	None = 0, Red, Brunette, Blonde
};

class HeroBuilder {
public:
	HeroBuilder(std::string name, int health) : name(std::move(name)), health(health) {}

	HeroBuilder &withHairColor(HairColor color) {
		this->hairColor = color;
		return *this;
	}
	HeroBuilder &withHairType(HairType hairType) {
		this->hairType = hairType;
		return *this;
	}
	HeroBuilder &withArmor(int armor) {
		this->armor = armor;
		return *this;
	}
	HeroBuilder &withMana(int mana) {
		this->armor = mana;
		return *this;
	}

	std::string name;
	HairColor hairColor = HairColor::None;
	HairType hairType = HairType::Bold;
	int armor = 0;
	int health = 0;
	int mana = 0;
};

struct Hero {
public:
	explicit Hero(const HeroBuilder &builder) : name(builder.name),
	                                            hairColor(builder.hairColor),
	                                            hairType(builder.hairType),
	                                            armor(builder.armor),
	                                            health(builder.health),
	                                            mana(builder.mana) {}
	friend std::ostream &operator<<(std::ostream &os, const Hero &hero) {
		os << "Hero name: " << hero.name << ", Armor: " << hero.armor << ", Health: " << hero.health << ", Mana: "
		   << hero.mana << ";";
		return os;
	}
private:
	std::string name;
	HairColor hairColor;
	HairType hairType;
	int armor;
	int health;
	int mana;
};

int main() {
	auto builder = HeroBuilder("Kevin", 100);
	builder.withArmor(10).withMana(10).withHairColor(HairColor::Brunette).withHairType(HairType::Medium);
	auto hero = Hero(builder);
	std::cout << hero << std::endl;

	return 0;
}