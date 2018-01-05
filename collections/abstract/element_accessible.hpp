//
// Created by Artem Bobrov on 01.01.2018.
//

#ifndef COLLECTIONS_ELEMENT_ACCESSIBLE_HPP
#define COLLECTIONS_ELEMENT_ACCESSIBLE_HPP

#include <iterator>
#include "object.hpp"

namespace ctl {
	template<class T, class Iterator>
	class element_accessible {
	public:
		typedef T value_type;
		typedef Iterator iterator;
		typedef value_type &reference;
		typedef value_type const &const_reference;
		typedef iterator pointer;
		typedef size_t size_type;
	public:
		inline virtual reference at(size_type i) = 0; // stl
		inline virtual const_reference at(size_type i) const = 0; // stl

		inline virtual reference back() = 0; // stl
		inline virtual const_reference back() const = 0; // stl

		inline virtual pointer data() noexcept = 0; // stl
		inline virtual const pointer data() const noexcept = 0; // stl

		inline virtual reference front() = 0; // stl
		inline virtual const_reference front() const = 0; // stl

		inline reference operator[](size_type i); // stl
		inline const_reference operator[](size_type i) const; // stl
	};
	template<class T, class Iterator>
	typename element_accessible<T, Iterator>::reference element_accessible<T, Iterator>::operator[](size_type i) {
		return at(i);
	}
	template<class T, class Iterator>
	typename element_accessible<T, Iterator>::const_reference
	element_accessible<T, Iterator>::operator[](size_type i) const {
		return at(i);
	}

}

#endif //COLLECTIONS_ELEMENT_ACCESSIBLE_HPP
