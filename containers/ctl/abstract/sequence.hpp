//
// Created by Artem Bobrov on 23.01.2018.
//

#ifndef CONTAINERS_SEQUENCE_HPP
#define CONTAINERS_SEQUENCE_HPP

#include "../interface/interfaces.hpp"

namespace ctl {
	template<class T, class Iterator>
	class sequence : public object, public iterable<Iterator> {
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
		typedef std::function<void(const_reference)> action;
		typedef std::function<reference(reference)> map_action;
		typedef std::function<bool(const_reference, const_reference)> comparer;
	public:
		inline bool contains(const_reference item) const; // qt
		inline bool contains(conformer predicate) const; // qt

		inline size_type count(const_reference item) const noexcept; // qt
		inline size_type count_if(conformer predicate) const noexcept; // qt


		inline iterator first_if(conformer predicate) const noexcept; // qt
		inline void for_each(action act); // c#

		inline void map(map_action mapper);
		template<typename = typename std::enable_if<is_comparable<T, T>::value>::type>
		inline iterator min();
		inline iterator min(comparer comp);
		template<typename = typename std::enable_if<is_comparable<T, T>::value>::type>
		inline iterator max();
		inline iterator max(comparer comp);

		virtual const sequence &filter(conformer predicate) = 0;

		virtual const sequence &prefix(size_type max_length) = 0;
		virtual const sequence &prefix(conformer predicate) = 0;

		virtual const sequence &suffix(size_type max_length) = 0;
		virtual const sequence &suffix(conformer predicate) = 0;

		inline void reverse(); // c#
		inline virtual void reverse(iterator first, iterator last) = 0; // c#

		inline virtual sequence &subsequence(iterator from,
		                                    iterator to) = 0; // swift

		inline bool true_for_all(conformer conform); // c#
	};
	template<class T, class Iterator>
	bool sequence<T, Iterator>::contains(const T &item) const {
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
	typename sequence<T,Iterator>::size_type sequence<T, Iterator>::count(const T &item) const noexcept {
		size_type _count = 0;
		for (const_reference element: *this)
			if (element == item)
				++_count;
		return _count;
	}
	template<class T, class Iterator>
	typename sequence<T,Iterator>::size_type sequence<T, Iterator>::count_if(conformer predicate) const noexcept {
		size_type _count = 0;
		for (const_reference element: *this)
			if (predicate(element))
				++_count;
		return _count;
	}
	template<class T, class Iterator>
	typename sequence<T,Iterator>::iterator sequence<T, Iterator>::first_if(conformer predicate) const noexcept {
		iterator _iter = this->begin();
		for (; _iter != this->end() && !predicate(*_iter); ++_iter) {}
		return _iter;
	}
	template<class T, class Iterator>
	void sequence<T, Iterator>::for_each(action act) {
		for (const_reference element: *this)
			act(element);
	}
	template<class T, class Iterator>
	void sequence<T, Iterator>::map(map_action mapper) {
		for (reference element: *this)
			element = mapper(element);
	}
	template<class T, class Iterator>
	template<typename>
	typename sequence<T,Iterator>::iterator sequence<T, Iterator>::min() {
		iterator _min = this->begin(), _iter = this->begin();
		for (; _iter != this->end(); ++_iter)
			if (*_iter < *_min)
				_min = _iter;

		return _min;
	}
	template<class T, class Iterator>
	typename sequence<T,Iterator>::iterator sequence<T, Iterator>::min(comparer comp) {
		iterator _min = this->begin(), _iter = this->begin();
		for (; _iter != this->end(); ++_iter)
			if (!comp(*_iter, *_min))
				_min = _iter;

		return _min;
	}
	template<class T, class Iterator>
	template<typename>
	typename sequence<T,Iterator>::iterator sequence<T, Iterator>::max() {
		iterator _max = this->begin(), _iter = this->begin();
		for (; _iter != this->end(); ++_iter)
			if (*_iter > *_max)
				_max = _iter;

		return _max;
	}
	template<class T, class Iterator>
	typename sequence<T,Iterator>::iterator sequence<T, Iterator>::max(comparer comp) {
		iterator _max = this->begin(), _iter = this->begin();
		for (; _iter != this->end(); ++_iter)
			if (comp(*_iter, *_max))
				_max = _iter;

		return _max;
	}
	template<class T, class Iterator>
	bool sequence<T, Iterator>::true_for_all(sequence::conformer conform) {
		for (const_reference element: *this)
			if (!conform(element))
				return false;
		return true;
	}
	template<class T, class Iterator>
	void sequence<T, Iterator>::reverse() {
		reverse(this->begin(), this->end());
	}
}

#endif //CONTAINERS_SEQUENCE_HPP
