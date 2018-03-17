//
// Created by Artem Bobrov on 25.01.2018.
//

#ifndef CONTAINERS_COLLECTION_HPP
#define CONTAINERS_COLLECTION_HPP

#include <list>
#include <set>

#include "../interface/interfaces.hpp"
#include "../interface/meta.hpp"
#include "sequence.hpp"

namespace ctl {
	template<class T, class Iterator, class Allocator = std::allocator<T> >
	class collection : public sequence<T, Iterator> {
	public:
		typedef Allocator allocator_type;
		typedef T value_type;
		typedef Iterator iterator;
		typedef iterator const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef const std::reverse_iterator<iterator> const_reverse_iterator;
		typedef value_type &reference;
		typedef const value_type &const_reference;
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef std::function<bool(const_reference)> conformer;
		typedef std::function<void(reference)> action;
		typedef std::function<bool(const_reference, const_reference)> comparer;
		typedef std::function<reference(reference)> map_action;
	public:
		explicit collection(const Allocator &alloc = Allocator()) : _allocator(alloc) {}
		inline virtual ~collection() = default;
	public:
		inline allocator_type allocator() const noexcept { return _allocator; } // stl

		inline size_type index_of(const_reference value, size_type first, size_type last) const; // qt
		inline size_type index_of(const_reference value) const {
			return index_of(value, 0, ctl::distance(this->begin(), this->end()));
		} // qt
		inline size_type iterator_of(const_reference value, iterator first, iterator last) const; // qt

		inline void reverse(); // c#
		inline void reverse(iterator first, iterator last); // c#
		inline void reverse(size_type first, size_type last); // c#

	protected:
		allocator_type _allocator;
	};
	template<class T, class Iterator, class Allocator>
	typename collection<T, Iterator, Allocator>::size_type collection<T, Iterator, Allocator>::index_of(const T &value,
	                                                                                                    size_type first,
	                                                                                                    size_type last) const {
		iterator _first = this->begin() + first;
		iterator _last = this->begin() + last;
		for (; _first != _last && *_first != value; ++_first) {}
		return _first - this->begin();
	}
	template<class T, class Iterator, class Allocator>
	void collection<T, Iterator, Allocator>::reverse() {
		reverse(this->begin(), this->end());
	}
	template<class T, class Iterator, class Allocator>
	typename collection<T, Iterator, Allocator>::size_type collection<T,
	                                                                  Iterator,
	                                                                  Allocator>::iterator_of(const T &value,
	                                                                                          Iterator first,
	                                                                                          Iterator last) const {
		for (; first != last && *first != value; ++first) {}
		return first;
	}
	template<class T, class Iterator, class Allocator>
	void collection<T, Iterator, Allocator>::reverse(Iterator first, Iterator last) {
		using std::swap;
		while (first != last) {
			if (first == --last)
				break;
			swap(*first, *last);
			++first;
		}
	}
	template<class T, class Iterator, class Allocator>
	void collection<T, Iterator, Allocator>::reverse(size_type first, size_type last) {
		using std::swap;
		while (first != last) {
			if (first == --last)
				break;
			swap(this[first], this[last]);
			++first;
		}
	}

	/*
	 * Non member functions: BEGIN
	 */

	// FIXME: mb bug with the comparison of other ctl containers(ctl::list, ctl::set etc)
	// the brute force solution: implement operator== for each pair of ctl containers
	template<class T, class Iterator, class Allocator, class Container, typename =
	typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator==(const collection<T, Iterator, Allocator> &lhs, const Container &rhs) {
		auto _collection_iter = lhs.begin();
		auto _container_iter = rhs.begin();

		for (; _collection_iter != lhs.end() && _container_iter != rhs.end();
		       ++_collection_iter, ++_container_iter) {
			if (*_collection_iter != *_container_iter)
				return false;
		}
		return _collection_iter == lhs.end() && _container_iter == rhs.end();
	}

	template<class T, class Iterator, class Allocator, class Container, typename =
	typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator==(const Container &lhs, const collection<T, Iterator, Allocator> &rhs) {
		return rhs == lhs;
	}

	template<class T, class Iterator, class Allocator, class Container, typename =
	typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator!=(const collection<T, Iterator, Allocator> &lhs, const Container &rhs) {
		return !(lhs == rhs);
	}

	template<class T, class Iterator, class Allocator, class Container, typename =
	typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator!=(const Container &lhs, const collection<T, Iterator, Allocator> &rhs) {
		return !(rhs == lhs);
	}

	template<class T, class Iterator, class Allocator, class Container, typename =
	typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator<(const collection<T, Iterator, Allocator> &lhs, const Container &rhs) {
		auto lfirst = lhs.begin(), llast = lhs.end();
		auto rfirst = rhs.begin(), rlast = rhs.end();
		for (; lfirst != llast && rfirst != rlast; ++lfirst, ++rfirst) {
			if (lfirst <= llast || *lfirst < *rfirst)
				return true;
			if (*rfirst < *lfirst)
				return false;
		}
		return false;
	}
	template<class T, class Iterator, class Allocator, class Container, typename =
	typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator>(const collection<T, Iterator, Allocator> &lhs, const Container &rhs) {
		return rhs < lhs;
	}
	template<class T, class Iterator, class Allocator, class Container, typename =
	typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator<=(const collection<T, Iterator, Allocator> &lhs, const Container &rhs) {
		return !(rhs < lhs);
	}
	template<class T, class Iterator, class Allocator, class Container, typename =
	typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator>=(const collection<T, Iterator, Allocator> &lhs, const Container &rhs) {
		return !(lhs < rhs);
	}

	template<class T, class Iterator, class Allocator, class Container, typename =
	typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator<(const Container &lhs, const collection<T, Iterator, Allocator> &rhs) {
		auto lfirst = lhs.begin(), llast = lhs.end();
		auto rfirst = rhs.begin(), rlast = rhs.end();
		for (; lfirst != llast && rfirst != rlast; ++lfirst, ++rfirst) {
			if (lfirst <= llast || *lfirst < *rfirst)
				return true;
			if (*rfirst < *lfirst)
				return false;
		}
		return false;
	}
	template<class T, class Iterator, class Allocator, class Container, typename =
	typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator>(const Container &lhs, const collection<T, Iterator, Allocator> &rhs) {
		return rhs < lhs;
	}
	template<class T, class Iterator, class Allocator, class Container, typename =
	typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator<=(const Container &lhs, const collection<T, Iterator, Allocator> &rhs) {
		return !(rhs < lhs);
	}
	template<class T, class Iterator, class Allocator, class Container, typename =
	typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator>=(const Container &lhs, const collection<T, Iterator, Allocator> &rhs) {
		return !(lhs < rhs);
	}
	/*
	 * Non member functions: END
	 */



}

#endif //CONTAINERS_COLLECTION_HPP
