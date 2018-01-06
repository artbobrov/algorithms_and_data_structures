//
// Created by Artem Bobrov on 06.01.2018.
//

#ifndef COLLECTIONS_ITERATOR_HPP
#define COLLECTIONS_ITERATOR_HPP

#include <iterator>
namespace ctl {

	template<class Category, class T>
	class __iterator : public std::iterator<Category, T> {
		typedef T value_type;
		typedef value_type &reference;
		typedef value_type const &const_reference;
		typedef size_t size_type;
	public:
		inline virtual __iterator &operator++() = 0;
		inline virtual __iterator &operator--() = 0;
		inline virtual T &operator*() = 0;
		inline virtual T *operator->() = 0;

		inline virtual bool operator==(const __iterator &rhs) = 0;
		inline virtual bool operator!=(const __iterator &rhs) {
			return !(*this == rhs);
		}

		inline virtual __iterator& plus(size_type idx) = 0;
		inline virtual __iterator& minus(size_type idx) = 0;
	};

}

#endif //COLLECTIONS_ITERATOR_HPP
