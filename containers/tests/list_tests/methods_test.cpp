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
#include "../extra/profiler.hpp"
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
		std::uniform_int_distribution<size_t> uid(2, static_cast<int>(max_size));
		return uid(generator) - 1;
	}

	int get_value() {
		std::uniform_int_distribution<int> uid(0, 10);
		return uid(generator);
	}

	ctl::list<int> *get_random_list() {
		ctl::list<int> *lst = new ctl::list<int>();
		size_t size = get_size(1000);

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
		size_t size = get_size(10000);
		this->fill(size);
	}
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
	auto value = this->get_value();
	auto size = ctllst->size();
	auto offset = this->get_size(size);
	ctllst->emplace(iterator_with_offset(ctllst->begin(), offset), value);
	stdlst->emplace(iterator_with_offset(stdlst->begin(), offset), value);

	ASSERT_EQ(size + 1, ctllst->size());
	ASSERT_EQ(*ctllst, *stdlst);

	value = this->get_value();
	size = ctllst->size();
	offset = size;
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
	auto idx = this->get_size(ctllst->size());
	ctllst->erase(iterator_with_offset(ctllst->begin(), idx));
	stdlst->erase(iterator_with_offset(stdlst->begin(), idx));

	ASSERT_EQ(*stdlst, *ctllst);

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
	auto size_diff = get_size(1000);
	ctllst->fill(value, size_diff + ctllst->size());
	ASSERT_EQ(ctllst->subsequence(iterator_with_offset(ctllst->end(), -size_diff), ctllst->end()).count(value),
	          size_diff);
}

TEST_F(list_test_fixture, filter) {
	ctllst->filter([](auto &value) { return value % 2 == 0; });
	ASSERT_TRUE(ctllst->true_for_all([](auto &value) { return value % 2 == 0; }));
	ASSERT_EQ(ctllst->size(), ctllst->count([](auto &value) { return value % 2 == 0; }));
}

bool is_square(int value) {
	return sqrt(value) * sqrt(value) == value;
}

TEST_F(list_test_fixture, first) {
	const auto value = this->get_value();

	ASSERT_EQ(ctl::distance(stdlst->begin(), std::find(stdlst->begin(), stdlst->end(), value)),
	          ctl::distance(ctllst->begin(), ctllst->first(value)));

	auto cfirst = ctllst->first([&value](auto &element)->bool { return is_square(element); });
	auto
		sfirst = std::find_if(stdlst->begin(), stdlst->end(), [&value](auto &element) { return is_square(element); });

	if (cfirst == ctllst->end() && sfirst == stdlst->end())
		return;

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
TEST_F(list_test_fixture, merge) {
	std::list<int> list1 = {5, 9, 0, 1, 3};
	std::list<int> list2 = {8, 7, 2, 6, 4};

	ctl::list<int> clist1 = {5, 9, 0, 1, 3};
	ctl::list<int> clist2 = {8, 7, 2, 6, 4};

	list1.merge(list2);
	clist1.merge(std::move(clist2));
	ASSERT_EQ(clist1, list1);
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
	stdlst->pop_front();
	ASSERT_EQ(*ctllst, *stdlst);
	auto size = this->get_size(std::min(this->get_size(1000), ctllst->size()));
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

TEST_F(list_test_fixture, splice_1) {
	std::list<int> list1 = {1, 2, 3, 4, 5};
	std::list<int> list2 = {10, 20, 30, 40, 50};
	ctl::list<int> clist1 = {1, 2, 3, 4, 5};
	ctl::list<int> clist2 = {10, 20, 30, 40, 50};

	auto offset = this->get_size(list1.size());
	auto it = iterator_with_offset(list1.begin(), offset);

	auto cit = iterator_with_offset(clist1.begin(), offset);

	list1.splice(it, list2);
	clist1.splice(cit, std::move(clist2));

	ASSERT_EQ(list1, clist1);
}
TEST_F(list_test_fixture, splice_2) {
	std::list<int> list1 = {1, 2, 3, 4, 5};
	std::list<int> list2 = {10, 20, 30, 40, 50};
	ctl::list<int> clist1 = {1, 2, 3, 4, 5};
	ctl::list<int> clist2 = {10, 20, 30, 40, 50};

	auto offset = this->get_size(list1.size());
	auto max_value = list1.size() - offset;
	auto diff = std::min(this->get_size(max_value), max_value);

	auto it = iterator_with_offset(list1.begin(), offset);
	auto it_ = iterator_with_offset(list2.begin(), offset);
	auto it_1 = iterator_with_offset(list2.begin(), offset + diff);

	auto cit = iterator_with_offset(clist1.begin(), offset);
	auto cit_ = iterator_with_offset(clist2.begin(), offset);
	auto cit_1 = iterator_with_offset(clist2.begin(), offset + diff);

	list1.splice(it, list2, it_, it_1);
	clist1.splice(cit, std::move(clist2), cit_, cit_1);

	ASSERT_EQ(list1, clist1);
}
TEST_F(list_test_fixture, sort_1) {
	cout << endl;
	stdlst->sort();
	ctllst->sort();
	ASSERT_EQ(*stdlst, *ctllst);
}
TEST_F(list_test_fixture, sort_2) {
	auto foo = [](auto &a, auto &b)->bool { return a < b; };
	stdlst->sort(foo);
	ctllst->sort(foo);
	ASSERT_EQ(*stdlst, *ctllst);
}

TEST_F(list_test_fixture, true_for_all) {
	auto flag = ctllst->true_for_all([](auto &value) { return value > 0; });
	ASSERT_EQ(flag, ctllst->count([](auto &value) { return value > 0; }) == ctllst->size());
}

TEST_F(list_test_fixture, unique) {
	ctl::list<int> clst = {1, 2, 2, 3, 3, 2, 1, 1, 2};
	std::list<int> slst = {1, 2, 2, 3, 3, 2, 1, 1, 2};

	clst.unique();
	slst.unique();

	ASSERT_EQ(clst, slst);

	ctllst->unique();
	stdlst->unique();

	ASSERT_EQ(*ctllst, *stdlst);

}