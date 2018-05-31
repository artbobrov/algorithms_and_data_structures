#include <iostream>
#include <string>

struct Currency {
	virtual std::string symbol() = 0;
	virtual std::string code() = 0;
};

enum class Country {
	Spain, France, USA
};

struct Euro: Currency {
	std::string symbol() override {
		return "€";
	}
	std::string code() override {
		return "EUR";
	}
};

struct Dollar: Currency {
	std::string symbol() override {
		return "$";
	}
	std::string code() override {
		return "USD";
	}
};

class CurrencyFactory {
public:
	Currency *createCurrency(Country country) {
		switch (country) {
			case Country::France:
			case Country::Spain: return new Euro;
			case Country::USA: return new Dollar;
		}
	}
};

int main() {
	auto factory = CurrencyFactory();
	auto *currency = factory.createCurrency(Country::France);
	std::cout << currency->code() << " – " << currency->symbol() << std::endl;
	delete currency;

	return 0;
}