//
// Created by Artem Bobrov on 19.03.2018.
//

#ifndef CONTAINERS_RESERVABLE_HPP
#define CONTAINERS_RESERVABLE_HPP
#include <memory>
#include <functional>
#include <iterator>
#include "../base/object.hpp"
namespace ctl {
	class revervable : public virtual object {
	public:
		typedef size_t size_type;
	public:
		inline virtual void reserve(size_type n) = 0; // stl

		inline virtual void shrink_to_fit() noexcept = 0; // stl
	};
}
#endif //CONTAINERS_RESERVABLE_HPP
