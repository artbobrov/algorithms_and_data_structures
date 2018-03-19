//
// Created by Artem Bobrov on 20.03.2018.
//

#ifndef CONTAINERS_BIDIRECTIONAL_ACCESS_COLLECTION_HPP
#define CONTAINERS_BIDIRECTIONAL_ACCESS_COLLECTION_HPP

#include <list>
#include <set>

#include "../../interface/interfaces.hpp"
#include "../../interface/meta.hpp"
#include "../sequence/random_access_sequence.hpp"

namespace ctl {
	template<class T, class Iterator, class Allocator = std::allocator<T> >
	class bidirectional_access_collection : public virtual bidirectional_access_sequence<T, Iterator> {
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
		explicit bidirectional_access_collection(const Allocator &alloc = Allocator()) : _allocator(alloc) {}
		inline virtual ~bidirectional_access_collection() = default;
	public:
		inline allocator_type allocator() const noexcept { return _allocator; } // stl

		inline size_type iterator_of(const_reference value, iterator first, iterator last) const; // qt

		inline void reverse() { reverse(this->begin(), this->end()); } // c#
		inline void reverse(iterator first, iterator last); // c#

	protected:
		allocator_type _allocator;
	};
	template<class T, class Iterator, class Allocator>
	typename bidirectional_access_collection<T, Iterator, Allocator>::size_type
	bidirectional_access_collection<T, Iterator, Allocator>::iterator_of(const T &value,
	                                                                     Iterator first,
	                                                                     Iterator last) const {
		for (; first != last && *first != value; ++first) {}
		return first;
	}
	template<class T, class Iterator, class Allocator>
	void bidirectional_access_collection<T, Iterator, Allocator>::reverse(Iterator first, Iterator last) {
		using std::swap;
		while (first != last) {
			if (first == --last)
				break;
			swap(*first, *last);
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
	inline bool operator==(const bidirectional_access_collection<T, Iterator, Allocator> &lhs, const Container &rhs) {
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
	inline bool operator==(const Container &lhs, const bidirectional_access_collection<T, Iterator, Allocator> &rhs) {
		return rhs == lhs;
	}

	template<class T, class Iterator, class Allocator, class Container, typename =
	typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator!=(const bidirectional_access_collection<T, Iterator, Allocator> &lhs, const Container &rhs) {
		return !(lhs == rhs);
	}

	template<class T, class Iterator, class Allocator, class Container, typename =
	typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator!=(const Container &lhs, const bidirectional_access_collection<T, Iterator, Allocator> &rhs) {
		return !(rhs == lhs);
	}

	template<class T, class Iterator, class Allocator, class Container, typename =
	typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator<(const bidirectional_access_collection<T, Iterator, Allocator> &lhs, const Container &rhs) {
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
	inline bool operator>(const bidirectional_access_collection<T, Iterator, Allocator> &lhs, const Container &rhs) {
		return rhs < lhs;
	}
	template<class T, class Iterator, class Allocator, class Container, typename =
	typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator<=(const bidirectional_access_collection<T, Iterator, Allocator> &lhs, const Container &rhs) {
		return !(rhs < lhs);
	}
	template<class T, class Iterator, class Allocator, class Container, typename =
	typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator>=(const bidirectional_access_collection<T, Iterator, Allocator> &lhs, const Container &rhs) {
		return !(lhs < rhs);
	}

	template<class T, class Iterator, class Allocator, class Container, typename =
	typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator<(const Container &lhs, const bidirectional_access_collection<T, Iterator, Allocator> &rhs) {
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
	inline bool operator>(const Container &lhs, const bidirectional_access_collection<T, Iterator, Allocator> &rhs) {
		return rhs < lhs;
	}
	template<class T, class Iterator, class Allocator, class Container, typename =
	typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator<=(const Container &lhs, const bidirectional_access_collection<T, Iterator, Allocator> &rhs) {
		return !(rhs < lhs);
	}
	template<class T, class Iterator, class Allocator, class Container, typename =
	typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator>=(const Container &lhs, const bidirectional_access_collection<T, Iterator, Allocator> &rhs) {
		return !(lhs < rhs);
	}
	/*
	 * Non member functions: END
	 */
}
#endif //CONTAINERS_BIDIRECTIONAL_ACCESS_COLLECTION_HPP
