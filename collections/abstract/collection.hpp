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
		typedef const reference const_reference;
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef std::function<bool(const_reference)> conformer;
		typedef std::function<void(reference)> action;
		typedef std::function<bool(const_reference, const_reference)> comparer;
	public:
		inline collection() = default;
		//		inline collection(const collection<value_type, allocator_type> &other) = 0;
		//		inline collection(collection<value_type, allocator_type> &&other) = 0;
		//		template<class InputIterator>
		//		inline virtual collection(InputIterator first, InputIterator last, const Allocator &alloc = Allocator()) = 0;

		inline virtual ~collection();
	public:
		reference at(size_type i) override;
		const_reference at(size_type i) const override;
		inline virtual void append(const collection<value_type> &value) override;
		inline allocator_type allocator() const noexcept;

		inline iterator begin() noexcept override;
		inline const_iterator begin() const noexcept override;

		inline bool contains(const_reference item) const; // qt OK
		inline size_type count(const_reference item) const noexcept; // qt OK
		inline const_iterator cbegin() const noexcept override; // OK
		inline const_iterator cend() const noexcept override; // OK
		inline const_reverse_iterator crbegin() const noexcept override; // OK
		inline const_reverse_iterator crend() const noexcept override; // OK

		inline iterator end() noexcept override; // OK
		inline const_iterator end() const noexcept override; // OK

		inline void foreach(action act); // c# OK
		inline void fill(const_reference value); // qt OK
		inline void fill(const_reference value, size_type size); // qt
		inline collection<value_type> &filled(const_reference value); // qt+swift FAIL
		inline collection<value_type> &filled(const_reference value, size_type size); // qt+swift FAIL

		inline size_type index_of(const_reference value, size_type first, size_type last) const; // qt OK
		inline iterator index_of(const_reference value, iterator first, iterator last) const; // myself OK

		inline void reverse(); // c# OK
		inline collection<value_type, allocator_type> &reversed(); // c# FAIL

		// TODO: mb should use template<class InputIterator> for replacement
		inline virtual void replace(iterator begin, iterator end, const collection<value_type> &other); // swift FAIL
		inline virtual void remove_all(const_reference item); // qt FAIL
		inline virtual void remove_at(int i); // qt FAIL
		inline virtual void remove(const_reference item); // qt FAIL
		inline reverse_iterator rbegin() noexcept override; // OK
		inline const_reverse_iterator rbegin() const noexcept override; // OK
		inline reverse_iterator rend() noexcept override; // OK
		inline const_reverse_iterator rend() const noexcept override; // OK

		inline virtual std::vector<value_type, allocator_type> to_std_vector() const noexcept; // qt OK
		inline virtual std::list<value_type, allocator_type> to_std_list() const noexcept; // c# OK
		inline virtual std::set<value_type> to_std_set() const noexcept; // c# OK
		inline bool true_for_all(conformer conform); // c# OK

		inline virtual collection<value_type> &subsequence(const_iterator from, const_iterator to); // FAIL
		inline virtual collection<value_type> &subsequence(size_type from, size_type to); // FAIL
	protected:
		iterator _begin = nullptr;
		iterator _end = nullptr;
		allocator_type _allocator;
	private:
		inline void _deallocate_data();
		inline iterator _copy_data(iterator begin, iterator end, iterator to); // returns end iterator to copied data
	};
	template<class T, class Allocator>
	collection<T, Allocator>::~collection() {
		_deallocate_data();
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::_deallocate_data() {
		if (this->_capacity && _begin != nullptr)
			_allocator.deallocate(_begin, this->_capacity);
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::iterator collection<T, Allocator>::_copy_data(iterator begin,
	                                                                                 iterator end,
	                                                                                 iterator to) {
		for (; begin != end; ++begin, ++to)
			_allocator.construct(to, *begin);

		return to;
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::reference collection<T, Allocator>::at(collection::size_type i) {
		return _begin[i];
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::const_reference collection<T, Allocator>::at(collection::size_type i) const {
		return _begin[i];
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
	collection<typename collection<T, Allocator>::value_type> &collection<T, Allocator>::filled(const_reference value,
	                                                                                            size_type size) {
		collection<value_type, allocator_type> other(*this);
		other.fill(value, size);
		return other;
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::size_type collection<T, Allocator>::index_of(const_reference value,
	                                                                                size_type first,
	                                                                                size_type last) const {
		for (; first != last && _begin[first] != value; ++first) {}
		return first == last ? this->size() : first;
	}
	template<class T, class Allocator>
	typename collection<T, Allocator>::iterator collection<T, Allocator>::index_of(const_reference value,
	                                                                               iterator first,
	                                                                               iterator last) const {
		for (; first != last && *first != value; ++first) {}
		return first == last ? end() : first;
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::reverse() {
		using std::swap;

		for (iterator itl = _begin, itr = _end - 1; itl < itr; ++itl, --itr) {
			swap(*itl, *itr);
		}
	}
	template<class T, class Allocator>
	collection<typename collection<T, Allocator>::value_type,
	           typename collection<T, Allocator>::allocator_type> &collection<T, Allocator>::reversed() {
		_NOT_IMPLEMENTED_;
		return *this;
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::replace(iterator begin,
	                                       iterator end,
	                                       const collection<value_type> &other) {
		size_type idx = 0;
		for (; begin != end && idx < other.size(); ++idx, ++begin)
			*begin = other[idx];

		if (idx == other.size())
			this->erase(begin, end);
		else if (begin == end) { _NOT_IMPLEMENTED_; }
		//			this->insert(end, other.begin() + idx, other.end());
	}

	template<class T, class Allocator>
	void collection<T, Allocator>::remove_all(const_reference item) {
		for (iterator it = begin(); it != end(); ++it)
			if (*it == item)
				this->erase(it);
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::remove_at(int i) {
		this->erase(begin() + i);
	}

	template<class T, class Allocator>
	void collection<T, Allocator>::remove(const_reference item) {
		for (iterator it = begin(); it != end(); ++it)
			if (*it == item) {
				this->erase(it);
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
		return std::vector<value_type, allocator_type>(_begin, _end);
	}
	template<class T, class Allocator>
	std::list<typename collection<T, Allocator>::value_type,
	          typename collection<T, Allocator>::allocator_type> collection<T,
	                                                                        Allocator>::to_std_list() const noexcept {
		return std::list<value_type, allocator_type>(_begin, _end);
	}
	template<class T, class Allocator>
	std::set<typename collection<T, Allocator>::value_type> collection<T, Allocator>::to_std_set() const noexcept {
		return std::set<value_type>(_begin, _end);
	}
	template<class T, class Allocator>
	bool collection<T, Allocator>::true_for_all(conformer conform) {
		for (const_reference element: *this)
			if (!conform(element))
				return false;
		return true;
	}
	template<class T, class Allocator>
	collection<typename collection<T, Allocator>::value_type> &collection<T,
	                                                                      Allocator>::subsequence(const_iterator from,
	                                                                                              const_iterator to) {
		//		collection<value_type, allocator_type> other(from, to);
		_NOT_IMPLEMENTED_;
		return *this;
	}
	template<class T, class Allocator>
	collection<typename collection<T, Allocator>::value_type> &collection<T, Allocator>::subsequence(size_type from,
	                                                                                                 size_type to) {
		//		collection<value_type, allocator_type> other(begin() + from, begin() + to);
		_NOT_IMPLEMENTED_;
		return *this;
	}
	template<class T, class Allocator>
	void collection<T, Allocator>::append(const collection<value_type> &value) {
		replace(end(), end(), value);
	}
}

#endif //COLLECTIONS_COLLECTION_HPP
