//
// Created by Artem Bobrov on 15.01.2018.
//

#ifndef CONTAINERS_ELEMENT_ACCESSIBLE_MODIFIABLE_HPP
#define CONTAINERS_ELEMENT_ACCESSIBLE_MODIFIABLE_HPP

#include <iterator>
#include <memory>

#include "bidirectional_element_accessible_modifiable.hpp"
#include "../base/object.hpp"
#include "../element_accessible/random_element_accessible.hpp"
#include "../iterable/iterable.hpp"

namespace ctl {
	template<class T, class Iterator>
	class random_element_accessible_modifiable : public virtual object,
	                                             public bidirectional_element_accessible_modifiable<T, Iterator>,
	                                             public random_element_accessible<T>,
	                                             public virtual iterable<Iterator> {
	public:
		typedef T value_type;
		typedef Iterator iterator;
		typedef const iterator const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef const std::reverse_iterator<iterator> const_reverse_iterator;
		typedef iterator pointer;
		typedef value_type &reference;
		typedef value_type const &const_reference;
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef std::function<bool(const_reference)> conformer;
	public:
		inline virtual iterator erase(size_type position) = 0;
		inline virtual iterator erase(size_type first, size_type last) = 0;

		inline virtual iterator insert(size_type idx, const T &value) = 0; // qt
		inline virtual iterator insert(size_type idx, value_type &&value) = 0; // stl
		inline virtual iterator insert(size_type idx, size_type count, const T &value) = 0; // qt
		inline virtual iterator insert(size_type idx, std::initializer_list<value_type> il) = 0; // stl

		using bidirectional_element_accessible_modifiable<value_type, iterator>::remove_all;
		inline void remove_all(size_type first, size_type last, const_reference item) {
			remove_all(this->begin() + first, this->begin() + last, item);
		} // qt
		inline void remove_all(size_type first, size_type last, conformer predicate) {
			remove_all(this->begin() + first, this->begin() + last, predicate);
		} // qt
		inline void remove_at(size_type idx) { erase(idx); } // qt
	};
}

#endif //CONTAINERS_ELEMENT_ACCESSIBLE_MODIFIABLE_HPP
