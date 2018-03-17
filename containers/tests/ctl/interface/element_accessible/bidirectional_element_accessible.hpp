//
// Created by Artem Bobrov on 14.01.2018.
//

#ifndef CONTAINERS_BEDIRECTIONAL_ELEMENT_ACCESSIBLE_HPP
#define CONTAINERS_BEDIRECTIONAL_ELEMENT_ACCESSIBLE_HPP

#include <iterator>

template<class T>
class bidirectional_element_accessible {
public:
	typedef T value_type;
	typedef value_type &reference;
	typedef value_type const &const_reference;
public:
	inline virtual reference back() = 0; // stl
	inline virtual const_reference back() const = 0; // stl

	inline virtual reference front() = 0; // stl
	inline virtual const_reference front() const = 0; // stl
};

#endif //CONTAINERS_BEDIRECTIONAL_ELEMENT_ACCESSIBLE_HPP
