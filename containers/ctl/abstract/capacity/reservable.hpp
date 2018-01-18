//
// Created by Artem Bobrov on 01.01.2018.
//

#ifndef COLLECTIONS_RESERVABLE_HPP
#define COLLECTIONS_RESERVABLE_HPP

#include <memory>
#include <functional>
#include <iterator>

#include "size_accessible.hpp"

namespace ctl {
	template<class T>
	class iterable: public size_accessible<T> {
	public:
		typedef size_t size_type;
	public:
		inline virtual void reserve(size_type n) = 0; // stl

		inline virtual void shrink_to_fit() noexcept = 0; // stl
	};
}

#endif //COLLECTIONS_RESERVABLE_HPP
