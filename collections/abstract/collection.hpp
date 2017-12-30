//
// Created by Artem Bobrov on 26.12.2017.
//

#ifndef COLLECTIONS_ICOLLECTION_HPP
#define COLLECTIONS_ICOLLECTION_HPP

#include <memory>
#include <functional>
#include <vector>
#include <list>
#include <set>

#include "object.hpp"
#include "resizable.hpp"
#include "enumerable.hpp"

namespace ctl {

	template<class T>
	class collection : public enumerable<T>, public resizable<T> {
	public:
		typedef typename enumerable<T>::value_type value_type;
		typedef typename enumerable<T>::iterator iterator;
		typedef typename enumerable<T>::const_iterator const_iterator;
		typedef typename enumerable<T>::reverse_iterator reverse_iterator;
		typedef typename enumerable<T>::const_reverse_iterator const_reverse_iterator;
		typedef typename enumerable<T>::pointer pointer;
		typedef typename enumerable<T>::reference reference;
		typedef typename enumerable<T>::const_reference const_reference;
		typedef typename object::size_type size_type;
		typedef typename enumerable<T>::difference_type difference_type;

		typedef typename enumerable<T>::conformer conformer;
		typedef std::function<void(reference)> action;
		typedef std::function<bool(const_reference, const_reference)> comparer;
	public:
		virtual void append(const collection<value_type> &value) = 0; // qt

		inline bool contains(const_reference item) const; // qt
		inline int count(const_reference item) const noexcept; // qt
		inline virtual size_type count() const noexcept = 0; // qt
		inline virtual size_type size() const noexcept = 0; // stl
		inline virtual size_type capacity() const noexcept = 0; // stl

		inline virtual pointer data() noexcept = 0; // stl
		inline virtual const pointer data() const noexcept = 0; // stl
		inline virtual collection<value_type> &distinct(comparer comp) = 0; // c#

		inline virtual bool empty() const noexcept = 0; // stl
		inline virtual iterator erase(iterator pos) = 0; // stl
		inline virtual iterator erase(iterator begin, iterator end) = 0; // stl
		inline virtual collection<value_type> &except(const collection<value_type> &other) = 0; // c#

		inline void foreach(action act); // c#

		inline void fill(const_reference value, size_type size = -1); // qt
		inline void fill(const value_type value, size_type size = -1); // qt
		inline collection<value_type> &filled(const_reference value, size_type size = -1); // qt
		inline collection<value_type> &filled(const value_type value, size_type size = -1); // qt

		inline size_type index(const_reference of, size_type begin, size_type end) const; // qt
		inline iterator index(const_reference of, iterator begin, iterator end) const; // myself

		inline virtual collection<value_type> &concat(const enumerable<value_type> &other) = 0; // c# == x + y


		inline virtual void resize(size_type sz) = 0; // stl
		inline virtual void resize(size_type sz, const_reference c) = 0; // stl

		inline virtual void replace(const_iterator begin,
		                            const_iterator end,
		                            enumerable<value_type> &other) = 0; // swift
		inline virtual void remove_all(const_reference item) = 0; // qt
		inline virtual void remove_at(int i) = 0; // qt
		inline virtual void remove(const_reference item) = 0; // qt

		inline virtual void shrink_to_fit() noexcept = 0; // stl
		inline virtual void swap(collection<value_type> &other) = 0; // qt


		inline virtual difference_type max_size() const noexcept = 0; // stl

		// TODO: function by default
		// TODO: function specialization for int/double/uint/long long ect.
		inline virtual value_type min(const_reference max_value, comparer comp) = 0; // c#
		inline virtual value_type min(iterator begin,
		                              iterator end,
		                              const_reference max_value,
		                              comparer comp) = 0; // +c#

		inline virtual value_type max(const_reference min_value, comparer comp) = 0; // c#
		inline virtual value_type max(iterator begin,
		                              iterator end,
		                              const_reference min_value,
		                              comparer comp) = 0; // +c#

		inline virtual value_type sum(const_reference zero_value) = 0; // c#
		inline virtual value_type sum(iterator begin,
		                              iterator end,
		                              const_reference zero_value) = 0; // +c#


		inline collection<collection<value_type> > split(const_reference element,
		                                                 int max_splits,
		                                                 bool omitting_empty_subsequences); // swift
		inline collection<collection<value_type> > split(int max_splits,
		                                                 bool omitting_empty_subsequences,
		                                                 conformer is_separator); // swift

		inline virtual std::vector<value_type> to_std_vector() const noexcept = 0; // qt
		inline virtual std::list<value_type> to_std_list() const noexcept = 0; // c#
		inline virtual std::set<value_type> to_std_set() const noexcept = 0; // c#

	private:
		const size_type _empty_flag_int_ = -1;
		const_iterator _empty_flag_iterator_ = nullptr;
	};

	template<class T>
	inline void collection<T>::foreach(action act) {
		for (reference element: *this)
			act(element);
	}

	template<class T>
	inline void collection<T>::fill(const_reference value, size_type size) {
		if (size > -1)
			this->resize(size);
		for (reference element: *this)
			element = value;
	}
	template<class T>
	inline void collection<T>::fill(const value_type value, size_type size) {
		if (size > -1)
			this->resize(size);
		for (reference element: *this)
			element = value;
	}
	template<class T>
	inline collection<typename collection<T>::value_type> &collection<T>::filled(const_reference value, size_type size) {
		_NOT_IMPLEMENTED_;
	}
	template<class T>
	inline collection<typename collection<T>::value_type> &collection<T>::filled(const value_type value, size_type size) {
		_NOT_IMPLEMENTED_;
	}

	template<class T>
	inline typename collection<T>::size_type collection<T>::index(const_reference of,
	                                                              size_type begin,
	                                                              size_type end) const {
		for (size_type i = begin; i <= end; ++i)
			if (of == this[i])
				return i;

		return _empty_flag_int_;
	}

	template<class T>
	inline typename collection<T>::iterator collection<T>::index(const_reference of,
	                                                             iterator begin,
	                                                             iterator end) const {
		for (iterator it = begin; it != end; ++it) {
			if (of == *it)
				return it;
		}
		return _empty_flag_iterator_;
	}

	template<class T>
	inline bool collection<T>::contains(const_reference item) const {
		for (const_reference element: *this)
			if (element == item)
				return true;
		return false;
	}

	template<class T>
	inline int collection<T>::count(const_reference item) const noexcept {
		size_type count = 0;
		for (const_reference element: *this)
			if (item == element)
				++count;
		return ++count;
	}

	template<class T>
	inline collection<collection<typename collection<T>::value_type> > collection<T>::split(const_reference element,
	                                                                                        int max_splits,
	                                                                                        bool omitting_empty_subsequences) {
		_NOT_IMPLEMENTED_;
	}

	template<class T>
	inline collection<collection<typename collection<T>::value_type> > collection<T>::split(int max_splits,
	                                                                                        bool omitting_empty_subsequences,
	                                                                                        conformer is_separator) {
		_NOT_IMPLEMENTED_;
	}
}
#endif //COLLECTIONS_ICOLLECTION_HPP
