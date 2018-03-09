//
// Created by Artem Bobrov on 08.03.2018.
//

#ifndef CONTAINERS_VECTOR_HPP
#define CONTAINERS_VECTOR_HPP

#include "../interface/interfaces.hpp"
#include "../abstract/collection.hpp"
#include "../abstract/sequence.hpp"
namespace ctl {

	template<class T>
	struct __vector_iterator : public std::iterator<std::random_access_iterator_tag, T> {
		typedef T value_type;
		typedef value_type &reference;
		typedef value_type const &const_reference;

		typedef value_type *data;
		typedef size_t size_type;
	public: // properties
		mutable data data_point;
	public: // methods
		explicit __vector_iterator(data point = nullptr) : data_point(point) {}

		__vector_iterator(const __vector_iterator &other) : data_point(other.data_point) {}

		__vector_iterator &operator=(const __vector_iterator &other) {
			if (this != &other) {
				data_point = other.data_point;
			}
			return *this;
		}

		inline bool operator==(const __vector_iterator &other) { return other.data_point == data_point; }
		inline bool operator!=(const __vector_iterator &other) { return !(*this == other); }

		inline reference operator*() { return *data_point; }
		inline const_reference operator*() const { return *data_point; }
		inline T *operator->() { return &(operator*()); }

		inline __vector_iterator &operator++() {
			++data_point;
			return *this;
		}
		inline __vector_iterator operator++(int) {
			__vector_iterator value = *this;
			++*this;
			return value;
		}
		inline __vector_iterator &operator--() {
			--data_point;
			return *this;
		}
		inline __vector_iterator operator--(int) {
			__vector_iterator value = *this;
			--*this;
			return value;
		}

		inline __vector_iterator &plus(size_type idx) {
			data_point += idx;
			return *this;
		}
		inline __vector_iterator &minus(size_type idx) {
			data_point -= idx;
			return *this;
		}

		inline size_type operator-(__vector_iterator other) {
			return static_cast<size_type>(data_point - other.data_point);
		}

		inline bool operator<(const __vector_iterator &rhs) {
			return data_point < rhs.data_point;
		}
		inline bool operator>(const __vector_iterator &rhs) {
			return rhs < *this;
		}
		inline bool operator<=(const __vector_iterator &rhs) {
			return !(rhs < *this);
		}
		inline bool operator>=(const __vector_iterator &rhs) {
			return !(*this < rhs);
		}

	};

	template<class T>
	inline __vector_iterator<T> operator+(const __vector_iterator<T> &__x,
	                                      typename __vector_iterator<T>::size_type idx) {
		auto value = __x;
		value.plus(idx);
		return value;
	}
	template<class T>
	inline __vector_iterator<T> operator-(const __vector_iterator<T> &__x,
	                                      typename __vector_iterator<T>::size_type idx) {
		auto value = __x;
		value.minus(idx);
		return value;
	}

	template<class T, class Allocator = std::allocator<T>>
	class vector : public collection<T, __vector_iterator<T>, Allocator>,
	               public reservable<T>, /* TODO: add or remove comparable<...> ... */
	               public random_element_accessible<T>,
	               public modifiable<T>,
	               public resizable<T>,
	               public element_accessible_modifiable<T, __vector_iterator<T>> {
	public:
		typedef Allocator allocator_type;
		typedef T value_type;
		typedef __vector_iterator<T> iterator;
		typedef const iterator const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef const std::reverse_iterator<iterator> const_reverse_iterator;
		typedef T *pointer;
		typedef value_type &reference;
		typedef value_type const &const_reference;
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef std::function<bool(const_reference)> conformer;
		typedef std::function<void(reference)> action;
		typedef std::function<bool(const_reference, const_reference)> comparer;
	private:
		iterator _begin, _end, _storage_end;
	private:
		inline void _deallocate_data(iterator from, iterator to);
		inline iterator _copy_data(iterator begin, iterator end, iterator to); // returns end iterator to copied data
		inline size_type _recommend(size_type new_size);
		inline void _destruct_at_end(iterator new_end);
		inline void _construct_at_end(size_type n); // with default contructor
		inline void _construct_at_end(size_type n, const_reference __x); // with default contructor
		inline void _allocate(size_type n);
	public:
		inline vector() noexcept : vector(Allocator()) {}
		inline explicit vector(const Allocator &alloc)
			: vector::collection(alloc), _begin(nullptr), _end(nullptr), _storage_end(nullptr) {}

		inline explicit vector(size_type count, const Allocator &alloc = Allocator());
		inline explicit vector(size_type count, const T &value, const Allocator &alloc = Allocator());

		template<class Iterator>
		inline explicit vector(Iterator begin, Iterator end, const Allocator &alloc = Allocator(),
		                       typename std::enable_if<is_random_access_iterator<Iterator>::value>::type * = 0);
		template<class Iterator>
		inline explicit vector(Iterator begin, Iterator end, const Allocator &alloc = Allocator(),
		                       typename std::enable_if<is_input_iterator<Iterator>::value
			                                               && !is_random_access_iterator<Iterator>::value>::type * = 0);

		inline vector(const vector<value_type, allocator_type> &other);
		inline vector(vector &&other) noexcept;

		template<class Container, typename = typename std::enable_if<has_begin_end<Container>::value>::type>
		inline vector(const Container &__container): vector(__container.begin(), __container.end()) {}

		inline explicit vector(const vector<value_type, allocator_type> &other, const Allocator &alloc);

		inline vector(std::initializer_list<value_type> il, const Allocator &alloc = Allocator()) :
			vector(il.begin(), il.end(), alloc) {}

		inline virtual ~vector();
	public:
		void fill(iterator first, iterator last, const T &value) override;
		void fill(const T &value, size_type size) override;
		collection<value_type, iterator, allocator_type> &subsequence(iterator from, iterator to) override;
		collection<value_type, iterator, allocator_type> &subsequence(size_type from, size_type to) override;

		template<typename R>
		inline sequence<R, iterator> reduce(R initial, std::function<R(reference)> next_result);

		template<typename U>
		inline sequence<U, iterator> map(std::function<U(reference)> mapper);

		sequence<std::pair<size_type, iterator>, iterator> enumerated() const noexcept override;
		const sequence<value_type, iterator> &filter(conformer predicate) override;
		void filtered(conformer predicate) override;
		const sequence<value_type, iterator> &prefix(size_type max_length) override;
		const sequence<value_type, iterator> &prefix(conformer predicate) override;
		const sequence<value_type, iterator> &suffix(size_type max_length) override;
		const sequence<value_type, iterator> &suffix(conformer predicate) override;
		size_type capacity() const noexcept override;
		void reserve(size_type n) override;
		void shrink_to_fit() noexcept override;
		unsigned long size() const noexcept override;
		T &back() override;
		const T &back() const override;
		T &front() override;
		const T &front() const override;
		T &at(size_type i) override;
		const T &at(size_type i) const override;
		iterator begin() noexcept override;
		const_iterator begin() const noexcept override;
		const_iterator cbegin() const noexcept override;
		const_iterator cend() const noexcept override;
		const_reverse_iterator crbegin() const noexcept override;
		const_reverse_iterator crend() const noexcept override;
		iterator end() noexcept override;
		const_iterator end() const noexcept override;
		reverse_iterator rbegin() noexcept override;
		const_reverse_iterator rbegin() const noexcept override;
		reverse_iterator rend() noexcept override;
		const_reverse_iterator rend() const noexcept override;

		void append(modifiable<value_type> &value) override;
		iterator erase(const_iterator position) override;
		iterator erase(size_type position) override;
		iterator erase(const_iterator first, const_iterator last) override;
		iterator erase(size_type first, size_type last) override;
		iterator insert(const_iterator before, const T &value) override;
		iterator insert(size_type i, const T &value) override;
		iterator insert(const_iterator before, value_type &&value) override;
		iterator insert(size_type i, value_type &&value) override;
		iterator insert(const_iterator before, size_type count, const T &value) override;
		iterator insert(size_type i, size_type count, const T &value) override;
		iterator insert(const_iterator position, std::initializer_list<value_type> il) override;
		iterator insert(size_type i, std::initializer_list<value_type> il) override;
		void pop_back() override;
		void pop_front() override;
		void push_back(const_reference value) override;
		void push_back(value_type &&value) override;
		void push_front(const_reference value) override;
		void push_front(value_type &&value) override;
		void remove_all(const_reference item) override;
		void remove_at(int i) override;
		void remove(const_reference item) override;
		void resize(size_type count) override;
		void resize(size_type count, const value_type &value) override;
		void clear() noexcept override;
		void swap(modifiable<value_type> &other) override;
	};

	template<class T, class Allocator>
	void vector<T, Allocator>::_deallocate_data(iterator from, iterator to) {

		iterator _start = from;
		size_type _sz = this->distance(to, from);
		for (; from != to; ++from)
			this->_allocator.destroy(from.data_point);
		if (capacity() && _begin.data_point != nullptr)
			this->_allocator.deallocate(_start.data_point, _sz);
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::_copy_data(iterator begin,
	                                                                         iterator end,
	                                                                         iterator to) {
		for (; begin != end; ++begin, ++to)
			this->_allocator.construct(to.data_point, *begin);
		return to;
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::size_type vector<T, Allocator>::_recommend(size_type new_size) {
		size_type _max_size = this->max_size();
		if (_max_size < new_size)
			throw std::length_error("vector is too large");
		if (capacity() > _max_size / 2)
			return _max_size;
		return std::max(2 * capacity(), new_size);
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::_destruct_at_end(iterator new_end) {
		while (new_end != _end)
			this->_allocator.destroy(--_end);
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::_construct_at_end(size_type n) {
		while (n-- > 0) {
			this->_allocator.contruct(_end++);
		}
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::_construct_at_end(size_type n, const_reference __x) {
		while (n-- > 0) {
			this->_allocator.contruct(_end++, __x);
		}
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::_allocate(size_type n) {
		if (n > this->max_size())
			throw std::length_error("vector");
		_begin = _end = _allocator.allocate(n);
		_storage_end = _begin + n;
	}
}
#endif //CONTAINERS_VECTOR_HPP
