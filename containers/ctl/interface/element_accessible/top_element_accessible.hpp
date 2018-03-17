//
// Created by Artem Bobrov on 23.01.2018.
//

#ifndef CONTAINERS_TOP_ELEMENT_ACCESSIBLE_HPP
#define CONTAINERS_TOP_ELEMENT_ACCESSIBLE_HPP
namespace ctl {
	template<class T>
	class top_element_accessible {
	public:
		typedef T value_type;
		typedef const value_type &const_reference;
	public:
		virtual const_reference top() const = 0;
	};
}
#endif //CONTAINERS_TOP_ELEMENT_ACCESSIBLE_HPP
