//
// Created by Artem Bobrov on 17.03.2018.
//

#ifndef TESTS_BIDIRECTIONAL_ELEMENT_ACCESSIBLE_MODIFIABLE_HPP
#define TESTS_BIDIRECTIONAL_ELEMENT_ACCESSIBLE_MODIFIABLE_HPP

#include <iterator>
#include <memory>

#include "../interfaces.hpp"

namespace ctl {
	template<class T, class Iterator>
	class bidirectional_element_accessible_modifiable : public bidirectional_element_accessible<T>,
	                                      public modifiable,
	                                      public size_accessible,
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
		inline virtual void append(bidirectional_element_accessible_modifiable<value_type, iterator> &value) = 0;
		inline void append(const_reference value) { push_back(value); } // qt
		inline void append(value_type &&value) { push_back(std::move(value)); } // qt

		inline virtual iterator erase(const_iterator position) = 0;
		inline virtual iterator erase(const_iterator first, const_iterator last) = 0;

		inline virtual iterator insert(const_iterator before, const T &value) = 0; // stl
		inline virtual iterator insert(const_iterator before, value_type &&value) = 0; // stl
		inline virtual iterator insert(const_iterator before, size_type count, const T &value) = 0; // stl
		inline virtual iterator insert(const_iterator position, std::initializer_list<value_type> il) = 0; // stl

		inline virtual void pop_back() = 0; // stl
		inline virtual void pop_back(size_type count) = 0; // stl
		inline virtual void pop_front() = 0; // qt
		inline virtual void pop_front(size_type count) = 0; // qt
		inline virtual void push_back(const_reference value) = 0; // stl
		inline virtual void push_back(size_type count, const_reference value) = 0;
		inline virtual void push_back(value_type &&value) = 0; // stl
		inline virtual void push_front(const_reference value) = 0; // qt
		inline virtual void push_front(value_type &&value) = 0; // qt
		inline virtual void push_front(size_type count, const_reference value) = 0; // qt

		inline virtual void remove_all(const_reference item) { remove_all(this->begin(), this->end(), item); } // qt
		inline virtual void remove_all(iterator first, iterator last, const_reference item) = 0; // qt
		inline virtual void remove_all(conformer predicate) { remove_all(this->begin(), this->end(), predicate); } // qt
		inline virtual void remove_all(iterator first, iterator last, conformer predicate) = 0; // qt
		inline virtual void remove_at(int idx) = 0; // qt
		inline virtual void remove(const_reference item) = 0; // qt
		inline void remove_last() { pop_back(); } // qt
		inline void remove_first() { pop_front(); } // qt

		inline virtual void resize(size_type count) = 0;
		inline virtual void resize(size_type count, const value_type &value) = 0;
	};
}

#endif //TESTS_BIDIRECTIONAL_ELEMENT_ACCESSIBLE_MODIFIABLE_HPP
