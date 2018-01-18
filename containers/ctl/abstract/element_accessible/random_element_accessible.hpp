//
// Created by Artem Bobrov on 01.01.2018.
//

#ifndef COLLECTIONS_ELEMENT_ACCESSIBLE_HPP
#define COLLECTIONS_ELEMENT_ACCESSIBLE_HPP

#include <iterator>
#include "bidirectional_element_accessible.hpp"

namespace ctl {
	template<class T>
	class random_element_accessible : public bidirectional_element_accessible<T> {
	public:
		typedef T value_type;
		typedef value_type &reference;
		typedef value_type const &const_reference;
		typedef size_t size_type;
	public:
		inline virtual reference at(size_type i) = 0; // stl
		inline virtual const_reference at(size_type i) const = 0; // stl

		inline reference operator[](size_type i); // stl
		inline const_reference operator[](size_type i) const; // stl
	};
	template<class T>
	typename random_element_accessible<T>::reference random_element_accessible<T>::operator[](size_type i) {
		return at(i);
	}
	template<class T>
	typename random_element_accessible<T>::const_reference
	random_element_accessible<T>::operator[](size_type i) const {
		return at(i);
	}

}

#endif //COLLECTIONS_ELEMENT_ACCESSIBLE_HPP
