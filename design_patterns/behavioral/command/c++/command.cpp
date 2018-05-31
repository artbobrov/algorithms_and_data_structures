#include <iostream>
#include <utility>

class DoorCommand {
public:
	virtual std::string execute() = 0;
};

class OpenCommand: public DoorCommand {
public:
	OpenCommand(std::string doors): doors(std::move(doors)) {}
private:
	std::string execute() override {
		return "Opened " + doors;
	}
private:

	std::string doors;
};

class CloseCommand: public DoorCommand {
public:
	CloseCommand(std::string doors): doors(std::move(doors)) {}

private:
	std::string execute() override {
		return "Closed " + doors;
	}
private:
	std::string doors;
};

class MacbookDoorOperations {
public:
	MacbookDoorOperations(const std::string &door)
		: door(door),
		  openCommand(new OpenCommand(door)),
		  closeCommand(new CloseCommand(door)) {}

	virtual ~MacbookDoorOperations() {
		delete openCommand;
		delete closeCommand;
	}

	std::string open() { return openCommand->execute(); }
	std::string close() { return closeCommand->execute(); }
private:
	std::string door;
	DoorCommand *openCommand;
	DoorCommand *closeCommand;
};

int main() {
	auto door = "front door";
	auto module = MacbookDoorOperations(door);

	std::cout << module.open() << std::endl;
	std::cout << module.close() << std::endl;
	return 0;
}