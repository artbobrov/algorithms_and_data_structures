//
// Created by Artem Bobrov on 01.01.2018.
//

#ifndef COLLECTIONS_COLLECTION_HPP
#define COLLECTIONS_COLLECTION_HPP

#include "object.hpp"
#include "element_accessible.hpp"
#include "iterable.hpp"
#include "reservable.hpp"
#include "modifiable.hpp"

#include <vector>
#include <list>
#include <set>

namespace ctl {

	template<class T, class Allocator = std::allocator<T> >
	class collection
		: public object, public element_accessible<T>, public iterable<T>, public reservable<T>, public modifiable<T> {
	private:
		typedef Allocator allocator_type;
	public:
		typedef T value_type;
		typedef value_type *iterator;
		typedef const iterator const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef const std::reverse_iterator<iterator> const_reverse_iterator;
		typedef iterator pointer;
		typedef value_type &reference;
		typedef const reference const_reference;
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef std::function<bool(const_reference)> conformer;
		typedef std::function<void(reference)> action;
		typedef std::function<bool(const_reference, const_reference)> comparer;
	public:
		inline allocator_type Allocator() const noexcept;

		inline bool contains(const_reference item) const; // qt
		inline size_type count(const_reference item) const noexcept; // qt

		inline void foreach(action act); // c#

		inline size_type index_of(const_reference value, size_type first, size_type last = size()) const; // qt
		inline iterator index_of(const_reference value, iterator first, iterator last = end()) const; // myself


		inline virtual void replace(const_iterator begin,
		                            const_iterator end,
		                            collection<value_type> &other); // swift
		inline virtual void remove_all(const_reference item); // qt
		inline virtual void remove_at(int i); // qt
		inline virtual void remove(const_reference item); // qt

		inline virtual std::vector<value_type, allocator_type> to_std_vector() const noexcept; // qt
		inline virtual std::list<value_type, allocator_type> to_std_list() const noexcept; // c#
		inline virtual std::set<value_type, allocator_type> to_std_set() const noexcept; // c#
		inline bool true_for_all(conformer conform); // c#

		inline virtual collection<value_type> &subsequence(const_iterator from, const_iterator to) = 0;
		inline void swap(size_type from, size_type to);
	protected:
		iterator _begin;
		iterator _end;
	private:
		allocator_type _allocator;
	};

	template<class T, class Allocator>
	typename collection<T, Allocator>::allocator_type collection<T, Allocator>::Allocator() const noexcept {
		return _allocator;
	}

	template<class T, class Allocator>
	bool collection<T, Allocator>::contains(const_reference item) const {
		for (const_reference element: *this)
			if (item == element)
				return true;
		return false;
	}

	template<class T, class Allocator>
	typename collection<T, Allocator>::size_type collection<T, Allocator>::count(const_reference item) const noexcept {
		size_type __count = 0;
		for (const_reference element: *this)
			if (item == element)
				++__count;
		return __count;
	}

	template<class T, class Allocator>
	void collection<T, Allocator>::foreach(collection::action act) {
		for (reference element: *this)
			act(element);
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::size_type collection<T, Allocator>::index_of(const_reference value,
	                                                                                size_type first,
	                                                                                size_type last) const {
		for (; first != last && _begin[first] != value; ++first) {}
		return first;
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::iterator collection<T, Allocator>::index_of(const_reference value,
	                                                                               iterator first,
	                                                                               iterator last) const {
		for (; first != last && *first != value; ++first) {}
		return first;
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::replace(collection::const_iterator begin,
	                                       collection::const_iterator end,
	                                       collection<value_type> &other) {
		_NOT_IMPLEMENTED_;
	}

	template<class T, class Allocator>
	void collection<T, Allocator>::remove_all(const_reference item) {
		for (iterator it = begin(); it != end(); ++it)
			if (*it == item)
				erase(it);
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::remove_at(int i) {
		erase(begin() + i);
	}

	template<class T, class Allocator>
	void collection<T, Allocator>::remove(const_reference item) {
		for (iterator it = begin(); it != end(); ++it)
			if (*it == item) {
				erase(it);
				break;
			}
	}

	template<class T, class Allocator>
	std::vector<typename collection<T, Allocator>::value_type,
	            typename collection<T, Allocator>::allocator_type> collection<T,
	                                                                          Allocator>::to_std_vector() const noexcept {
		return std::vector<T>(_begin, _end);
	}
	template<class T, class Allocator>
	std::list<typename collection<T, Allocator>::value_type,
	          typename collection<T, Allocator>::allocator_type> collection<T,
	                                                                        Allocator>::to_std_list() const noexcept {
		return std::list<T>(_begin, _end);
	}
	template<class T, class Allocator>
	std::set<typename collection<T, Allocator>::value_type,
	         typename collection<T, Allocator>::allocator_type> collection<T, Allocator>::to_std_set() const noexcept {
		return std::set<T>(_begin, _end);
	}
	template<class T, class Allocator>
	bool collection<T, Allocator>::true_for_all(conformer conform) {
		for (const_reference element: *this)
			if (!conform(element))
				return false;
		return true;
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::swap(collection::size_type from, collection::size_type to) {
		using std::swap;
		swap(_begin[from], _begin[to]);
	}
}

#endif //COLLECTIONS_COLLECTION_HPP
