//
// Created by Artem Bobrov on 01.01.2018.
//

#ifndef COLLECTIONS_OBJECT_HPP
#define COLLECTIONS_OBJECT_HPP

#include <iostream>
#include <string>

#define __NOT_IMPLEMENTED__ throw std::logic_error("Function not yet implemented")

namespace ctl {
	class object {
	public:
		virtual explicit operator std::string() const noexcept = 0; // c#

		friend std::ostream &operator<<(std::ostream &os, const object &obj) { // OK
			os << std::string(obj);
			return os;
		}
	};
}

#endif //COLLECTIONS_OBJECT_HPP
