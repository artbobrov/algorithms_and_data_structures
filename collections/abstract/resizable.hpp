//
// Created by Artem Bobrov on 28.12.2017.
//

#ifndef COLLECTIONS_MUTABLE_HPP
#define COLLECTIONS_MUTABLE_HPP

#include <iterator>
#include <initializer_list>

namespace ctl {
	template<class T>
	class resizable {
	public:
		typedef T value_type;
		typedef value_type *iterator;
		typedef const iterator const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef const std::reverse_iterator<iterator> const_reverse_iterator;
		typedef iterator pointer;
		typedef value_type &reference;
		typedef const reference const_reference;
		typedef object::size_type size_type;
		typedef std::ptrdiff_t difference_type;
	public:
		inline virtual void append(const_reference value) = 0; // qt
		inline virtual void append(value_type &&value) = 0; // qt

		inline virtual void clear() noexcept = 0; // stl

		// FIXME: mb move to collection
		inline virtual void insert(size_type i, const_reference value) = 0; // qt
		inline virtual void insert(size_type i, size_type count, const_reference value) = 0; // qt
		inline virtual iterator insert(const_iterator before, size_type count, const_reference value) = 0; // stl
		inline virtual iterator insert(const_iterator before, const_reference value) = 0; // stl
		inline virtual iterator insert(const_iterator before, value_type &&value) = 0; // stl
		inline virtual iterator insert(const_iterator position, std::initializer_list<value_type> il) = 0; // stl

		inline virtual void pop_back() = 0; // stl
		inline virtual void pop_front() = 0; // stl
		inline virtual void push_back(const_reference value) = 0; // stl
		inline virtual void push_back(value_type &&value) = 0; // stl
		inline virtual void push_front(value_type &&value) = 0; // stl
		inline virtual void push_front(const_reference value) = 0; // stl

		inline virtual void remove_last() = 0; // qt
		inline virtual void remove_first() = 0; // qt

		inline virtual void reserve(size_type n) = 0; // stl
	public:
		inline virtual resizable<value_type> &operator+=(const resizable<value_type> &other) = 0; // qt
		inline virtual resizable<value_type> &operator+=(const_reference other) = 0; // qt
		inline virtual resizable<value_type> &operator+(const resizable<value_type> &other) = 0; // qt
		inline virtual resizable<value_type> &operator+(const_reference other) = 0; // qt
	};
}

#endif //COLLECTIONS_MUTABLE_HPP
