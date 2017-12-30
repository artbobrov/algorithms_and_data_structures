//
// Created by Artem Bobrov on 28.12.2017.
//

#ifndef COLLECTIONS_IENUMERABLE_HPP
#define COLLECTIONS_IENUMERABLE_HPP

#include <iterator>
#include <memory>
namespace ctl {

	template<class T>
	class enumerable {
	public:
		typedef T value_type;
		typedef value_type *iterator;
		typedef const iterator const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef const std::reverse_iterator<iterator> const_reverse_iterator;
		typedef iterator pointer;
		typedef value_type &reference;
		typedef const reference const_reference;
		typedef object::size_type size_type;
		typedef std::ptrdiff_t difference_type;

		typedef std::function<bool(const_reference)> conformer;

	public:
		inline virtual reference at(size_type i) = 0; // qt
		inline virtual const_reference at(size_type i) const = 0; // qt

		inline virtual size_type distance(const_iterator from, const_iterator to) = 0; // swift

		inline virtual iterator begin() noexcept = 0; // stl
		inline virtual iterator end() noexcept = 0; // stl
		inline virtual const_iterator begin() const noexcept = 0; // stl
		inline virtual const_iterator end() const noexcept = 0; // stl
		inline virtual const_iterator cbegin() const noexcept = 0; // stl
		inline virtual const_iterator cend() const noexcept = 0; // stl
		inline virtual reverse_iterator rbegin() noexcept = 0; // stl
		inline virtual reverse_iterator rend() noexcept = 0; // stl
		inline virtual const_reverse_iterator rbegin() const noexcept = 0; // stl
		inline virtual const_reverse_iterator rend() const noexcept = 0; // stl
		inline virtual const_reverse_iterator crbegin() const noexcept = 0; // stl
		inline virtual const_reverse_iterator crend() const noexcept = 0; // stl

		inline virtual reference front() = 0; // stl
		inline virtual const_reference front() const = 0; // stl
		inline virtual reference back() = 0; // stl
		inline virtual const_reference back() const = 0; // stl

		inline virtual void reverse() = 0; // c#
		inline virtual enumerable<value_type> &reversed() = 0; // c#

		inline bool true_for_all(conformer conform); // c#

		inline const_reference operator[](size_type i) const; // stl
		inline reference operator[](size_type i); // stl

		inline virtual enumerable<value_type> &prefix(size_type count) = 0; // swift
		inline virtual enumerable<value_type> &prefix(conformer conform) = 0; // swift (while ok)

		inline virtual enumerable<value_type> &suffix(size_type count) = 0; // swift
		inline virtual enumerable<value_type> &suffix(conformer conform) = 0; // swift (while ok)

		inline virtual void swap(size_type from, size_type to) = 0;
	public:
		inline virtual bool operator!=(const enumerable<value_type> &other) const = 0; // qt
		inline virtual bool operator==(const enumerable<value_type> &other) const = 0; // qt
	};

	template<class T>
	inline bool enumerable<T>::true_for_all(conformer conform) {
		for (const_reference element: *this)
			if (!conform(element))
				return false;
		return true;
	}

	template<class T>
	inline typename enumerable<T>::const_reference enumerable<T>::operator[](size_type i) const {
		return this->at(i);
	}

	template<class T>
	inline typename enumerable<T>::reference enumerable<T>::operator[](size_type i) {
		return this->at(i);
	}

}

#endif //COLLECTIONS_IENUMERABLE_HPP
