//
// Created by Artem Bobrov on 11.01.2018.
//

#ifndef TESTS_EXTRA_STD_FUNCTION_HPP
#define TESTS_EXTRA_STD_FUNCTION_HPP

#include <string>

namespace std {
	template<class Key, class Value>
	std::string to_string(const std::pair<Key, Value> &__pair) {
		return std::string('{' + std::to_string(__pair.first) + ',' + std::to_string(__pair.second) + '}');
	}

	template<class Key, class Value>
	bool operator==(const std::pair<Key, Value> &__x, const std::pair<Key, Value> &__y) {
		return __x.first == __y.first && __x.second == __y.second;
	}
}

#endif //TESTS_EXTRA_STD_FUNCTION_HPP
