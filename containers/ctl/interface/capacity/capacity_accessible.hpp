//
// Created by Artem Bobrov on 01.01.2018.
//

#ifndef COLLECTIONS_RESERVABLE_HPP
#define COLLECTIONS_RESERVABLE_HPP

#include <memory>
#include <functional>
#include <iterator>

#include "size_accessible.hpp"
#include "../base/object.hpp"
namespace ctl {
	class capacity_accessible : public virtual object {
	public:
		typedef size_t size_type;
	public:
		inline virtual size_type capacity() const noexcept = 0;
	};
}

#endif //COLLECTIONS_RESERVABLE_HPP
