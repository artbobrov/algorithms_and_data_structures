//
// Created by Artem Bobrov on 15.01.2018.
//

#ifndef CONTAINERS_ELEMENT_ACCESSIBLE_MODIFIABLE_HPP
#define CONTAINERS_ELEMENT_ACCESSIBLE_MODIFIABLE_HPP

#include <iterator>
#include <memory>

#include "modifiable.hpp"

namespace ctl {
	template<class T, class Iterator>
	class element_accessible_modifiable {
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
	public:
		inline virtual void append(modifiable<value_type> &value) = 0;
		inline void append(const_reference value) { push_back(value); } // qt
		inline void append(value_type &&value) { push_back(std::move(value)); } // qt

		inline virtual iterator erase(const_iterator position) = 0;
		inline virtual iterator erase(size_type position) = 0;
		inline virtual iterator erase(const_iterator first, const_iterator last) = 0;
		inline virtual iterator erase(size_type first, size_type last) = 0;

		inline virtual iterator insert(const_iterator before, const T &value) = 0; // stl
		inline virtual iterator insert(size_type i, const T &value) = 0; // qt
		inline virtual iterator insert(const_iterator before, value_type &&value) = 0; // stl
		inline virtual iterator insert(size_type i, value_type &&value) = 0; // stl
		inline virtual iterator insert(const_iterator before, size_type count, const T &value) = 0; // stl
		inline virtual iterator insert(size_type i, size_type count, const T &value) = 0; // qt
		inline virtual iterator insert(const_iterator position, std::initializer_list<value_type> il) = 0; // stl
		inline virtual iterator insert(size_type i, std::initializer_list<value_type> il) = 0; // stl

		inline virtual void pop_back() = 0; // stl
		inline virtual void pop_front() = 0; // qt
		inline virtual void push_back(const_reference value) = 0; // stl
		inline virtual void push_back(value_type &&value) = 0; // stl
		inline virtual void push_front(const_reference value) = 0; // qt
		inline virtual void push_front(value_type &&value) = 0; // qt

		inline virtual void remove_all(const_reference item) = 0; // qt
		inline virtual void remove_at(int i) = 0; // qt
		inline virtual void remove(const_reference item) = 0; // qt
		inline void remove_last() { pop_back(); } // qt
		inline void remove_first() { pop_front(); } // qt

//		inline virtual void resize(size_type count) = 0;
		inline virtual void resize(size_type count, const value_type &value) = 0;
	};
}

#endif //CONTAINERS_ELEMENT_ACCESSIBLE_MODIFIABLE_HPP
