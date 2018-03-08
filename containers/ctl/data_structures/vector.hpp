//
// Created by Artem Bobrov on 08.03.2018.
//

#ifndef CONTAINERS_VECTOR_HPP
#define CONTAINERS_VECTOR_HPP

#include "../interface/interfaces.hpp"
#include "../abstract/collection.hpp"
#include "../abstract/sequence.hpp"
namespace ctl {

	template<class T>
	struct __vector_iterator : public std::iterator<std::random_access_iterator_tag, T> {
		typedef T value_type;
		typedef value_type &reference;
		typedef value_type const &const_reference;

		typedef value_type *data;
		typedef size_t size_type;
	public: // properties
		mutable data data_point;
	public: // methods
		explicit __vector_iterator(data point = nullptr) : data_point(point) {}

		__vector_iterator(const __vector_iterator &other) : data_point(other.data_point) {}

		__vector_iterator &operator=(const __vector_iterator &other) {
			if (this != &other) {
				data_point = other.data_point;
			}
			return *this;
		}

		inline bool operator==(const __vector_iterator &other) { return other.data_point == data_point; }
		inline bool operator!=(const __vector_iterator &other) { return !(other == *this); }

		inline reference operator*() { return *data_point; }
		inline const_reference operator*() const { return *data_point; }
		inline T *operator->() { return &(operator*()); }

		inline __vector_iterator &operator++() {
			++data_point;
			return *this;
		}
		inline __vector_iterator operator++(int) {
			__vector_iterator value = *this;
			++*this;
			return value;
		}
		inline __vector_iterator &operator--() {
			--data_point;
			return *this;
		}
		inline __vector_iterator operator--(int) {
			__vector_iterator value = *this;
			--*this;
			return value;
		}

		inline __vector_iterator &plus(size_type idx) {
			data_point += idx;
			return *this;
		}
		inline __vector_iterator &minus(size_type idx) {
			data_point -= idx;
			return *this;
		}

		bool operator<(const __vector_iterator &rhs) {
			return data_point < rhs.data_point;
		}
		bool operator>(const __vector_iterator &rhs) {
			return rhs < *this;
		}
		bool operator<=(const __vector_iterator &rhs) {
			return !(rhs < *this);
		}
		bool operator>=(const __vector_iterator &rhs) {
			return !(*this < rhs);
		}

	};

	template<class T>
	inline __vector_iterator<T> operator+(const __vector_iterator<T> &__x,
	                                      typename __vector_iterator<T>::size_type idx) {
		auto value = __x;
		value.plus(idx);
		return value;
	}
	template<class T>
	inline __vector_iterator<T> operator-(const __vector_iterator<T> &__x,
	                                      typename __vector_iterator<T>::size_type idx) {
		auto value = __x;
		value.minus(idx);
		return value;
	}

	template<class T, class Allocator>
	class vector : public collection<T, __vector_iterator<T>, Allocator>,
	               public reservable<T>, /*TODO:comparable<> ... */
	               public random_element_accessible<T>,
	               public modifiable<T>,
	               public resizable<T>,
	               public element_accessible_modifiable<T, __vector_iterator<T>> {
	public:
		typedef Allocator allocator_type;
		typedef T value_type;
		typedef __vector_iterator<T> iterator;
		typedef iterator const_iterator;
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
	public:

	};
}
#endif //CONTAINERS_VECTOR_HPP
