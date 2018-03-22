//
// Created by Artem Bobrov on 17.03.2018.
//

#include "gtest/gtest.h"
#include "list.hpp"
#include <list>
#include "../extra/generator.hpp"
#include "../extra/extra_functions.hpp"

#include <thread>
#include <random>
generator list_generator(static_cast<unsigned int>(time(0)));

TEST(list_contructor_test_suite, default) {
	std::list<int> slst;
	ctl::list<int> clst;
	ASSERT_EQ(clst, slst);
}

TEST(list_contructor_test_suite, default_value) {
	size_t size = list_generator.get_size();
	int value = list_generator.get_value();
	std::list<int> slst(size, value);
	ctl::list<int> clst(size, value);
	ASSERT_EQ(clst, slst);
}
TEST(list_contructor_test_suite, template) {
	size_t size = list_generator.get_size();

	std::vector<int> std_tmp_vector(size);
	std::list<int> std_tmp_list;

	for (size_t i = 0; i < size; ++i) {
		int value = list_generator.get_value();
		std_tmp_vector[i] = value;
		std_tmp_list.push_back(value);
	}

	ctl::list<int> clst_v(std_tmp_vector.begin(), std_tmp_vector.end());
	ASSERT_EQ(clst_v, std_tmp_list);
	ASSERT_EQ(clst_v, std_tmp_vector);

	ctl::list<int> clst_l(std_tmp_list.begin(), std_tmp_list.end());
	ASSERT_EQ(clst_l, std_tmp_list);
	ASSERT_EQ(clst_l, std_tmp_vector);
}

TEST(list_contructor_test_suite, copy) {
	size_t size = list_generator.get_size();

	ctl::list<int> clist(size);
	ctl::list<int> copy_ctl_list(clist);

	ASSERT_EQ(clist, copy_ctl_list);
}


TEST(list_contructor_test_suite, copy_from_stl) {
	size_t size = list_generator.get_size();

	std::vector<int> svector;
	std::list<int> slist;
	std::set<int> sset;
	std::map<int, int> smap;

	for (size_t i = 0; i < size; ++i) {
		int value = list_generator.get_value();

		svector.push_back(value);
		slist.push_back(value);
		sset.insert(value);
		smap[value] = value;
	}

	ctl::list<int> copy_ctl_svector(svector);
	ctl::list<int> copy_ctl_slist(slist);
	ctl::list<int> copy_ctl_sset(sset);
	ctl::list<std::pair<int, int>> copy_ctl_smap(smap);

	ASSERT_EQ(copy_ctl_svector, svector); // from std::vector test
	ASSERT_EQ(copy_ctl_slist, slist); // from std::list test


	// from std::set test
	ASSERT_EQ(copy_ctl_sset.size(), sset.size());
	for (auto it = sset.begin(); it != sset.end(); ++it)
		ASSERT_TRUE(copy_ctl_sset.contains(*it));

	// from std::map test
	ASSERT_EQ(copy_ctl_smap.size(), smap.size());
	auto cit = copy_ctl_smap.begin();
	for (auto sit = smap.begin(); sit != smap.end(); ++sit, ++cit)
		ASSERT_EQ(cit->second, sit->second);
}

TEST(list_contructor_test_suite, move) {
	size_t size = list_generator.get_size();

	ctl::list<int> ctl_list(size);

	for (size_t i = 0; i < size; ++i) {
		int value = list_generator.get_value();
		ctl_list.push_back(value);
	}

	auto saved_list(ctl_list);
	ctl::list<int> moved_ctl_list(std::move(ctl_list));

	ASSERT_EQ(ctl_list.size(), 0);

	ASSERT_EQ(moved_ctl_list, saved_list);
}

TEST(list_contructor_test_suite, initializer_list) {
	ctl::list<int> ctl_list{1, 2, 3, 4, 5, 6, 7};
	std::list<int> std_list{1, 2, 3, 4, 5, 6, 7};

	ASSERT_EQ(ctl_list, std_list);

	ctl_list.push_back(-1);
	ASSERT_NE(ctl_list, std_list);

	std_list.push_back(-1);
	ASSERT_EQ(ctl_list, std_list);
}