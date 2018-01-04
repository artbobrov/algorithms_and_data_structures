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
		: public object,
		  public element_accessible<T>,
		  public iterable<T>,
		  public reservable<T>,
		  public modifiable<T, Allocator> {
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
		typedef std::function<void(const_reference)> action;
		typedef std::function<reference(reference)> map_action;
		typedef std::function<bool(const_reference, const_reference)> comparer;
	public:
		inline allocator_type allocator() const noexcept; // stl

		inline bool contains(const_reference item) const; // qt 
		inline size_type count(const_reference item) const noexcept; // qt 
		inline const_reverse_iterator crbegin() const noexcept override; // stl
		inline const_reverse_iterator crend() const noexcept override; // stl

		inline void foreach(action act); // c#
		inline virtual collection<value_type, allocator_type> &filter(conformer conform) = 0;
		inline void fill(const T &value); // qt 
		inline void fill(iterator first, iterator last, const T &value); // qt 
		inline void fill(const T &value, size_type size); // qt

		inline size_type index_of(const_reference value, size_type first, size_type last) const; // qt 
		inline iterator index_of(const_reference value, iterator first, iterator last) const; // myself 

		inline void map(map_action mapper);

		inline void reverse(); // c# 
		inline void reverse(iterator first, iterator last); // c# 

		inline virtual void remove_all(const_reference item); // qt 
		inline virtual void remove_at(int i); // qt 
		inline virtual void remove(const_reference item); // qt 
		inline reverse_iterator rbegin() noexcept override; // 
		inline const_reverse_iterator rbegin() const noexcept override; // 
		inline reverse_iterator rend() noexcept override; // 
		inline const_reverse_iterator rend() const noexcept override; // 

		inline virtual std::vector<value_type, allocator_type> to_std_vector() const noexcept; // qt 
		inline virtual std::list<value_type, allocator_type> to_std_list() const noexcept; // c# 
		inline virtual std::set<value_type> to_std_set() const noexcept; // c# 
		inline bool true_for_all(conformer conform); // c#

		inline virtual collection<value_type, allocator_type> &subsequence(const_iterator from, const_iterator to) = 0;
		inline virtual collection<value_type, allocator_type> &subsequence(size_type from, size_type to) = 0;
	protected:
		allocator_type _allocator;
	};

	template<class T, class Allocator>
	typename collection<T, Allocator>::allocator_type collection<T, Allocator>::allocator() const noexcept {
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
	typename collection<T, Allocator>::const_reverse_iterator collection<T, Allocator>::crbegin() const noexcept {
		return const_reverse_iterator(this->end());
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::const_reverse_iterator collection<T, Allocator>::crend() const noexcept {
		return const_reverse_iterator(this->begin());
	}

	template<class T, class Allocator>
	void collection<T, Allocator>::foreach(action act) {
		for (reference element: *this)
			act(element);
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::fill(const T &value) {
		fill(this->begin(), this->end(), value);
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::fill(iterator first, iterator last, const T &value) {
		for (; first != last; ++first)
			*first = value;
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::fill(const T &value, size_type size) {
		resize(size, value);
	}

	template<class T, class Allocator>
	typename collection<T, Allocator>::size_type collection<T, Allocator>::index_of(const_reference value,
	                                                                                size_type first,
	                                                                                size_type last) const {
		for (; first != last && this[first] != value; ++first) {}
		return first == last ? this->size() : first;
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::iterator collection<T, Allocator>::index_of(const_reference value,
	                                                                               iterator first,
	                                                                               iterator last) const {
		for (; first != last && *first != value; ++first) {}
		return first == last ? this->end() : first;
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::reverse() {
		reverse(this->begin(), this->end() - 1);
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::reverse(iterator first, iterator last) {
		using std::swap;

		for (; first < last; ++first, --last) {
			swap(*first, *last);
		}
	}

	template<class T, class Allocator>
	void collection<T, Allocator>::remove_all(const_reference item) {
		for (iterator it = this->begin(); it != this->end(); ++it)
			if (*it == item)
				this->erase(it);
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::remove_at(int i) {
		this->erase(this->begin() + i);
	}

	template<class T, class Allocator>
	void collection<T, Allocator>::remove(const_reference item) {
		for (iterator it = this->begin(); it != this->end(); ++it)
			if (*it == item) {
				this->erase(it);
				break;
			}
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::reverse_iterator collection<T, Allocator>::rbegin() noexcept {
		return reverse_iterator(this->end());
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::const_reverse_iterator collection<T, Allocator>::rbegin() const noexcept {
		return reverse_iterator(this->end());
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::reverse_iterator collection<T, Allocator>::rend() noexcept {
		return reverse_iterator(this->begin());
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::const_reverse_iterator collection<T, Allocator>::rend() const noexcept {
		return reverse_iterator(this->begin());
	}

	template<class T, class Allocator>
	std::vector<typename collection<T, Allocator>::value_type,
	            typename collection<T, Allocator>::allocator_type> collection<T,
	                                                                          Allocator>::to_std_vector() const noexcept {
		return std::vector<value_type, allocator_type>(this->begin(), this->end());
	}
	template<class T, class Allocator>
	std::list<typename collection<T, Allocator>::value_type,
	          typename collection<T, Allocator>::allocator_type> collection<T,
	                                                                        Allocator>::to_std_list() const noexcept {
		return std::list<value_type, allocator_type>(this->begin(), this->end());
	}
	template<class T, class Allocator>
	std::set<typename collection<T, Allocator>::value_type> collection<T, Allocator>::to_std_set() const noexcept {
		return std::set<value_type>(this->begin(), this->end());
	}
	template<class T, class Allocator>
	bool collection<T, Allocator>::true_for_all(conformer conform) {
		for (const_reference element: *this)
			if (!conform(element))
				return false;
		return true;
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::map(map_action mapper) {
		for (reference element: *this)
			element = mapper(element);
	}

}

#endif //COLLECTIONS_COLLECTION_HPP
