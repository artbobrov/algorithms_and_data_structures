//
// Created by Artem Bobrov on 21.03.2018.
//

#include "gtest/gtest.h"
#include "list.hpp"

#include <ctime>
#include <list>
#include <random>
#include <algorithm>
#include <ostream>
#include "../extra/generator.hpp"

std::default_random_engine generator(static_cast<unsigned int>(time(0)));

template<class Iterator>
Iterator iterator_with_offset(Iterator iter, long long offset) {
	if (offset > 0)
		for (; offset > 0; --offset, ++iter) {}
	if (offset < 0)
		for (; offset < 0; ++offset, --iter) {}
	return iter;

}

class list_test_fixture : public ::testing::Test {
public:
	void fill(size_t size) {
		std::uniform_int_distribution<> uid(0, 10);
		for (int i = 0; i < size; i++) {
			auto value = static_cast<int>(uid(generator));
			stdlst->push_back(value);
			ctllst->push_back(value);
		}
	}

	size_t get_size(size_t max_size) {
		std::uniform_int_distribution<size_t> uid(1, static_cast<int>(max_size));
		return uid(generator) - 1;
	}

	int get_value() {
		std::uniform_int_distribution<int> uid(0, 10);
		return uid(generator);
	}

	ctl::list<int> *get_random_list() {
		ctl::list<int> *lst = new ctl::list<int>();
		size_t size = get_size(10);

		for (int i = 0; i < size; ++i) {
			auto value = get_value();
			lst->push_back(value);
		}
		return lst;
	}
protected:
	virtual void TearDown() final {
		delete stdlst;
		delete ctllst;
	}
	void SetUp() override {
		Test::SetUp();
		generator = std::default_random_engine(time(0));
		stdlst = new std::list<int>();
		ctllst = new ctl::list<int>();
		size_t size = get_size(1000000);
		this->fill(size);
	}
private:

public:

	std::list<int> *stdlst;
	ctl::list<int> *ctllst;
};
TEST_F(list_test_fixture, compare_operators) {
	ctl::list<int> c = {1, 2, 3};
	std::list<int> s = {1, 3, 3};
	ASSERT_LT(c, s);
	ASSERT_GT(s, c);
	ctl::list<int> c1 = {1, 2, 3, 4};

	ASSERT_LT(c, c1);
}

TEST_F(list_test_fixture, append_subsequence) {
	ctl::list<int> origin = ctl::list<int>(*ctllst);
	ASSERT_EQ(origin, *ctllst);
	auto *rnd_lst = this->get_random_list();

	ctllst->append(*rnd_lst);

	ASSERT_EQ(origin.size() + rnd_lst->size(), ctllst->size());
	ASSERT_EQ(origin, ctllst->subsequence(ctllst->begin(), iterator_with_offset(ctllst->begin(), origin.size())));
	ASSERT_EQ(*rnd_lst,
	          ctllst->subsequence(iterator_with_offset(ctllst->begin(), origin.size()),
	                              iterator_with_offset(ctllst->begin(), ctllst->size())));
	delete rnd_lst;
}

TEST_F(list_test_fixture, contains) {
	auto value = this->get_value();
	ASSERT_EQ(ctllst->contains(value), std::find(stdlst->begin(), stdlst->end(), value) != stdlst->end());

	bool result = ctllst->contains([](const auto &value)->bool {
		return value % 2 == 0;
	}); // out of assertion because "Lambda-expressions are not allowed in unevaluated expressions"

	ASSERT_EQ(result,
	          std::find_if(stdlst->begin(), stdlst->end(), [](auto value)->bool { return value % 2 == 0; })
		          != stdlst->end());
}

TEST_F(list_test_fixture, count) {
	auto value = this->get_value();
	ASSERT_EQ(ctllst->count(value), std::count(stdlst->begin(), stdlst->end(), value));

	auto result = ctllst->count([](const auto &value)->bool {
		return value % 2 != 0;
	}); // out of assertion because "Lambda-expressions are not allowed in unevaluated expressions"

	ASSERT_EQ(result, std::count_if(stdlst->begin(), stdlst->end(), [](int value) { return value % 2 != 0; }));
}

TEST_F(list_test_fixture, emplace) {
	auto value = get_value();
	auto size = ctllst->size();
	auto offset = this->get_size(size);
	ctllst->emplace(iterator_with_offset(ctllst->begin(), offset), value);
	stdlst->emplace(iterator_with_offset(stdlst->begin(), offset), value);

	ASSERT_EQ(size + 1, ctllst->size());
	ASSERT_EQ(*ctllst, *stdlst);
}
TEST_F(list_test_fixture, emplace_back) {
	auto value = get_value();
	auto size = ctllst->size();
	ctllst->emplace_back(value);
	stdlst->emplace_back(value);

	ASSERT_EQ(size + 1, ctllst->size());
	ASSERT_EQ(*ctllst, *stdlst);
}

TEST_F(list_test_fixture, erase) {
	ASSERT_EQ(*ctllst, *stdlst);
	auto idx = this->get_size(ctllst->size());
	ctllst->erase(iterator_with_offset(ctllst->begin(), idx));
	stdlst->erase(iterator_with_offset(stdlst->begin(), idx));
	ASSERT_EQ(*ctllst, *stdlst);
	idx = this->get_size(ctllst->size());
	auto difference = this->get_size(ctllst->size() - idx);
	ctllst->erase(iterator_with_offset(ctllst->begin(), idx),
	              iterator_with_offset(ctllst->begin(), idx + difference));
	stdlst->erase(iterator_with_offset(stdlst->begin(), idx), iterator_with_offset(stdlst->begin(), idx + difference));
	ASSERT_EQ(*ctllst, *stdlst);
}

TEST_F(list_test_fixture, fill) {
	auto value = get_value();
	ctllst->fill(value);
	ASSERT_EQ(ctllst->count(value), ctllst->size());
	value = get_value();
	auto idx = get_size(ctllst->size());
	auto diff = ctllst->size() - idx;
	ctllst->fill(iterator_with_offset(ctllst->begin(), idx),
	             iterator_with_offset(ctllst->begin(), idx + diff), value);
	ASSERT_EQ(ctllst->subsequence(iterator_with_offset(ctllst->begin(), idx),
	                              iterator_with_offset(ctllst->begin(), idx + diff)).count(value), diff);
	value = get_value();
	auto size_diff = get_size(100);
	ctllst->fill(value, size_diff + ctllst->size());
	ASSERT_EQ(ctllst->subsequence(iterator_with_offset(ctllst->end(), -size_diff), ctllst->end()).count(value),
	          size_diff);
}

TEST_F(list_test_fixture, filter) {
	ctllst->filter([](auto &value) { return value % 2 == 0; });

	ASSERT_TRUE(ctllst->true_for_all([](auto &value) { return value % 2 == 0; }));
	ASSERT_EQ(ctllst->size(), ctllst->count([](auto &value) { return value % 2 == 0; }));
}

TEST_F(list_test_fixture, first) {
	const auto value = this->get_value() + 1;

	ASSERT_EQ(ctl::distance(stdlst->begin(), std::find(stdlst->begin(), stdlst->end(), value)),
	          ctl::distance(ctllst->begin(), ctllst->first(value)));
	auto cfirst = ctllst->first([&value](auto &element)->bool { return element % value == 0; });
	auto
		sfirst = std::find_if(stdlst->begin(), stdlst->end(), [&value](auto &element) { return element % value == 0; });

	ASSERT_EQ(*cfirst, *sfirst);
	ASSERT_EQ(std::distance(stdlst->begin(), sfirst), std::distance(ctllst->begin(), cfirst));

}

TEST_F(list_test_fixture, for_each) {
	size_t csize = 0;
	size_t ssize = 0;
	std::for_each(stdlst->begin(), stdlst->end(), [&ssize](int &element) {
		++ssize;
	});
	ctllst->for_each([&csize](int &element) {
		++csize;
	});

	ASSERT_EQ(csize, ssize);
}

TEST_F(list_test_fixture, insert) {
	for (int i = 0; i < 5; ++i) {
		auto value = this->get_value();
		auto offset = this->get_size(ctllst->size());

		auto cit = ctllst->insert(iterator_with_offset(ctllst->begin(), offset), value);
		auto sit = stdlst->insert(iterator_with_offset(stdlst->begin(), offset), value);

		ASSERT_EQ(*stdlst, *ctllst);
		ASSERT_EQ(*cit, *sit);

		ASSERT_EQ(std::distance(ctllst->begin(), cit), std::distance(stdlst->begin(), sit));
	}

}

TEST_F(list_test_fixture, min_max) {
	ASSERT_EQ(*ctllst->max(), *std::max_element(ctllst->begin(), ctllst->end()));
	ASSERT_EQ(*ctllst->min(), *std::min_element(ctllst->begin(), ctllst->end()));

	auto c_result = *ctllst->max([](auto &lhs, auto &rhs) {
		return std::sin(lhs) < std::sin(rhs);
	}); // out of assertion because "Lambda-expressions are not allowed in unevaluated expressions"
	auto s_result = *std::max_element(ctllst->begin(), ctllst->end(), [](auto &lhs, auto &rhs) {
		return std::sin(lhs) < std::sin(rhs);
	}); // out of assertion because "Lambda-expressions are not allowed in unevaluated expressions"

	ASSERT_EQ(c_result, s_result);
}

TEST_F(list_test_fixture, pop_back) {
	ctllst->pop_back();
	stdlst->pop_back();
	ASSERT_EQ(*ctllst, *stdlst);
	auto size = this->get_size(ctllst->size());
	ctllst->pop_back(size);
	for (int i = 0; i < size; i++) { stdlst->pop_back(); }
	ASSERT_EQ(*ctllst, *stdlst);
}

TEST_F(list_test_fixture, pop_front) {
	ctllst->pop_front();
	stdlst->erase(stdlst->begin());
	ASSERT_EQ(*ctllst, *stdlst);
	auto size = this->get_size(std::min(this->get_size(100), ctllst->size()));
	ctllst->pop_front(size);
	for (int i = 0; i < size; i++) { stdlst->erase(stdlst->begin()); }
	ASSERT_EQ(*ctllst, *stdlst);
}

TEST_F(list_test_fixture, push_back) {
	auto size = this->get_size(ctllst->size());
	auto value = this->get_value();
	ctllst->push_back(size, value);
	for (int i = 0; i < size; i++) { stdlst->push_back(value); }
	ASSERT_EQ(*ctllst, *stdlst);
}

TEST_F(list_test_fixture, push_front) {
	auto size = this->get_size(std::min(this->get_size(1000), ctllst->size()));
	auto value = -this->get_value();
	ctllst->push_front(size, value);
	for (int i = 0; i < size; i++) { stdlst->insert(stdlst->begin(), value); }
	ASSERT_EQ(*ctllst, *stdlst);
}

TEST_F(list_test_fixture, remove) {
	ctllst->remove_all([](auto &value) {
		return value % 2 == 0;
	});

	auto cnt = ctllst->count([](auto &value) { return value % 2 == 0; });

	ASSERT_EQ(cnt, 0);
}

TEST_F(list_test_fixture, true_for_all) {
	auto flag = ctllst->true_for_all([](auto &value) { return value > 0; });
	ASSERT_EQ(flag, ctllst->count([](auto &value) { return value > 0; }) == ctllst->size());
}