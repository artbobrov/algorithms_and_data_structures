//
// Created by Artem Bobrov on 23.01.2018.
//

#ifndef CONTAINERS_EXTREME_ELEMENT_ACCESSIBLE_MODIFIABLE_HPP
#define CONTAINERS_EXTREME_ELEMENT_ACCESSIBLE_MODIFIABLE_HPP

#include <utility>
#include <iterator>

namespace ctl {
	template<class T>
	class top_element_accessible_modifiable {
	public:
		typedef T value_type;
	public:
		virtual void push(value_type &&value) = 0;

		virtual void pop() = 0;
	};
}
#endif //CONTAINERS_EXTREME_ELEMENT_ACCESSIBLE_MODIFIABLE_HPP
