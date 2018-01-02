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
		inline virtual size_type capacity() const noexcept; // stl OK

		inline virtual bool empty() const noexcept; // stl OK

		inline virtual difference_type max_size() const noexcept; // stl OK

		inline virtual void reserve(size_type n) = 0; // stl

		inline virtual size_type size() const noexcept; // stl OK
		inline virtual void shrink_to_fit() noexcept = 0; // stl
	protected:
		size_type _capacity = 0;
		size_type _size = 0;
	};
	template<class T>
	typename reservable<T>::size_type reservable<T>::capacity() const noexcept {
		return _capacity;
	}
	template<class T>
	bool reservable<T>::empty() const noexcept {
		return !_size;
	}
	template<class T>
	typename reservable<T>::difference_type reservable<T>::max_size() const noexcept {
		return std::numeric_limits<difference_type>::max();
	}
	template<class T>
	typename reservable<T>::size_type reservable<T>::size() const noexcept {
		return _size;
	}

}

#endif //COLLECTIONS_RESERVABLE_HPP
