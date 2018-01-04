//
// Created by Artem Bobrov on 01.01.2018.
//

#ifndef COLLECTIONS_RESERVABLE_HPP
#define COLLECTIONS_RESERVABLE_HPP

#include <memory>
#include <functional>
#include <iterator>

namespace ctl {
	template<class T>
	class reservable {
	public:
		typedef T value_type;
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;
	public:
		inline virtual size_type capacity() const noexcept = 0; // stl 

		inline virtual bool empty() const noexcept = 0; // stl 

		inline virtual size_type max_size() const noexcept; // stl 

		inline virtual void reserve(size_type n) = 0; // stl

		inline virtual size_type size() const noexcept = 0; // stl 
		inline virtual void shrink_to_fit() noexcept = 0; // stl
	};
	template<class T>
	typename reservable<T>::size_type reservable<T>::max_size() const noexcept {
		return std::numeric_limits<size_type>::max();
	}

}

#endif //COLLECTIONS_RESERVABLE_HPP
