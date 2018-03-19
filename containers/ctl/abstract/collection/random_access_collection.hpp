//
// Created by Artem Bobrov on 25.01.2018.
//

#ifndef CONTAINERS_COLLECTION_HPP
#define CONTAINERS_COLLECTION_HPP

#include <list>
#include <set>

#include "../../interface/interfaces.hpp"
#include "../../interface/meta.hpp"
#include "../sequence/random_access_sequence.hpp"
#include "bidirectional_access_collection.hpp"

namespace ctl {
	template<class T, class Iterator, class Allocator = std::allocator<T> >
	class random_access_collection : public random_access_sequence<T, Iterator>,
	                                 public bidirectional_access_collection<T, Iterator, Allocator> {
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
		explicit random_access_collection(const Allocator &alloc = Allocator())
			: random_access_collection::bidirectional_access_collection(alloc) {}
		inline virtual ~random_access_collection() = default;
	public:
		inline size_type index_of(const_reference value, size_type first, size_type last) const; // qt
		inline size_type index_of(const_reference value) const {
			return index_of(value, 0, ctl::distance(this->begin(), this->end()));
		} // qt

		using bidirectional_access_collection<value_type, iterator, allocator_type>::reverse;
		inline void reverse(size_type first, size_type last); // c#
	};
	template<class T, class Iterator, class Allocator>
	typename random_access_collection<T, Iterator, Allocator>::size_type random_access_collection<T,
	                                                                                              Iterator,
	                                                                                              Allocator>::index_of(
		const T &value,
		size_type first,
		size_type last) const {
		iterator _first = this->begin() + first;
		iterator _last = this->begin() + last;
		for (; _first != _last && *_first != value; ++_first) {}
		return _first - this->begin();
	}
	template<class T, class Iterator, class Allocator>
	void random_access_collection<T, Iterator, Allocator>::reverse(size_type first, size_type last) {
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
	//	// FIXME: mb bug with the comparison of other ctl containers(ctl::list, ctl::set etc)
	//	// the brute force solution: implement operator== for each pair of ctl containers
	//	template<class T, class Iterator, class Allocator, class Container, typename =
	//	typename std::enable_if<has_begin_end<Container>::value>::type>
	//	inline bool operator==(const random_access_collection<T, Iterator, Allocator> &lhs, const Container &rhs) {
	//		auto _collection_iter = lhs.begin();
	//		auto _container_iter = rhs.begin();
	//
	//		for (; _collection_iter != lhs.end() && _container_iter != rhs.end();
	//		       ++_collection_iter, ++_container_iter) {
	//			if (*_collection_iter != *_container_iter)
	//				return false;
	//		}
	//		return _collection_iter == lhs.end() && _container_iter == rhs.end();
	//	}
	//
	//	template<class T, class Iterator, class Allocator, class Container, typename =
	//	typename std::enable_if<has_begin_end<Container>::value>::type>
	//	inline bool operator==(const Container &lhs, const random_access_collection<T, Iterator, Allocator> &rhs) {
	//		return rhs == lhs;
	//	}
	//
	//	template<class T, class Iterator, class Allocator, class Container, typename =
	//	typename std::enable_if<has_begin_end<Container>::value>::type>
	//	inline bool operator!=(const random_access_collection<T, Iterator, Allocator> &lhs, const Container &rhs) {
	//		return !(lhs == rhs);
	//	}
	//
	//	template<class T, class Iterator, class Allocator, class Container, typename =
	//	typename std::enable_if<has_begin_end<Container>::value>::type>
	//	inline bool operator!=(const Container &lhs, const random_access_collection<T, Iterator, Allocator> &rhs) {
	//		return !(rhs == lhs);
	//	}
	//
	//	template<class T, class Iterator, class Allocator, class Container, typename =
	//	typename std::enable_if<has_begin_end<Container>::value>::type>
	//	inline bool operator<(const random_access_collection<T, Iterator, Allocator> &lhs, const Container &rhs) {
	//		auto lfirst = lhs.begin(), llast = lhs.end();
	//		auto rfirst = rhs.begin(), rlast = rhs.end();
	//		for (; lfirst != llast && rfirst != rlast; ++lfirst, ++rfirst) {
	//			if (lfirst <= llast || *lfirst < *rfirst)
	//				return true;
	//			if (*rfirst < *lfirst)
	//				return false;
	//		}
	//		return false;
	//	}
	//	template<class T, class Iterator, class Allocator, class Container, typename =
	//	typename std::enable_if<has_begin_end<Container>::value>::type>
	//	inline bool operator>(const random_access_collection<T, Iterator, Allocator> &lhs, const Container &rhs) {
	//		return rhs < lhs;
	//	}
	//	template<class T, class Iterator, class Allocator, class Container, typename =
	//	typename std::enable_if<has_begin_end<Container>::value>::type>
	//	inline bool operator<=(const random_access_collection<T, Iterator, Allocator> &lhs, const Container &rhs) {
	//		return !(rhs < lhs);
	//	}
	//	template<class T, class Iterator, class Allocator, class Container, typename =
	//	typename std::enable_if<has_begin_end<Container>::value>::type>
	//	inline bool operator>=(const random_access_collection<T, Iterator, Allocator> &lhs, const Container &rhs) {
	//		return !(lhs < rhs);
	//	}
	//
	//	template<class T, class Iterator, class Allocator, class Container, typename =
	//	typename std::enable_if<has_begin_end<Container>::value>::type>
	//	inline bool operator<(const Container &lhs, const random_access_collection<T, Iterator, Allocator> &rhs) {
	//		auto lfirst = lhs.begin(), llast = lhs.end();
	//		auto rfirst = rhs.begin(), rlast = rhs.end();
	//		for (; lfirst != llast && rfirst != rlast; ++lfirst, ++rfirst) {
	//			if (lfirst <= llast || *lfirst < *rfirst)
	//				return true;
	//			if (*rfirst < *lfirst)
	//				return false;
	//		}
	//		return false;
	//	}
	//	template<class T, class Iterator, class Allocator, class Container, typename =
	//	typename std::enable_if<has_begin_end<Container>::value>::type>
	//	inline bool operator>(const Container &lhs, const random_access_collection<T, Iterator, Allocator> &rhs) {
	//		return rhs < lhs;
	//	}
	//	template<class T, class Iterator, class Allocator, class Container, typename =
	//	typename std::enable_if<has_begin_end<Container>::value>::type>
	//	inline bool operator<=(const Container &lhs, const random_access_collection<T, Iterator, Allocator> &rhs) {
	//		return !(rhs < lhs);
	//	}
	//	template<class T, class Iterator, class Allocator, class Container, typename =
	//	typename std::enable_if<has_begin_end<Container>::value>::type>
	//	inline bool operator>=(const Container &lhs, const random_access_collection<T, Iterator, Allocator> &rhs) {
	//		return !(lhs < rhs);
	//	}
	//	/*
	//	 * Non member functions: END
	//	 */
}

#endif //CONTAINERS_COLLECTION_HPP
