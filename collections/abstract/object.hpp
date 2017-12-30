//
// Created by Artem Bobrov on 27.12.2017.
//

#ifndef COLLECTIONS_OBJECT_HPP
#define COLLECTIONS_OBJECT_HPP

#include <iostream>
#include <string>
#define _NOT_IMPLEMENTED_ throw std::logic_error("Function not yet implemented")
namespace ctl {
	class object {
	public:
		typedef int size_type;
	public:
		virtual explicit operator std::string() const noexcept = 0; // c#

		// TODO: something using std::hash
		inline virtual size_type hash() const = 0; // c#

		friend std::ostream &operator<<(std::ostream &os, const object &obj) {
			os << std::string(obj);
			return os;
		}
	};
}

#endif //COLLECTIONS_OBJECT_HPP
