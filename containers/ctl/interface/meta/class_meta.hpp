//
// Created by Artem Bobrov on 23.01.2018.
//

#ifndef CONTAINERS_CLASS_META_HPP
#define CONTAINERS_CLASS_META_HPP

#include <type_traits>
namespace ctl {

	template<typename T, typename U, typename = std::void_t<>>
	struct has_less : std::false_type {};

	template<typename T, typename U>
	struct has_less<T, U, std::void_t<decltype(std::declval<T>() < std::declval<U>())>> : std::true_type {};

	template<typename T, typename U, typename = std::void_t<>>
	struct has_greater : std::false_type {};

	template<typename T, typename U>
	struct has_greater<T, U, std::void_t<decltype(std::declval<T>() > std::declval<U>())>> : std::true_type {};

	template<class T, class U>
	struct is_comparable : std::conjunction<has_less<T, U>, has_greater<T, U>> {};
}
#endif //CONTAINERS_CLASS_META_HPP
