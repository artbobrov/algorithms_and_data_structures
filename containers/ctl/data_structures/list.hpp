//
// Created by Artem Bobrov on 20.03.2018.
//

#ifndef CONTAINERS_LIST_HPP
#define CONTAINERS_LIST_HPP

#include <iterator>
#include "../abstract/abstracts.hpp"

namespace ctl {
	template<class T>
	struct __node {
		typedef T value_type;

		value_type data;
		__node *prev;
		__node *next;

		inline bool operator==(const __node &other) {
			return other.data == data && other.next == next && other.prev == prev;
		}
	};

	template<class T>
	class __list_iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
	public:
		typedef T value_type;
		typedef value_type &reference;
		typedef value_type const &const_reference;

		typedef __node<T> node;
		typedef node *node_point;
		typedef size_t size_type;
	public:
		mutable node_point data_point;
	public:
		explicit __list_iterator(node_point point = nullptr) : data_point(point) {}

		inline __list_iterator &operator++() {
			data_point = data_point->next;
			return *this;
		}
		inline __list_iterator operator++(int) {
			__list_iterator value = *this;
			++*this;
			return value;
		}
		inline const __list_iterator &operator++() const {
			data_point = data_point->next;
			return *this;
		}
		inline const __list_iterator operator++(int) const {
			__list_iterator value = *this;
			++*this;
			return value;
		}
		inline __list_iterator &operator--() {
			data_point = data_point->prev;
			return *this;
		}
		inline __list_iterator operator--(int) {
			__list_iterator value = *this;
			--*this;
			return value;
		}
		inline const __list_iterator &operator--() const {
			data_point = data_point->prev;
			return *this;
		}
		inline const __list_iterator operator--(int) const {
			__list_iterator value = *this;
			--*this;
			return value;
		}
		inline reference operator*() { return data_point->data; }
		inline const_reference operator*() const { return data_point->data; }
		inline T *operator->() { return &(operator*()); }

		friend inline bool operator==(const __list_iterator &__x, const __list_iterator &__y) {
			return __x.data_point == __y.data_point;
		}
		friend inline bool operator!=(const __list_iterator &__x, const __list_iterator &__y) {
			return !(__x == __y);
		}
	};

	template<class T, class Allocator = std::allocator<__node<T>>>
	class list : public bidirectional_access_collection<T, __list_iterator<T>, Allocator>,
	             public bidirectional_element_accessible_modifiable<T, __list_iterator<T>>,
	             public data_accessible<__node<T>>,
	             public resizable<T> {
	public:
		typedef Allocator allocator_type;
		typedef T value_type;
		typedef __list_iterator<T> iterator;
		typedef const iterator const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef const std::reverse_iterator<iterator> const_reverse_iterator;
		typedef T *pointer;
		typedef value_type &reference;
		typedef value_type const &const_reference;
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef std::function<bool(const_reference)> conformer;
		typedef std::function<void(reference)> action;
		typedef std::function<bool(const_reference, const_reference)> comparer;

		typedef __node<T> node;
		typedef node *node_point;

	};
}

#endif //CONTAINERS_LIST_HPP
