//
// Created by Artem Bobrov on 01.01.2018.
//

#ifndef COLLECTIONS_ENUMERABLE_HPP
#define COLLECTIONS_ENUMERABLE_HPP

#include <functional>
#include "../base/object.hpp"

namespace ctl {

	template<class Iterator>
	class iterable {
	public:
		typedef Iterator iterator;
		typedef const Iterator const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef const std::reverse_iterator<iterator> const_reverse_iterator;
		typedef size_t size_type;
	public:
		inline virtual iterator begin() noexcept = 0; // stl
		inline virtual const_iterator begin() const noexcept = 0; // stl

		inline const_iterator cbegin() const noexcept { return begin(); }; // stl
		inline const_iterator cend() const noexcept { return end(); }; // stl
		inline const_reverse_iterator crbegin() const noexcept { return rbegin(); }; // stl
		inline const_reverse_iterator crend() const noexcept { return rend(); } // stl

		inline virtual iterator end() noexcept = 0; // stl
		inline virtual const_iterator end() const noexcept = 0; // stl

		inline reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }; // stl
		inline const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }; // stl
		inline reverse_iterator rend() noexcept { return reverse_iterator(begin()); }; // stl
		inline const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }; // stl
	};
}

#endif //COLLECTIONS_ENUMERABLE_HPP
