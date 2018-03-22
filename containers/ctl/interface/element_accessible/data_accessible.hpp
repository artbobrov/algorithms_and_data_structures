//
// Created by Artem Bobrov on 14.01.2018.
//

#ifndef CONTAINERS_DATA_ACCESSIBLE_H
#define CONTAINERS_DATA_ACCESSIBLE_H
#include "../base/object.hpp"
namespace ctl {
	template<class T>
	class data_accessible: public virtual object {
	public:
		typedef T *pointer;
	public:
		inline virtual pointer data() noexcept = 0; // stl
		inline virtual const pointer data() const noexcept = 0; // stl
	};
}

#endif //CONTAINERS_DATA_ACCESSIBLE_H
