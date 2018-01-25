//
// Created by Artem Bobrov on 23.01.2018.
//

#ifndef CONTAINERS_EXTREME_ELEMENT_ACCESSIBLE_MODIFIABLE_HPP
#define CONTAINERS_EXTREME_ELEMENT_ACCESSIBLE_MODIFIABLE_HPP

#include <utility>
#include <iterator>

namespace ctl {
	template<class T, class Iterator>
	class top_element_accessible_modifiable {
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
		virtual void push(value_type &&value) = 0;

		virtual void pop() = 0;
	};
}
#endif //CONTAINERS_EXTREME_ELEMENT_ACCESSIBLE_MODIFIABLE_HPP
