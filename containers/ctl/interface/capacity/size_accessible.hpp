//
// Created by Artem Bobrov on 14.01.2018.
//

#ifndef CONTAINERS_SIZE_ACCESSIBLE_HPP
#define CONTAINERS_SIZE_ACCESSIBLE_HPP

#include <memory>
#include <functional>

namespace ctl {
	class size_accessible {
	public:
		typedef size_t size_type;
	public:
		inline virtual bool empty() const noexcept { return !size(); } // stl
		inline virtual size_type max_size() const noexcept { return std::numeric_limits<size_type>::max(); } // stl
		inline virtual size_type size() const noexcept = 0; // stl
	};
}
#endif //CONTAINERS_SIZE_ACCESSIBLE_HPP
