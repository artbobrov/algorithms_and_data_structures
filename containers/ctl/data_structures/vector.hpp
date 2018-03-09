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
		typedef const value_type &const_reference;

		typedef value_type *data;
		typedef size_t size_type;
	public: // properties
		data data_point;
	public: // methods
		explicit __vector_iterator(data point = nullptr) : data_point(point) {}

		__vector_iterator(const __vector_iterator &other) : data_point(other.data_point) {}

		__vector_iterator &operator=(const __vector_iterator &other) {
			if (this != &other) {
				data_point = other.data_point;
			}
			return *this;
		}

		__vector_iterator &operator=(__vector_iterator &&other) noexcept {
			data_point = other.data_point;
			other.data_point = nullptr;
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
		friend inline size_type operator-(const __vector_iterator &__x, const __vector_iterator &__y) {
			return static_cast<size_type>(__x.data_point - __y.data_point);
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
	template<class T>
	inline __vector_iterator<T> operator+(typename __vector_iterator<T>::size_type idx,
	                                      const __vector_iterator<T> &__x) {
		return __x + idx;
	}

	template<class T, class Allocator = std::allocator<T>>
	class vector : public collection<T, __vector_iterator<T>, Allocator>,
	               public reservable, /* TODO: add or remove comparable<...> ... */
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
		typedef const value_type &const_reference;
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
		template<class Iterator, typename = typename std::enable_if<is_forward_iterator<Iterator>::value>::type>
		inline void _construct_at_end(Iterator begin,
		                              Iterator end); // with default contructor
		inline void _allocate(size_type n);
	public:
		inline explicit vector(const Allocator &alloc = Allocator())
			: vector::collection(alloc), _begin(nullptr), _end(nullptr), _storage_end(nullptr) {}

		inline explicit vector(size_type count, const Allocator &alloc = Allocator());
		inline explicit vector(size_type count, const T &value, const Allocator &alloc = Allocator());

		template<class Iterator>
		inline explicit vector(Iterator begin, Iterator end, const Allocator &alloc = Allocator(),
		                       typename std::enable_if<is_forward_iterator<Iterator>::value>::type * = 0);

		inline vector(const vector<value_type, allocator_type> &other) :
			vector(other.begin(), other.end(), other.allocator()) {}

		inline vector(const vector<value_type, allocator_type> &other, const Allocator &alloc) :
			vector(other.begin(), other.end(), alloc) {}

		inline vector(vector &&other) noexcept;

		template<class Container, typename std::enable_if<has_begin_end<Container>::value>::type * = 0>
		inline vector(const Container &__container): vector(__container.begin(), __container.end()) {}

		inline vector(std::initializer_list<value_type> il, const Allocator &alloc = Allocator()) :
			vector(il.begin(), il.end(), alloc) {}

		inline virtual ~vector();
	public:
		inline void append(element_accessible_modifiable<value_type, iterator> &other) override;
		inline reference at(size_type i) override;
		inline const_reference at(size_type i) const override;

		inline reference back() override;
		inline const_reference back() const override;
		inline iterator begin() noexcept override;
		inline const_iterator begin() const noexcept override;

		inline const_iterator cbegin() const noexcept override;
		inline const_iterator cend() const noexcept override;
		inline const_reverse_iterator crbegin() const noexcept override;
		inline const_reverse_iterator crend() const noexcept override;
		inline size_type capacity() const noexcept override;
		inline void clear() noexcept override;

		inline sequence<std::pair<size_type, iterator>, iterator> enumerated() const noexcept override;
		inline iterator erase(const_iterator position) override;
		inline iterator erase(size_type position) override;
		inline iterator erase(const_iterator first, const_iterator last) override;
		inline iterator erase(size_type first, size_type last) override;
		inline iterator end() noexcept override;
		inline const_iterator end() const noexcept override;

		inline void fill(iterator first, iterator last, const T &value) override;
		inline void fill(const T &value, size_type size) override;
		inline const sequence<value_type, iterator> &filter(conformer predicate) override;
		inline void filtered(conformer predicate) override;
		inline reference front() override;
		inline const_reference front() const override;

		inline iterator insert(const_iterator before, const T &value) override;
		inline iterator insert(size_type i, const T &value) override;
		inline iterator insert(const_iterator before, value_type &&value) override;
		inline iterator insert(size_type i, value_type &&value) override;
		inline iterator insert(const_iterator before, size_type count, const T &value) override;
		inline iterator insert(size_type i, size_type count, const T &value) override;
		inline iterator insert(const_iterator position, std::initializer_list<value_type> il) override;
		inline iterator insert(size_type i, std::initializer_list<value_type> il) override;

		template<typename U>
		inline sequence<U, iterator> map(std::function<U(reference)> mapper);

		inline void pop_back() override;
		inline void pop_front() override;
		inline void push_back(const_reference value) override;
		inline void push_back(value_type &&value) override;
		inline void push_front(const_reference value) override;
		inline void push_front(value_type &&value) override;
		inline const sequence<value_type, iterator> &prefix(size_type max_length) override;
		inline const sequence<value_type, iterator> &prefix(conformer predicate) override;

		inline void reserve(size_type n) override;
		inline void remove_all(const_reference item) override;
		inline void remove_at(int i) override;
		inline void remove(const_reference item) override;
		inline void resize(size_type count) override;
		inline void resize(size_type count, const value_type &value) override;
		template<typename R>
		inline sequence<R, iterator> reduce(R initial, std::function<R(reference)> next_result);
		inline reverse_iterator rbegin() noexcept override;
		inline const_reverse_iterator rbegin() const noexcept override;
		inline reverse_iterator rend() noexcept override;
		inline const_reverse_iterator rend() const noexcept override;

		inline collection<value_type, iterator, allocator_type> &subsequence(iterator from, iterator to) override;
		inline collection<value_type, iterator, allocator_type> &subsequence(size_type from, size_type to) override;
		inline const sequence<value_type, iterator> &suffix(size_type max_length) override;
		inline const sequence<value_type, iterator> &suffix(conformer predicate) override;
		inline void shrink_to_fit() noexcept override;
		inline unsigned long size() const noexcept override;
		inline void swap(modifiable<value_type> &other) override;
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
	template<class Iterator, typename>
	void vector<T, Allocator>::_construct_at_end(Iterator begin, Iterator end) {
		for (; begin != end; ++begin, ++_end)
			this->_allocator.construct(_end.data_point, *begin);
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::_allocate(size_type n) {
		if (n > this->max_size())
			throw std::length_error("vector");
		_begin.data_point = _end.data_point = this->_allocator.allocate(n);
		_storage_end = _begin + n;
	}
	template<class T, class Allocator>
	vector<T, Allocator>::vector(vector::size_type count, const Allocator &alloc): vector(alloc) {
		if (count > 0) {
			_allocate(count);
			_construct_at_end(count);
		}
	}
	template<class T, class Allocator>
	vector<T, Allocator>::vector(vector::size_type count, const T &value, const Allocator &alloc): vector(alloc) {
		if (count > 0) {
			_allocate(count);
			_construct_at_end(count, value);
		}
	}
	template<class T, class Allocator>
	template<class Iterator>
	vector<T, Allocator>::vector(Iterator begin,
	                             Iterator end,
	                             const Allocator &alloc,
	                             typename std::enable_if<is_forward_iterator<Iterator>::value>::type *):
		vector(alloc) {
		size_type count = end - begin;
		if (count > 0) {
			_allocate(count);
			_construct_at_end(begin, end);
		}
	}
	template<class T, class Allocator>
	vector<T, Allocator>::vector(vector &&other) noexcept: vector(std::move(other.allocator())) {
		_begin = other.begin();
		_end = other.end();
		_storage_end = other._storage_end;
		other._begin = other._end = other._storage_end = nullptr;
	}
	template<class T, class Allocator>
	vector<T, Allocator>::~vector() {
		if (_begin.data_point != nullptr) {
			_deallocate_data(_begin, _storage_end);
		}
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::at(size_type i) {
		if (i < capacity() && i > size())
			_end = _begin + i;
		return *(_begin + i);
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::at(size_type i) const {
		return *(_begin + i);
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::append(element_accessible_modifiable<value_type, iterator> &other) {
		if (size() + other.size() > capacity()) {
			size_type __capacity = _recommend(size() + other.size());
			_allocate(__capacity);
		}

		_construct_at_end(other.begin(), other.end());
	}
}
#endif //CONTAINERS_VECTOR_HPP
