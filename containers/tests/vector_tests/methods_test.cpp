//
// Created by Artem Bobrov on 14.01.2018.
//

#include "gtest/gtest.h"
#include "vector.hpp"
#include "../extra/extra_std_functions.hpp"

#include <ctime>
#include <vector>
#include <random>

#include <thread>
using namespace std::chrono_literals;
using std::cout;
using std::endl;

class vector_test_fixture : public ::testing::Test {
public:
	~vector_test_fixture() override {
		delete stdvec;
		delete ctlvec;
	}

protected:
	void SetUp() override {
		Test::SetUp();
		stdvec = new std::vector<int>();
		ctlvec = new ctl::vector<int>();
	}
public:
	std::vector<int> *stdvec;
	ctl::vector<int> *ctlvec;
};

TEST_F(vector_test_fixture, testst) {
	ASSERT_EQ(*ctlvec, *stdvec);
}
