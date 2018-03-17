//
// Created by Artem Bobrov on 23.01.2018.
//

#ifndef CONTAINERS_SEQUENCE_HPP
#define CONTAINERS_SEQUENCE_HPP

#include "../interface/interfaces.hpp"
#include "../interface/meta.hpp"
#include <memory>

namespace ctl {
	template<class T, class Iterator>
	class sequence : public virtual iterable<Iterator> {
	public:
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
	public:
		inline bool contains(const_reference item) const; // qt
		inline bool contains(conformer predicate) const; // qt

		inline difference_type count(const_reference item) const noexcept; // qt
		inline difference_type count(conformer predicate) const noexcept; // qt

		virtual inline void fill(size_type first, size_type last, const_reference value) = 0; // qt
		virtual inline void fill(iterator first, iterator last, const_reference value) = 0; // qt
		virtual inline void fill(const_reference value, size_type size) = 0; // qt

		inline void fill(const_reference value); // qt
		virtual void filter(conformer predicate) = 0;
		inline iterator first(conformer predicate) const noexcept; // qt
		inline iterator first(const_reference item) const noexcept; // qt
		inline void for_each(action act) noexcept; // c#

		template<typename = typename std::enable_if<is_comparable<T, T>::value>::type>
		inline iterator min() const;
		inline iterator min(comparer comp) const;

		template<typename = typename std::enable_if<is_comparable<T, T>::value>::type>
		inline iterator max() const;
		inline iterator max(comparer comp) const;

		inline bool true_for_all(conformer conform) const; // c#
		inline virtual std::vector<value_type> to_std_vector() const; // qt
		inline virtual std::list<value_type> to_std_list() const; // c#
		inline virtual std::set<value_type> to_std_set() const; // c#
	};
	template<class T, class Iterator>
	bool sequence<T, Iterator>::contains(const_reference item) const {
		for (const_reference element: *this)
			if (element == item)
				return true;
		return false;
	}
	template<class T, class Iterator>
	bool sequence<T, Iterator>::contains(conformer predicate) const {
		for (const_reference element: *this)
			if (predicate(element))
				return true;
		return false;
	}
	template<class T, class Iterator>
	typename sequence<T, Iterator>::difference_type sequence<T, Iterator>::count(const_reference item) const noexcept {
		difference_type _count = 0;
		for (const_reference element: *this)
			if (element == item)
				++_count;

		return _count;
	}
	template<class T, class Iterator>
	typename sequence<T, Iterator>::difference_type sequence<T,
	                                                         Iterator>::count(conformer predicate) const noexcept {
		difference_type _count = 0;
		for (const_reference element: *this)
			if (predicate(element))
				++_count;

		return _count;
	}
	template<class T, class Iterator>
	void sequence<T, Iterator>::fill(const_reference value) {
		for (iterator _it = this->begin(); _it != this->end(); ++_it)
			*_it = value;
	}

	template<class T, class Iterator>
	typename sequence<T, Iterator>::iterator sequence<T, Iterator>::first(conformer predicate) const noexcept {
		iterator _first = this->begin();
		for (; _first != this->end() && !predicate(*_first); ++_first) {}
		return _first;
	}
	template<class T, class Iterator>
	typename sequence<T, Iterator>::iterator sequence<T, Iterator>::first(const_reference item) const noexcept {
		iterator _first = this->begin();
		for (; _first != this->end() && *_first != item; ++_first) {}
		return _first;
	}

	template<class T, class Iterator>
	void sequence<T, Iterator>::for_each(action act) noexcept {
		for (reference element: *this)
			act(element);
	}

	template<class T, class Iterator>
	template<typename>
	typename sequence<T, Iterator>::sequence::iterator sequence<T, Iterator>::min() const {
		iterator _min_value = this->begin();
		for (iterator _it = this->begin(); _it != this->end(); ++_it)
			if (*_it < *_min_value)
				_min_value = _it;
		return _min_value;
	}
	template<class T, class Iterator>
	typename sequence<T, Iterator>::iterator sequence<T, Iterator>::min(comparer comp) const {
		iterator _min_value = this->begin();
		for (iterator _it = this->begin(); _it != this->end(); ++_it)
			if (comp(*_it, *_min_value))
				_min_value = _it;
		return _min_value;
	}
	template<class T, class Iterator>
	template<typename>
	typename sequence<T, Iterator>::iterator sequence<T, Iterator>::max() const {
		iterator _max_value = this->begin();
		for (iterator _it = this->begin(); _it != this->end(); ++_it)
			if (*_it > *_max_value)
				_max_value = _it;
		return _max_value;
	}

	template<class T, class Iterator>
	typename sequence<T, Iterator>::iterator sequence<T, Iterator>::max(comparer comp) const {
		iterator _min_value = this->begin();
		for (iterator _it = this->begin(); _it != this->end(); ++_it)
			if (comp(*_min_value, *_it))
				_min_value = _it;
		return _min_value;
	}
	template<class T, class Iterator>
	bool sequence<T, Iterator>::true_for_all(conformer conform) const {
		for (const_reference element: *this)
			if (!conform(element))
				return false;
		return true;
	}
	template<class T, class Iterator>
	std::vector<typename sequence<T, Iterator>::value_type> sequence<T, Iterator>::to_std_vector() const {
		return std::vector<value_type>(this->begin(), this->end());
	}
	template<class T, class Iterator>
	std::list<typename sequence<T, Iterator>::value_type> sequence<T, Iterator>::to_std_list() const {
		return std::list<value_type>(this->begin(), this->end());
	}
	template<class T, class Iterator>
	std::set<typename sequence<T, Iterator>::value_type> sequence<T, Iterator>::to_std_set() const {
		return std::set<value_type>(this->begin(), this->end());
	}
}

#endif //CONTAINERS_SEQUENCE_HPP
