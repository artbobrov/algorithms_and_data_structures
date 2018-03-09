//
// Created by Artem Bobrov on 15.01.2018.
//

#ifndef CONTAINERS_MODIFIABLE_HPP
#define CONTAINERS_MODIFIABLE_HPP

#include <functional>
#include <iterator>
#include <memory>

namespace ctl {
	template<class T>
	class modifiable {
	public:
		inline virtual void clear() noexcept = 0; // stl

		inline virtual void swap(modifiable &other) = 0; // stl
	};
}

#endif //CONTAINERS_MODIFIABLE_HPP
