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
	public:
		typedef Allocator allocator_type;
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
		inline virtual collection() {}
		inline virtual collection(const collection<value_type, allocator_type> &other) = 0;
		inline virtual collection(collection<value_type, allocator_type> &&other) = 0;
		template<class InputIterator>
		inline virtual collection(InputIterator first, InputIterator last, const Allocator &alloc = Allocator()) = 0;

		inline virtual ~collection();
	public:
		inline virtual void append(const collection<value_type> &value) override;
		inline allocator_type allocator() const noexcept;

		inline iterator begin() noexcept override;
		inline const_iterator begin() const noexcept override;

		inline bool contains(const_reference item) const; // qt
		inline size_type count(const_reference item) const noexcept; // qt
		inline const_iterator cbegin() const noexcept override;
		inline const_iterator cend() const noexcept override;
		inline const_reverse_iterator crbegin() const noexcept override;
		inline const_reverse_iterator crend() const noexcept override;

		inline iterator end() noexcept override;
		inline const_iterator end() const noexcept override;

		inline void foreach(action act); // c#
		inline void fill(const_reference value); // qt
		inline void fill(const_reference value, size_type size); // qt
		inline collection<value_type> &filled(const_reference value); // qt+swift
		inline collection<value_type> &filled(const_reference value, size_type size); // qt+swift

		inline size_type index_of(const_reference value, size_type first, size_type last = size()) const; // qt
		inline iterator index_of(const_reference value, iterator first, iterator last = end()) const; // myself

		inline void reverse(); // c#
		inline collection<value_type, allocator_type> &reversed(); // c#

		// TODO: mb should use template<class InputIterator> for replacement
		inline virtual void replace(iterator begin, iterator end, const collection<value_type> &other); // swift
		inline virtual void remove_all(const_reference item); // qt
		inline virtual void remove_at(int i); // qt
		inline virtual void remove(const_reference item); // qt
		inline reverse_iterator rbegin() noexcept override;
		inline const_reverse_iterator rbegin() const noexcept override;
		inline reverse_iterator rend() noexcept override;
		inline const_reverse_iterator rend() const noexcept override;

		inline virtual std::vector<value_type, allocator_type> to_std_vector() const noexcept; // qt
		inline virtual std::list<value_type, allocator_type> to_std_list() const noexcept; // c#
		inline virtual std::set<value_type, allocator_type> to_std_set() const noexcept; // c#
		inline bool true_for_all(conformer conform); // c#

		inline virtual collection<value_type> &subsequence(const_iterator from, const_iterator to);
		inline virtual collection<value_type> &subsequence(size_type from, size_type to);
		inline void swap(size_type from, size_type to);
	protected:
		iterator _begin = nullptr;
		iterator _end = nullptr;
	private:
		inline void _deallocate_data();
		inline iterator _copy_data(iterator begin, iterator end, iterator to); // returns end iterator to copied data
	private:
		allocator_type _allocator;
	};
	template<class T, class Allocator>
	collection<T, Allocator>::~collection() {
		_deallocate_data();
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::_deallocate_data() {
		// TODO: CHECK with objects(mb foreach destroy?)
		if (_capacity && _begin != nullptr)
			_allocator.deallocate(_begin, _capacity);
	}
	template<class T, class Allocator>
	collection::iterator collection<T, Allocator>::_copy_data(iterator begin, iterator end, iterator to) {
		for (; begin != end; ++begin, ++to)
			_allocator.construct(to, *begin);

		return to;
	}

	template<class T, class Allocator>
	typename collection<T, Allocator>::allocator_type collection<T, Allocator>::allocator() const noexcept {
		return _allocator;
	}

	template<class T, class Allocator>
	typename collection<T, Allocator>::iterator collection<T, Allocator>::begin() noexcept {
		return _begin;
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::const_iterator collection<T, Allocator>::begin() const noexcept {
		return _begin;
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
	typename collection<T, Allocator>::const_iterator collection<T, Allocator>::cbegin() const noexcept {
		return _begin;
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::const_iterator collection<T, Allocator>::cend() const noexcept {
		return _end;
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::const_reverse_iterator collection<T, Allocator>::crbegin() const noexcept {
		return const_reverse_iterator(_end);
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::const_reverse_iterator collection<T, Allocator>::crend() const noexcept {
		return const_reverse_iterator(_begin);
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::iterator collection<T, Allocator>::end() noexcept {
		return _end;
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::const_iterator collection<T, Allocator>::end() const noexcept {
		return _end;
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::foreach(action act) {
		for (reference element: *this)
			act(element);
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::fill(const_reference value) {
		for (reference element: *this)
			element = value;
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::fill(const_reference value, size_type size) {
		resize(size, value);
	}
	template<class T, class Allocator>
	collection<typename collection<T, Allocator>::value_type> &collection<T, Allocator>::filled(const_reference value) {
		collection<value_type, allocator_type> other(*this);
		other.fill(value);
		return other;
	}
	template<class T, class Allocator>
	collection<value_type> &collection<T, Allocator>::filled(const_reference value, size_type size) {
		collection<value_type, allocator_type> other(*this);
		other.fill(value, size);
		return other;
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
	void collection<T, Allocator>::reverse() {
		for (size_type left = 0, right = size(); left < right; ++left, --right)
			swap(left, right);
	}
	template<class T, class Allocator>
	collection<value_type, allocator_type> &collection<T, Allocator>::reversed() {
		collection<value_type, allocator_type> other(*this);
		other.reverse();
		return other;
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::replace(iterator begin,
	                                       iterator end,
	                                       const collection<value_type> &other) {
		size_type idx = 0;
		for (; begin != end && idx < other.size(); ++idx, ++begin)
			*begin = other[idx];

		if (idx == other.size())
			erase(begin, end);
		else if (begin == end)
			insert(end, other.begin() + idx, other.end());
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
	typename collection<T, Allocator>::reverse_iterator collection<T, Allocator>::rbegin() noexcept {
		return reverse_iterator(_end);
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::const_reverse_iterator collection<T, Allocator>::rbegin() const noexcept {
		return reverse_iterator(_end);
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::reverse_iterator collection<T, Allocator>::rend() noexcept {
		return reverse_iterator(_begin);
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::const_reverse_iterator collection<T, Allocator>::rend() const noexcept {
		return reverse_iterator(_begin);
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
	collection<value_type> &collection<T, Allocator>::subsequence(const_iterator from,
	                                                              const_iterator to) {
		collection<value_type, allocator_type> other(from, to);
		return other;
	}
	template<class T, class Allocator>
	collection<value_type> &collection<T, Allocator>::subsequence(size_type from,
	                                                              size_type to) {
		collection<value_type, allocator_type> other(begin() + from, begin() + to);
		return other;
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::swap(size_type from, size_type to) {
		using std::swap;
		swap(_begin[from], _begin[to]);
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::append(const collection<value_type> &value) {
		replace(end(), end(), value);
	}
}

#endif //COLLECTIONS_COLLECTION_HPP
