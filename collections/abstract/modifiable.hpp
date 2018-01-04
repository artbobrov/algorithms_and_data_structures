//
// Created by Artem Bobrov on 01.01.2018.
//

#ifndef COLLECTIONS_MODIFIABLE_HPP
#define COLLECTIONS_MODIFIABLE_HPP

#include <functional>
#include <iterator>
#include <memory>

namespace ctl {
	template<class T, class Allocator>
	class collection;

	template<class T, class Allocator>
	class modifiable {
	public:
		typedef Allocator allocator_type;
		typedef T value_type;
		typedef value_type *iterator;
		typedef const iterator const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef const std::reverse_iterator<iterator> const_reverse_iterator;
		typedef iterator pointer;
		typedef value_type &reference;
		typedef value_type const &const_reference;
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef std::function<bool(const_reference)> conformer;
		typedef std::function<void(reference)> action;
		typedef std::function<bool(const_reference, const_reference)> comparer;
	public:
		inline virtual void append(const collection<value_type, allocator_type> &value) = 0;
		inline void append(const_reference value); // qt
		inline void append(value_type &&value); // qt

		inline virtual void clear() noexcept = 0; // stl



		inline virtual iterator erase(const_iterator position) = 0;
		inline virtual iterator erase(const_iterator first, const_iterator last) = 0;

		inline virtual iterator insert(const_iterator before, const T &value) = 0; // stl
		inline virtual iterator insert(size_type i, const T &value) = 0; // qt
		inline virtual iterator insert(const_iterator before, value_type &&value) = 0; // stl
		inline virtual iterator insert(const_iterator before, size_type count, const T &value) = 0; // stl
		inline virtual iterator insert(const_iterator position, std::initializer_list<value_type> il) = 0; // stl
		inline virtual iterator insert(size_type i, size_type count, const T &value) = 0; // qt

		inline virtual void pop_back() = 0; // stl
		inline virtual void pop_front() = 0; // qt
		inline virtual void push_back(const_reference value) = 0; // stl
		inline virtual void push_back(value_type &&value) = 0; // stl
		inline virtual void push_front(const_reference value) = 0; // qt
		inline virtual void push_front(value_type &&value) = 0; // qt

		inline virtual void resize(size_type count) = 0; // stl
		inline virtual void resize(size_type count, const T &value) = 0; // stl
		inline void remove_last(); // qt
		inline void remove_first(); // qt

	};
	template<class T, class Allocator>
	void modifiable<T, Allocator>::append(const_reference value) {
		push_back(value);
	}
	template<class T, class Allocator>
	void modifiable<T, Allocator>::append(value_type &&value) {
		push_back(value);
	}
	template<class T, class Allocator>
	void modifiable<T, Allocator>::remove_last() {
		pop_back();
	}
	template<class T, class Allocator>
	void modifiable<T, Allocator>::remove_first() {
		pop_front();
	}

}

#endif //COLLECTIONS_MODIFIABLE_HPP
