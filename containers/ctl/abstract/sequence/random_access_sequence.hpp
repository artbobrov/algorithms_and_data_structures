//
// Created by Artem Bobrov on 23.01.2018.
//

#ifndef CONTAINERS_SEQUENCE_HPP
#define CONTAINERS_SEQUENCE_HPP

#include "../../interface/interfaces.hpp"
#include "../../interface/meta.hpp"
#include <memory>
#include "bidirectional_access_sequence.hpp"
namespace ctl {
	template<class T, class Iterator>
	class random_access_sequence : public virtual bidirectional_access_sequence<T, Iterator> {
	public:
		typedef T value_type;
		typedef Iterator iterator;
		typedef iterator const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef const std::reverse_iterator<iterator> const_reverse_iterator;
		typedef value_type &reference;
		typedef const value_type &const_reference;
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef std::function<bool(const_reference)> conformer;
		typedef std::function<void(reference)> action;
		typedef std::function<bool(const_reference, const_reference)> comparer;
	public:
		using bidirectional_access_sequence<value_type, iterator>::fill;
		inline void fill(size_type first, size_type last, const_reference value) {
			fill(this->begin() + first, this->begin() + last, value);
		} // qt
	};
}

#endif //CONTAINERS_SEQUENCE_HPP
