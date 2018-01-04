//
// Created by Artem Bobrov on 01.01.2018.
//

#ifndef COLLECTIONS_ENUMERABLE_HPP
#define COLLECTIONS_ENUMERABLE_HPP

#include <functional>
#include "object.hpp"

namespace ctl {

	template<class T>
	class iterable {
	public:
		typedef T value_type;
		typedef value_type *iterator;
		typedef const iterator const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef const std::reverse_iterator<iterator> const_reverse_iterator;
		typedef size_t size_type;
	public:
		inline virtual iterator begin() noexcept = 0; // stl
		inline virtual const_iterator begin() const noexcept = 0; // stl

		inline virtual const_iterator cbegin() const noexcept = 0; // stl
		inline virtual const_iterator cend() const noexcept = 0; // stl
		inline virtual const_reverse_iterator crbegin() const noexcept = 0; // stl
		inline virtual const_reverse_iterator crend() const noexcept = 0; // stl

		inline size_type distance(const_iterator first, const_iterator last); // swift

		inline virtual iterator end() noexcept = 0; // stl
		inline virtual const_iterator end() const noexcept = 0; // stl

		inline virtual reverse_iterator rbegin() noexcept = 0; // stl
		inline virtual const_reverse_iterator rbegin() const noexcept = 0; // stl
		inline virtual reverse_iterator rend() noexcept = 0; // stl
		inline virtual const_reverse_iterator rend() const noexcept = 0; // stl
	};
	template<class T>
	typename iterable<T>::size_type iterable<T>::distance(const_iterator first, const_iterator last) {
		return static_cast<size_type>(first - last);
	}
}

#endif //COLLECTIONS_ENUMERABLE_HPP