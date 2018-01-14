//
// Created by Artem Bobrov on 11.01.2018.
//

#ifndef TESTS_EXTRA_STD_FUNCTIONS_HPP
#define TESTS_EXTRA_STD_FUNCTIONS_HPP

#include <string>
#include <utility>
namespace std {
	template<class Key, class Value>
	std::string to_string(const std::pair<Key, Value> &__pair) {
		return string('{' + to_string(__pair.first) + ',' + to_string(__pair.second) + '}');
	}
}

template<class Key, class Value>
bool operator==(const std::pair<Key, Value> &__x, const std::pair<Key, Value> &__y) {
	return __x.first == __y.first && __x.second == __y.second;
}
template<class Key, class Value>
bool operator!=(const std::pair<Key, Value> &__x, const std::pair<Key, Value> &__y) {
	return !(__x == __y);
}
#endif //TESTS_EXTRA_STD_FUNCTIONS_HPP
