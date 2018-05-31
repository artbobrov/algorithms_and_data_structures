#include <iostream>

class Context;

class State {
public:
	virtual bool isAuthorized(Context *context) = 0;
	virtual std::string userID(Context *context) = 0;
};

class UnauthorizedState;
class AuthorizedState;

class Context {
public:
	Context();
	~Context() { delete state; }

	bool isAuthorized() { return state->isAuthorized(this); };
	std::string userID() { return state->userID(this); }

	void changeStateToAuthorized(std::string userID);
	void changeStateToUnauthorized();

private:
	State *state;
};

class UnauthorizedState: public State {
public:
	bool isAuthorized(Context *context) override { return false; }
	std::string userID(Context *context) override { return ""; }
};

class AuthorizedState: public State {
public:
	explicit AuthorizedState(const std::string &userID): _userID(userID) {}

	bool isAuthorized(Context *context) override { return true; }
	std::string userID(Context *context) override { return _userID; }
private:
	std::string _userID;
};

void Context::changeStateToAuthorized(std::string userID) {
	delete state;
	state = new AuthorizedState(userID);
}
void Context::changeStateToUnauthorized() {
	delete state;
	state = new UnauthorizedState;
}
Context::Context(): state(new UnauthorizedState) {}

int main() {
	auto *userContext = new Context;
	std::cout << "isAuthorized: " << userContext->isAuthorized() << ", UserID: " << userContext->userID() << std::endl;
	userContext->changeStateToAuthorized("admin");
	std::cout << "isAuthorized: " << userContext->isAuthorized() << ", UserID: " << userContext->userID() << std::endl;
	userContext->changeStateToUnauthorized();
	std::cout << "isAuthorized: " << userContext->isAuthorized() << ", UserID: " << userContext->userID() << std::endl;
	delete userContext;
	return 0;
}