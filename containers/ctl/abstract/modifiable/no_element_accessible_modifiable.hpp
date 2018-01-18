//
// Created by Artem Bobrov on 18.01.2018.
//

#ifndef CONTAINERS_NO_ELEMENT_ACCESSIBLE_MODIFIABLE_HPP
#define CONTAINERS_NO_ELEMENT_ACCESSIBLE_MODIFIABLE_HPP

#include <utility>

#include "modifiable.hpp"
namespace ctl {
	template<class T, class Iterator>
	class no_element_accessible_modifiable : public modifiable<T> {
	public:
		typedef T value_type;
		typedef Iterator iterator;
		typedef const iterator const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef const std::reverse_iterator<iterator> const_reverse_iterator;
		typedef iterator pointer;
		typedef value_type &reference;
		typedef value_type const &const_reference;
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;
	public:
		inline virtual iterator erase(iterator position) = 0;
		inline virtual iterator erase(const_iterator first, const_iterator last) = 0;

		inline virtual std::pair<iterator, bool> insert(value_type &&value) = 0;
		inline virtual void insert(std::initializer_list<value_type> ilist) = 0;
	};
}
#endif //CONTAINERS_NO_ELEMENT_ACCESSIBLE_MODIFIABLE_HPP
