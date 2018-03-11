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

		typedef bool (*conformer)(const_reference);
		typedef void (*action)(reference);
		typedef bool (*comparer)(const_reference, const_reference);
		typedef reference (*map_action)(reference);
	public:
		explicit collection(const Allocator &alloc = Allocator()) : _allocator(alloc) {}
		inline virtual ~collection() = default;
	public:
		inline allocator_type allocator() const noexcept { return _allocator; } // stl

		inline size_type index_of(const_reference value, size_type first, size_type last) const; // qt
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
		for (; first != last && this[first] != value; ++first) {}
		return first;
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
	 * non-member functions
	 */
	template<class T, class Iterator, class Allocator, class Container,
		typename = typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator==(const collection<T, Iterator, Allocator> &__collection, const Container &__container) {
		auto _collection_iter = __collection.begin();
		auto _container_iter = __container.begin();

		for (; _collection_iter != __collection.end() && _container_iter != __container.end();
		       ++_collection_iter, ++_container_iter) {
			if (*_collection_iter != *_container_iter)
				return false;
		}
		return _collection_iter == __collection.end() && _container_iter == __container.end();
	}

	template<class T, class Iterator, class Allocator, class Container,
		typename = typename std::enable_if<has_begin_end<Container>::value>::type>
	inline bool operator!=(const collection<T, Iterator, Allocator> &__collection, const Container &__container) {
		return !(__collection == __container);
	}

}

#endif //CONTAINERS_COLLECTION_HPP
