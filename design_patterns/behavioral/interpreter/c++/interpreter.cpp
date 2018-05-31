#include <iostream>
#include <string>

class Context {};

class Expression {
public:
	virtual ~Expression() = default;
	virtual void interpret(const Context &context) = 0;
};

class TerminalExpression: public Expression {
public:
	TerminalExpression(std::string statement): _statement(std::move(statement)) {}

	void interpret(const Context &context) override {
		std::cout << this->_statement << " -- TerminalExpression" << std::endl;
	}

private:
	std::string _statement;
};

class NonterminalExpression: public Expression {
public:
	NonterminalExpression(Expression *expression, int times): _expression(expression), _times(times) {}

	void interpret(const Context &context) override {
		for (int i = 0; i < _times; i++) {
			_expression->interpret(context);
		}
	}

private:
	Expression *_expression;
	int _times;
};

int main() {
	auto *context = new Context();
	Expression *terminalExpression = new TerminalExpression("echo");
	Expression *nonterminalExpression = new NonterminalExpression(terminalExpression, 2);
	nonterminalExpression->interpret(*context);

	delete nonterminalExpression;
	delete terminalExpression;
	delete context;

	return 0;
}