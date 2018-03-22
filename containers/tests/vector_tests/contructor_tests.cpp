#include "gtest/gtest.h"
#include "vector.hpp"

#include <ctime>
#include <vector>
#include <random>

#include <thread>

#include "../extra/extra_functions.hpp"
#include "../extra/generator.hpp"

generator vector_generator(static_cast<unsigned int>(time(0)));

TEST(vector_contructor_test_suite, default) {
	std::vector<int> svec;
	ctl::vector<int> cvec;
	ASSERT_EQ(cvec, svec);
}

TEST(vector_contructor_test_suite, default_value) {
	size_t size = vector_generator.get_size();
	int value = vector_generator.get_value();
	std::vector<int> svec(size, value);
	ctl::vector<int> cvec(size, value);
	ASSERT_EQ(cvec, svec);
}

TEST(vector_contructor_test_suite, template) {
	size_t size = vector_generator.get_size();

	std::vector<int> std_tmp_vector(size);
	std::list<int> std_tmp_list;

	for (size_t i = 0; i < size; ++i) {
		int value = vector_generator.get_value();
		std_tmp_vector[i] = value;
		std_tmp_list.push_back(value);
	}

	ctl::vector<int> cvec_v(std_tmp_vector.begin(), std_tmp_vector.end());
	ASSERT_EQ(cvec_v, std_tmp_list);
	ASSERT_EQ(cvec_v, std_tmp_vector);

	ctl::vector<int> cvec_l(std_tmp_list.begin(), std_tmp_list.end());
	ASSERT_EQ(cvec_l, std_tmp_list);
	ASSERT_EQ(cvec_l, std_tmp_vector);
}
TEST(vector_contructor_test_suite, copy) {
	size_t size = vector_generator.get_size();

	ctl::vector<int> cvector(size);

	for (size_t i = 0; i < size; ++i) {
		int value = vector_generator.get_value();
		cvector[i] = value;
	}
	ctl::vector<int> copy_ctl_vector(cvector);

	ASSERT_EQ(cvector, copy_ctl_vector);
}
TEST(vector_contructor_test_suite, copy_from_stl) {
	size_t size = vector_generator.get_size();

	std::vector<int> svector;
	std::list<int> slist;
	std::set<int> sset;
	std::map<int, int> smap;

	for (size_t i = 0; i < size; ++i) {
		int value = vector_generator.get_value();

		svector.push_back(value);
		slist.push_back(value);
		sset.insert(value);
		smap[value] = value;
	}

	ctl::vector<int> copy_ctl_svector(svector);
	ctl::vector<int> copy_ctl_slist(slist);
	ctl::vector<int> copy_ctl_sset(sset);
	ctl::vector<std::pair<int, int>> copy_ctl_smap(smap);

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

TEST(vector_contructor_test_suite, move) {
	size_t size = vector_generator.get_size();

	ctl::vector<int> ctl_vector(size);

	for (size_t i = 0; i < size; ++i) {
		int value = vector_generator.get_value();
		ctl_vector[i] = value;
	}

	auto saved_vector(ctl_vector);
	ctl::vector<int> moved_ctl_vector(std::move(ctl_vector));

	ASSERT_EQ(ctl_vector.size(), 0);
	ASSERT_EQ(ctl_vector.capacity(), 0);

	ASSERT_EQ(moved_ctl_vector, saved_vector);
	ASSERT_EQ(moved_ctl_vector.capacity(), saved_vector.capacity());
}

TEST(vector_contructor_test_suite, initializer_list) {
	ctl::vector<int> ctl_vector{1, 2, 3, 4, 5, 6, 7};
	std::vector<int> std_vector{1, 2, 3, 4, 5, 6, 7};

	ASSERT_EQ(ctl_vector, std_vector);

	ctl_vector.push_back(-1);
	ASSERT_NE(ctl_vector, std_vector);

	std_vector.push_back(-1);
	ASSERT_EQ(ctl_vector, std_vector);
}

