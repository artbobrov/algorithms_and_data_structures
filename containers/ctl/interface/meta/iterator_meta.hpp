//
// Created by Artem Bobrov on 09.01.2018.
//

#ifndef CONTAINERS_ITERATOR_META_HPP
#define CONTAINERS_ITERATOR_META_HPP

#include <iterator>

namespace ctl {
	template<class T>
	struct has_begin_end {
	private:
		struct __no { char __x; char __xx; };

		template<class U, typename U::iterator (U::*)(), typename U::iterator (U::*)()>
		struct checker {};
	private:
		template<class U>
		static __no test(...);
		template<class U>
		static char test(checker<U, &U::begin, &U::end> *);
	public:
		static const bool value = sizeof(test<T>(0)) == sizeof(char);
	};

	template<class T>
	struct has_iterator_category {
	private:
		struct __no { char __x; char __xx; };
	private:
		template<class U>
		static __no test(...);
		template<class U>
		static char test(typename U::iterator_category *);
	public:
		static const bool value = sizeof(test<T>(0)) == sizeof(char);
	};

	template<class Iterator, class Category, bool = has_iterator_category<std::iterator_traits<Iterator> >::value>
	struct has_iterator_category_convertible_to
		: public std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category *, Category *> {
	};

	template<class Iterator, class Category>
	struct has_iterator_category_convertible_to<Iterator, Category, false> : public std::false_type {};

	template<class Iterator>
	struct is_input_iterator : public has_iterator_category_convertible_to<Iterator, std::input_iterator_tag> {};

	template<class Iterator>
	struct is_random_access_iterator : public has_iterator_category_convertible_to<Iterator,
	                                                                               std::random_access_iterator_tag> {
	};
	template<class Iterator>
	struct is_forward_iterator : public has_iterator_category_convertible_to<Iterator,
	                                                                         std::forward_iterator_tag> {
	};

}

#endif //CONTAINERS_ITERATOR_META_HPP
