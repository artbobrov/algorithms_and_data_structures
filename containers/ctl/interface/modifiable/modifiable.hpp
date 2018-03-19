//
// Created by Artem Bobrov on 15.01.2018.
//

#ifndef CONTAINERS_MODIFIABLE_HPP
#define CONTAINERS_MODIFIABLE_HPP

#include <functional>
#include <iterator>
#include <memory>
#include "../base/object.hpp"
namespace ctl {
	class modifiable: public virtual object {
	public:
		inline virtual void clear() noexcept = 0; // stl
	};
}

#endif //CONTAINERS_MODIFIABLE_HPP
