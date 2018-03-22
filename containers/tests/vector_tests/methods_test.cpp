//
// Created by Artem Bobrov on 14.01.2018.
//

#include "gtest/gtest.h"
#include "vector.hpp"

#include <ctime>
#include <vector>
#include <random>
#include <algorithm>
#include <ostream>
#include "../extra/generator.hpp"



std::default_random_engine gen(static_cast<unsigned int>(time(0)));

class vector_test_fixture : public ::testing::Test {
public:
	void fill(size_t size) {
		std::uniform_int_distribution<> uid(0, 10);
		for (int i = 0; i < size; i++) {
			auto value = static_cast<int>(uid(gen));
			stdvec->push_back(value);
			ctlvec->push_back(value);
		}
	}

	size_t get_size(size_t max_size) {
		std::uniform_int_distribution<size_t> uid(1, static_cast<int>(max_size));
		return uid(gen) - 1;
	}

	int get_value() {
		std::uniform_int_distribution<int> uid(0, 10);
		return uid(gen);
	}

	ctl::vector<int> *get_random_vector() {
		ctl::vector<int> *vec = new ctl::vector<int>();
		size_t size = get_size(100);

		for (int i = 0; i < size; ++i) {
			auto value = get_value();
			vec->push_back(value);
		}
		return vec;
	}
protected:
	virtual void TearDown() final {
		delete stdvec;
		delete ctlvec;
	}
	void SetUp() override {
		Test::SetUp();
		gen = std::default_random_engine(time(0));
		stdvec = new std::vector<int>();
		ctlvec = new ctl::vector<int>();
		size_t size = get_size(1000000);
		this->fill(size);
	}
private:

public:

	std::vector<int> *stdvec;
	ctl::vector<int> *ctlvec;
};
TEST_F(vector_test_fixture, compare_operators) {
	ctl::vector<int> c = {1, 2, 3};
	std::vector<int> s = {1, 3, 3};
	ASSERT_LT(c, s);
	ASSERT_GT(s, c);
	ctl::vector<int> c1 = {1, 2, 3, 4};

	ASSERT_LT(c, c1);
}

TEST_F(vector_test_fixture, append_subsequence) {
	ctl::vector<int> origin = ctl::vector<int>(*ctlvec);
	ASSERT_EQ(origin, *ctlvec);
	auto *rnd_vec = this->get_random_vector();

	ctlvec->append(*rnd_vec);

	ASSERT_GE(ctlvec->capacity(), ctlvec->size());
	ASSERT_EQ(origin.size() + rnd_vec->size(), ctlvec->size());
	ASSERT_EQ(origin, ctlvec->subsequence(0, origin.size()));
	ASSERT_EQ(origin.size(), ctlvec->subsequence(0, origin.size()).size());
	ASSERT_EQ(*rnd_vec, ctlvec->subsequence(origin.size(), ctlvec->size()));
	ASSERT_EQ(rnd_vec->size(), ctlvec->subsequence(origin.size(), ctlvec->size()).size());
	delete rnd_vec;
}

TEST_F(vector_test_fixture, contains) {
	auto value = this->get_value();
	ASSERT_EQ(ctlvec->contains(value), std::find(stdvec->begin(), stdvec->end(), value) != stdvec->end());

	bool result = ctlvec->contains([](const auto &value)->bool {
		return value % 2 == 0;
	}); // out of assertion because "Lambda-expressions are not allowed in unevaluated expressions"

	ASSERT_EQ(result,
	          std::find_if(stdvec->begin(), stdvec->end(), [](auto value)->bool { return value % 2 == 0; })
		          != stdvec->end());
}

TEST_F(vector_test_fixture, count) {
	auto value = this->get_value();
	ASSERT_EQ(ctlvec->count(value), std::count(stdvec->begin(), stdvec->end(), value));

	auto result = ctlvec->count([](const auto &value)->bool {
		return value % 2 != 0;
	}); // out of assertion because "Lambda-expressions are not allowed in unevaluated expressions"

	ASSERT_EQ(result, std::count_if(stdvec->begin(), stdvec->end(), [](int value) { return value % 2 != 0; }));
}

TEST_F(vector_test_fixture, emplace) {
	auto value = get_value();
	auto size = ctlvec->size();
	auto offset = this->get_size(size);
	ctlvec->emplace(offset, value);
	stdvec->emplace(stdvec->begin() + offset, value);

	ASSERT_GE(ctlvec->capacity(), ctlvec->size());
	ASSERT_EQ(size + 1, ctlvec->size());
	ASSERT_EQ(*ctlvec, *stdvec);
}
TEST_F(vector_test_fixture, emplace_back) {
	auto value = get_value();
	auto size = ctlvec->size();
	ctlvec->emplace_back(value);
	stdvec->emplace_back(value);

	ASSERT_GE(ctlvec->capacity(), ctlvec->size());
	ASSERT_EQ(size + 1, ctlvec->size());
	ASSERT_EQ(*ctlvec, *stdvec);
}

TEST_F(vector_test_fixture, erase) {
	ASSERT_EQ(*ctlvec, *stdvec);
	auto idx = this->get_size(ctlvec->size());
	ctlvec->erase(idx);
	stdvec->erase(stdvec->begin() + idx);
	ASSERT_EQ(*ctlvec, *stdvec);
	idx = this->get_size(ctlvec->size());
	auto difference = this->get_size(ctlvec->size() - idx);
	ctlvec->erase(idx, idx + difference);
	stdvec->erase(stdvec->begin() + idx, stdvec->begin() + idx + difference);
	ASSERT_EQ(*ctlvec, *stdvec);
}

TEST_F(vector_test_fixture, fill) {
	auto value = get_value();
	ctlvec->fill(value);
	ASSERT_EQ(ctlvec->count(value), ctlvec->size());
	value = get_value();
	auto idx = get_size(ctlvec->size());
	auto diff = ctlvec->size() - idx;
	ctlvec->fill(idx, idx + diff, value);
	ASSERT_EQ(ctlvec->subsequence(idx, idx + diff).count(value), diff);
	value = get_value();
	auto size_diff = get_size(100);
	ctlvec->fill(value, size_diff + ctlvec->size());
	ASSERT_EQ(ctlvec->subsequence(ctlvec->end() - size_diff, ctlvec->end()).count(value), size_diff);
}

TEST_F(vector_test_fixture, filter) {
	ctlvec->filter([](auto &value) { return value % 2 == 0; });

	ASSERT_TRUE(ctlvec->true_for_all([](auto &value) { return value % 2 == 0; }));
	ASSERT_EQ(ctlvec->size(), ctlvec->count([](auto &value) { return value % 2 == 0; }));
}

TEST_F(vector_test_fixture, first) {
	const auto value = this->get_value() + 1;

	ASSERT_EQ(std::find(stdvec->begin(), stdvec->end(), value) - stdvec->begin(),
	          ctlvec->first(value) - ctlvec->begin());
	auto cfirst = ctlvec->first([&value](auto &element)->bool { return element % value == 0; });
	auto
		sfirst = std::find_if(stdvec->begin(), stdvec->end(), [&value](auto &element) { return element % value == 0; });

	ASSERT_EQ(*cfirst, *sfirst);
	ASSERT_EQ(sfirst - stdvec->begin(), cfirst - ctlvec->begin());

}

TEST_F(vector_test_fixture, for_each) {
	size_t csize = 0;
	size_t ssize = 0;
	std::for_each(stdvec->begin(), stdvec->end(), [&ssize](int &element) {
		++ssize;
	});
	ctlvec->for_each([&csize](int &element) {
		++csize;
	});

	ASSERT_EQ(csize, ssize);
}

TEST_F(vector_test_fixture, index_of) {
	const auto value = this->get_value();
	ASSERT_EQ(ctlvec->index_of(value), std::find(stdvec->begin(), stdvec->end(), value) - stdvec->begin());
}

TEST_F(vector_test_fixture, insert) {
	for (int i = 0; i < 5; ++i) {
		auto value = this->get_value();
		auto offset = this->get_size(ctlvec->size());

		auto cit = ctlvec->insert(offset, value);
		auto sit = stdvec->insert(stdvec->begin() + offset, value);

		ASSERT_EQ(*stdvec, *ctlvec);
		ASSERT_EQ(*cit, *sit);
		ASSERT_EQ(cit - ctlvec->begin(), sit - stdvec->begin());
	}

}

TEST_F(vector_test_fixture, min_max) {
	ASSERT_EQ(*ctlvec->max(), *std::max_element(ctlvec->begin(), ctlvec->end()));
	ASSERT_EQ(*ctlvec->min(), *std::min_element(ctlvec->begin(), ctlvec->end()));

	auto c_result = *ctlvec->max([](auto &lhs, auto &rhs) {
		return std::sin(lhs) < std::sin(rhs);
	}); // out of assertion because "Lambda-expressions are not allowed in unevaluated expressions"
	auto s_result = *std::max_element(ctlvec->begin(), ctlvec->end(), [](auto &lhs, auto &rhs) {
		return std::sin(lhs) < std::sin(rhs);
	}); // out of assertion because "Lambda-expressions are not allowed in unevaluated expressions"

	ASSERT_EQ(c_result, s_result);
}

TEST_F(vector_test_fixture, pop_back) {
	ctlvec->pop_back();
	stdvec->pop_back();
	ASSERT_EQ(*ctlvec, *stdvec);
	auto size = this->get_size(ctlvec->size());
	ctlvec->pop_back(size);
	for (int i = 0; i < size; i++) { stdvec->pop_back(); }
	ASSERT_EQ(*ctlvec, *stdvec);
}

TEST_F(vector_test_fixture, pop_front) {
	ctlvec->pop_front();
	stdvec->erase(stdvec->begin());
	ASSERT_EQ(*ctlvec, *stdvec);
	auto size = this->get_size(std::min(this->get_size(100), ctlvec->size()));
	ctlvec->pop_front(size);
	for (int i = 0; i < size; i++) { stdvec->erase(stdvec->begin()); }
	ASSERT_EQ(*ctlvec, *stdvec);
}

TEST_F(vector_test_fixture, push_back) {
	auto size = this->get_size(ctlvec->size());
	auto value = this->get_value();
	ctlvec->push_back(size, value);
	for (int i = 0; i < size; i++) { stdvec->push_back(value); }
	ASSERT_EQ(*ctlvec, *stdvec);
}

TEST_F(vector_test_fixture, push_front) {
	auto size = this->get_size(std::min(this->get_size(1000), ctlvec->size()));
	auto value = -this->get_value();
	ctlvec->push_front(size, value);
	for (int i = 0; i < size; i++) { stdvec->insert(stdvec->begin(), value); }
	ASSERT_EQ(*ctlvec, *stdvec);
}

TEST_F(vector_test_fixture, remove) {
	ctlvec->remove_all([](auto &value) {
		return value % 2 == 0;
	});

	auto cnt = ctlvec->count([](auto &value) { return value % 2 == 0; });

	ASSERT_EQ(cnt, 0);
}

TEST_F(vector_test_fixture, reserve) {
	auto size_diff = this->get_size(100);
	auto new_capacity = ctlvec->capacity() + size_diff;
	ctlvec->reserve(new_capacity);
	ASSERT_EQ(new_capacity, ctlvec->capacity());
}

TEST_F(vector_test_fixture, reverse) {
	ASSERT_EQ(ctl::vector<int>(ctlvec->rbegin(), ctlvec->rend()), ctlvec->reversed());
}

TEST_F(vector_test_fixture, shrink_to_fit) {
	ctlvec->shrink_to_fit();
	ASSERT_EQ(ctlvec->size(), ctlvec->capacity());
	ASSERT_EQ(*ctlvec, *stdvec);
}
TEST_F(vector_test_fixture, true_for_all) {
	auto flag = ctlvec->true_for_all([](auto &value) { return value > 0; });
	ASSERT_EQ(flag, ctlvec->count([](auto &value) { return value > 0; }) == ctlvec->size());
}