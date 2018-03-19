//
// Created by Artem Bobrov on 15.01.2018.
//

#ifndef CONTAINERS_RESIZABLE_HPP
#define CONTAINERS_RESIZABLE_HPP

#include <iterator>
#include <memory>
#include "../base/object.hpp"
namespace ctl {
	template<class T>
	class resizable : public virtual object {
	public:
		typedef T value_type;
		typedef value_type &reference;
		typedef value_type const &const_reference;
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;
	public:
		inline virtual void resize(size_type count) = 0; // stl
		inline virtual void resize(size_type count, const T &value) = 0; // stl
	};
}

#endif //CONTAINERS_RESIZABLE_HPP
