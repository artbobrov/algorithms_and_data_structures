//
// Created by Artem Bobrov on 08.03.2018.
//

#ifndef CONTAINERS_VECTOR_HPP
#define CONTAINERS_VECTOR_HPP

#include "../interface/interfaces.hpp"
#include "../abstract/collection/random_access_collection.hpp"
#include "../abstract/sequence/random_access_sequence.hpp"

#include <string>
#include <algorithm>

namespace ctl {

	template<class T>
	struct __vector_iterator : public std::iterator<std::random_access_iterator_tag, T> {
		typedef T value_type;
		typedef value_type &reference;
		typedef const value_type &const_reference;

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

		__vector_iterator &operator=(__vector_iterator &&other) noexcept {
			data_point = other.data_point;
			other.data_point = nullptr;
			return *this;
		}

		friend inline bool operator==(const __vector_iterator &__x, const __vector_iterator &__y) {
			return __x.data_point == __y.data_point;
		}
		friend inline bool operator!=(const __vector_iterator &__x, const __vector_iterator &__y) {
			return !(__x == __y);
		}

		inline reference operator*() { return *data_point; }
		inline const_reference operator*() const { return *data_point; }
		inline T *operator->() { return &(operator*()); }

		inline __vector_iterator &plus(size_type idx) {
			data_point += idx;
			return *this;
		}
		inline __vector_iterator &minus(size_type idx) {
			data_point -= idx;
			return *this;
		}
		inline __vector_iterator &operator++() {
			++data_point;
			return *this;
		}
		inline __vector_iterator operator++(int) {
			__vector_iterator value = *this;
			++*this;
			return value;
		}
		inline const __vector_iterator &operator++() const {
			++data_point;
			return *this;
		}
		inline const __vector_iterator operator++(int) const {
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

		friend inline long long operator-(const __vector_iterator &__x, const __vector_iterator &__y) {
			return static_cast<long long>(__x.data_point - __y.data_point);
		}
		inline __vector_iterator &operator+=(int j) {
			data_point += j;
			return *this;
		}
		inline __vector_iterator &operator-=(int j) {
			data_point -= j;
			return *this;
		}

		friend bool operator<(const __vector_iterator &lhs, const __vector_iterator &rhs) {
			return lhs.data_point < rhs.data_point;
		}
		friend bool operator>(const __vector_iterator &lhs, const __vector_iterator &rhs) {
			return rhs < lhs;
		}
		friend bool operator<=(const __vector_iterator &lhs, const __vector_iterator &rhs) {
			return !(rhs < lhs);
		}
		friend bool operator>=(const __vector_iterator &lhs, const __vector_iterator &rhs) {
			return !(lhs < rhs);
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
	class vector : public random_access_collection<T, __vector_iterator<T>, Allocator>,
	               public random_element_accessible_modifiable<T, __vector_iterator<T>>,
	               public revervable,
	               public data_accessible<T> {
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
	private: // memory methods
		inline void _allocate(size_type n);
		inline void _deallocate_data(iterator from, iterator to);
		inline void _destruct_at_end(iterator new_end);
		inline void _destruct(iterator first, iterator last);
		inline void _construct_at_end(size_type n); // with default contructor
		template<class Iterator, typename = typename std::enable_if<is_forward_iterator<Iterator>::value>::type>
		inline void _construct(Iterator first, Iterator last, const_reference __x);
		inline void _construct_at_end(size_type n, const_reference __x); // with copy contructor
		inline void _construct_at_end(const_reference __x); // with copy contructor
		template<class... Args>
		inline void _construct_at_end(Args &&...args);
		template<class Iterator, typename = typename std::enable_if<is_forward_iterator<Iterator>::value>::type>
		inline void _construct_at_end(Iterator begin,
		                              Iterator end); // with default contructor
		template<class Iterator, typename = typename std::enable_if<is_forward_iterator<Iterator>::value>::type>
		inline void _copy(Iterator first, Iterator last, const_reference __x);
		template<class InputIterator, class OutputIterator,
			typename = typename std::enable_if<is_forward_iterator<InputIterator>::value
				                                   && is_forward_iterator<OutputIterator>::value>::type>
		inline OutputIterator _copy(InputIterator first, InputIterator last, OutputIterator result);
	private: // support methods
		inline size_type _recommend(size_type new_size);
		inline iterator _move_right(iterator begin, iterator end, size_type offset);
	public:
		inline explicit vector(const Allocator &alloc = Allocator())
			: vector::random_access_collection(alloc), _begin(nullptr), _end(nullptr), _storage_end(nullptr) {}

		inline explicit vector(size_type count, const Allocator &alloc = Allocator());
		inline explicit vector(size_type count, const_reference value, const Allocator &alloc = Allocator());

		template<class Iterator>
		inline explicit vector(Iterator begin, Iterator end, const Allocator &alloc = Allocator(),
		                       typename std::enable_if<is_forward_iterator<Iterator>::value>::type * = 0);

		inline vector(const vector<value_type, allocator_type> &other) :
			vector(other.begin(), other.end(), other.allocator()) {}

		inline vector(const vector<value_type, allocator_type> &other, const Allocator &alloc) :
			vector(other.begin(), other.end(), alloc) {}

		inline vector(vector &&other) noexcept;

		template<class Container, typename = typename std::enable_if<has_begin_end<Container>::value
			                                                             && !std::is_convertible<Container,
			                                                                                     vector>::value>::type>
		vector(const Container &__container): vector(__container.begin(), __container.end()) {}

		inline vector(std::initializer_list<value_type> il, const Allocator &alloc = Allocator()) :
			vector(il.begin(), il.end(), alloc) {}

		inline virtual ~vector();
	public: // operators
		vector &operator=(const vector &other);
		vector &operator=(vector &&other) noexcept;

		explicit operator std::string() const noexcept override;
	public:
		template<typename R>
		inline R accumulate(R initial, R (*next_result)(R, const_reference));
		inline void append(bidirectional_element_accessible_modifiable<value_type, iterator> &other) override;
		inline reference at(size_type i) override;
		inline const_reference at(size_type i) const override;

		inline reference back() override { return *(_end - 1); }
		inline const_reference back() const override { return *(_end - 1); }
		inline iterator begin() noexcept override { return _begin; }
		inline const_iterator begin() const noexcept override { return _begin; }

		inline size_type capacity() const noexcept override { return static_cast<size_type>(_storage_end - _begin); }
		inline void clear() noexcept override { _destruct_at_end(_begin); }

		inline pointer data() noexcept override { return _begin.data_point; }
		inline pointer const data() const noexcept override { return _begin.data_point; }

		inline iterator erase(const_iterator position) override;
		inline iterator erase(size_type position) override { return erase(begin() + position); }
		inline iterator erase(const_iterator first, const_iterator last) override;
		inline iterator erase(size_type first, size_type last) override {
			return erase(begin() + first, begin() + last);
		}
		template<class... Args>
		inline reference emplace(const_iterator position, Args &&... args);
		template<class... Args>
		inline reference emplace(size_type idx, Args &&... args) {
			return emplace(begin() + idx, std::forward<Args>(args)...);
		}
		template<class... Args>
		inline reference emplace_back(Args &&... args);
		inline iterator end() noexcept override { return _end; }
		inline const_iterator end() const noexcept override { return _end; }

		using random_access_sequence<value_type, iterator>::fill;
		inline void fill(iterator first, iterator last, const_reference value) override;
		inline void fill(const_reference value, size_type size) override;
		inline void filter(conformer predicate) override;
		inline vector<value_type, allocator_type> filtered(conformer predicate);
		inline reference front() override { return *_begin; }
		inline const_reference front() const override { return *_begin; }

		inline iterator insert(const_iterator position, const_reference value) override {
			return insert(position, 1, value);
		}
		inline iterator insert(size_type idx, const_reference value) override { return insert(begin() + idx, value); }
		inline iterator insert(const_iterator position, value_type &&value) override;
		inline iterator insert(size_type idx, value_type &&value) override {
			return insert(begin() + idx, std::move(value));
		}
		inline iterator insert(const_iterator position, size_type count, const_reference value) override;
		inline iterator insert(size_type idx, size_type count, const_reference value) override {
			return insert(begin() + idx, count, value);
		}
		inline iterator insert(const_iterator position, std::initializer_list<value_type> il) override {
			return insert(position, il.begin(), il.end());
		}
		inline iterator insert(size_type idx, std::initializer_list<value_type> il) override {
			return insert(begin() + idx, il);
		}
		template<class Iterator, typename = typename std::enable_if<is_forward_iterator<Iterator>::value>::type>
		inline iterator insert(const_iterator position, Iterator first, Iterator last);
		template<class Iterator, typename = typename std::enable_if<is_forward_iterator<Iterator>::value>::type>
		inline iterator insert(size_type idx, Iterator first, Iterator last) {
			return insert(begin() + idx, first, last);
		}

		template<typename U>
		inline vector<U, allocator_type> map(std::function<U(reference)> mapper);

		inline void pop_back() override { pop_back(1); }
		inline void pop_back(size_type count) override { _destruct_at_end(_end - count); }
		inline void pop_front() override { pop_front(1); }
		inline void pop_front(size_type count) override;
		inline void push_back(const_reference value) override;
		inline void push_back(size_type count, const_reference value) override;
		inline void push_back(value_type &&value) override;
		inline void push_front(const_reference value) override;
		inline void push_front(value_type &&value) override;
		inline void push_front(size_type count, const_reference value) override;
		inline vector<value_type, allocator_type> prefix(size_type max_length);
		inline vector<value_type, allocator_type> prefix(conformer predicate);

		inline void reserve(size_type n) override;
		using random_element_accessible_modifiable<value_type, iterator>::remove_all;
		inline void remove_all(iterator first, iterator last, const_reference item) override;
		inline void remove_all(iterator first, iterator last, conformer predicate) override;
		inline void remove(const_reference item) override;
		inline void resize(size_type n) override;
		inline void resize(size_type n, const value_type &value) override;
		inline vector reversed();

		inline vector<value_type, allocator_type> subsequence(iterator from, iterator to);
		inline vector<value_type, allocator_type> subsequence(size_type from, size_type to) {
			return subsequence(begin() + from, begin() + to);
		}

		inline vector<value_type, allocator_type> suffix(size_type max_length);
		inline vector<value_type, allocator_type> suffix(conformer predicate);
		inline void shrink_to_fit() noexcept override;
		inline size_type size() const noexcept override { return static_cast<size_type>(ctl::distance(_begin, _end)); }
		inline void swap(vector<value_type, allocator_type> &other);
	};

	/*
	 * Non member functions: BEGIN
	 */
	template<class T, class Allocator>
	inline bool operator==(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs) {
		auto _collection_iter = lhs.begin();
		auto _container_iter = rhs.begin();

		for (; _collection_iter != lhs.end() && _container_iter != rhs.end();
		       ++_collection_iter, ++_container_iter) {
			if (*_collection_iter != *_container_iter)
				return false;
		}
		return _collection_iter == lhs.end() && _container_iter == rhs.end()
			&& lhs.size() == rhs.size();
	}

	template<class T, class Allocator>
	inline bool operator!=(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs) {
		return !(lhs == rhs);
	}

	template<class T, class Allocator>
	inline bool operator<(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs) {
		auto lfirst = lhs.begin(), llast = lhs.end();
		auto rfirst = rhs.begin(), rlast = rhs.end();
		for (; lfirst != llast && rfirst != rlast; ++lfirst, ++rfirst) {
			if (lfirst <= llast || *lfirst < *rfirst)
				return true;
			if (*rfirst < *lfirst)
				return false;
		}
		return false;
	}
	template<class T, class Allocator>
	inline bool operator>(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs) {
		return rhs < lhs;
	}
	template<class T, class Allocator>
	inline bool operator<=(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs) {
		return !(rhs < lhs);
	}
	template<class T, class Allocator>
	inline bool operator>=(const vector<T, Allocator> &lhs, const vector<T, Allocator> &rhs) {
		return !(lhs < rhs);
	}
	/*
	 * Non member functions: END
	 */

	template<class T, class Allocator>
	void vector<T, Allocator>::_allocate(size_type n) {
		if (n > this->max_size())
			throw std::length_error("ctl vector");
		_begin.data_point = _end.data_point = this->_allocator.allocate(n);
		_storage_end = _begin + n;
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::_deallocate_data(iterator from, iterator to) {
		size_type _sz = ctl::distance(to, from);
		this->_allocator.deallocate(from.data_point, _sz);
	}

	template<class T, class Allocator>
	void vector<T, Allocator>::_destruct_at_end(iterator new_end) {
		while (new_end != _end)
			this->_allocator.destroy(--_end.data_point);
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::_destruct(iterator first, iterator last) {
		while (first != last)
			this->_allocator.destroy(++first.data_point);
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::_construct_at_end(size_type n) {
		while (n-- > 0) {
			this->_allocator.construct((_end++).data_point);
		}
	}
	template<class T, class Allocator>
	template<class Iterator, typename>
	void vector<T, Allocator>::_construct(Iterator first, Iterator last, const_reference __x) {
		while (first != last)
			this->_allocator.construct((first++).data_point, __x);
	}
	template<class T, class Allocator>
	template<class Iterator, typename>
	void vector<T, Allocator>::_copy(Iterator first, Iterator last, const_reference __x) {
		while (first != last)
			*(first++) = __x;
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::_construct_at_end(size_type n, const_reference __x) {
		while (n-- > 0) {
			this->_allocator.construct((_end++).data_point, __x);
		}
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::_construct_at_end(const_reference __x) {
		this->_allocator.construct(_end++.data_point, __x);
	}
	template<class T, class Allocator>
	template<class Iterator, typename>
	void vector<T, Allocator>::_construct_at_end(Iterator begin, Iterator end) {
		for (; begin != end; ++begin, ++_end)
			this->_allocator.construct(_end.data_point, *begin);
	}

	template<class T, class Allocator>
	template<class... Args>
	void vector<T, Allocator>::_construct_at_end(Args &&... args) {
		this->_allocator.construct(_end++.data_point, std::forward<Args>(args)...);
	}
	template<class T, class Allocator>
	template<class InputIterator, class OutputIterator, typename>
	OutputIterator vector<T, Allocator>::_copy(InputIterator first,
	                                           InputIterator last,
	                                           OutputIterator result) {
		for (; first != last; ++result, ++first)
			*result = *first;
		return result;
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::size_type vector<T, Allocator>::_recommend(size_type new_size) {
		size_type _max_size = this->max_size();
		if (_max_size < new_size)
			throw std::length_error("ctl vector");
		if (capacity() > _max_size / 2)
			return _max_size;
		return std::max(2 * capacity(), new_size);
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::_move_right(iterator begin,
	                                                                          iterator end,
	                                                                          size_type offset) {
		iterator result = end + offset;
		for (; begin != end; --end) {
			iterator new_position = end + offset;

			*new_position = std::move(*end);
		}
		return result + 1;
	}
	template<class T, class Allocator>
	vector<T, Allocator>::vector(size_type count, const Allocator &alloc): vector(alloc) {
		if (count > 0) {
			_allocate(count);
			_construct_at_end(count);
		}
	}
	template<class T, class Allocator>
	vector<T, Allocator>::vector(size_type count, const_reference value, const Allocator &alloc): vector(alloc) {
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
		size_type count = ctl::distance(begin, end);
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
		other._begin.data_point = other._end.data_point = other._storage_end.data_point = nullptr;
	}
	template<class T, class Allocator>
	vector<T, Allocator>::~vector() {
		if (_begin.data_point != nullptr) {
			_deallocate_data(_begin, _storage_end);
		}
	}
	template<class T, class Allocator>
	vector<T, Allocator> &vector<T, Allocator>::operator=(const vector &other) {
		if (this != &other) {
			_destruct_at_end(_begin);
			if (capacity() < other.size()) {
				_deallocate_data(_begin, _storage_end);
				_allocate(other.size());
			}
			_construct_at_end(other.begin(), other.end());
		}

		return *this;
	}
	template<class T, class Allocator>
	vector<T, Allocator> &vector<T, Allocator>::operator=(vector &&other) noexcept {
		_begin = std::move(other._begin);
		_end = std::move(other._end);
		_storage_end = std::move(other._storage_end);
		other._begin = other._end = other._storage_end = nullptr;
		return *this;
	}
	template<class T, class Allocator>
	vector<T, Allocator>::operator std::string() const noexcept {
		using std::to_string;
		std::string output;
		for (const_reference element: *this)
			output += to_string(element) + ' ';
		return output;
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::at(size_type i) {
		if (i >= size())
			throw std::out_of_range("ctl vector");
		return *(_begin + i);
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::at(size_type i) const {
		if (i >= size())
			throw std::out_of_range("ctl vector");
		return *(_begin + i);
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::append(bidirectional_element_accessible_modifiable<value_type, iterator> &other) {
		if (size() + other.size() > capacity()) {
			size_type __capacity = _recommend(size() + other.size());
			vector tmp = vector(__capacity, this->allocator());
			tmp._end = _copy(begin(), end(), tmp.begin());
			swap(tmp);
		}

		_construct_at_end(other.begin(), other.end());
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::erase(const_iterator position) {
		if (position < _begin || position > _end)
			throw std::invalid_argument("ctl vector; position should be in vector");
		_destruct_at_end(std::move(position + 1, end(), position));
		return position + 1;
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::erase(const_iterator first,
	                                                                    const_iterator last) {
		if (first < _begin || first > _end || last < _begin || last > _end)
			throw std::invalid_argument("ctl vector; slice should be in vector");

		if (first != last)
			_destruct_at_end(std::move(last, end(), first));

		return last;
	}
	template<class T, class Allocator>
	template<class... Args>
	typename vector<T, Allocator>::reference vector<T, Allocator>::emplace_back(Args &&...args) {
		if (size() >= capacity()) {
			vector other = vector(_recommend(size() + 1), this->allocator());
			other._end = std::move(begin(), end(), other.begin());
			swap(other);
		}
		_construct_at_end(std::forward<Args>(args)...);
		return back();
	}
	template<class T, class Allocator>
	template<class... Args>
	typename vector<T, Allocator>::reference vector<T, Allocator>::emplace(const_iterator position, Args &&...args) {
		size_type __capacity = capacity();
		iterator _it;
		if (size() < __capacity) {
			_end = _move_right(position - 1, end() - 1, 1);
			_it = position;
		} else {
			vector other = vector(_recommend(size() + 1), this->allocator());
			_it = std::move(begin(), position, other.begin());
			other._end = std::move(position + 1, end(), _it + 1);
			swap(other);
		}

		this->_allocator.construct(_it.data_point, std::forward<Args>(args)...);
		return *_it;
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::fill(iterator first, iterator last, const_reference value) {
		_destruct(first, last);
		_construct(first, last, value);
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::fill(const_reference value, size_type size) {
		size_type diff = size - this->size();
		resize(size);
		if (diff > 0)
			fill(end() - diff, end(), value);
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::filter(conformer predicate) {
		vector<T, Allocator> other(this->size());
		for (iterator it = begin(); it != end(); ++it)
			if (predicate(*it))
				other.push_back(*it);

		*this = other;
	}

	template<class T, class Allocator>
	vector<T, Allocator> vector<T, Allocator>::filtered(conformer predicate) {
		vector other(this->size());
		for (iterator it = begin(); it != end(); ++it)
			if (predicate(*it))
				other.push_back(*it);

		return other;
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator position,
	                                                                     size_type count,
	                                                                     const_reference value) {
		size_type __capacity = capacity();
		iterator _it;
		if (size() + count <= __capacity) {
			_end = _move_right(position - 1, end() - 1, count);
			_it = position;
		} else {
			vector other = vector(_recommend(size() + count), this->allocator());
			_it = std::move(begin(), position, other.begin());
			other._end = std::move(position, end(), _it + count);

			swap(other);
		}

		_copy(_it, _it + count, value);
		return _it;
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator position, T &&value) {
		size_type __capacity = capacity();
		iterator _it;
		if (size() < __capacity) {
			_end = _move_right(position - 1, end() - 1, 1);
			_it = position;
		} else {
			vector other = vector(_recommend(size() + 1), this->allocator());
			_it = std::move(begin(), position, other.begin());
			other._end = std::move(position + 1, end(), _it + 1);
			swap(other);
		}

		*_it = std::move(value);
		return _it;
	}
	template<class T, class Allocator>
	template<class Iterator, typename>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator position,
	                                                                     Iterator first,
	                                                                     Iterator last) {
		difference_type count = ctl::distance(first, last);
		size_type __capacity = capacity();
		iterator _it;

		if (size() + count <= __capacity) {
			_end = _move_right(position - 1, end() - 1, count);
			_it = position;
		} else {
			vector other = vector(_recommend(size() + count), this->allocator());
			_it = std::move(begin(), position, other.begin());
			other._end = std::move(position + count, end(), _it + count);
			swap(other);
		}
		_copy(first, last, _it);
		return _it;
	}

	template<class T, class Allocator>
	template<typename U>
	vector<U, Allocator> vector<T, Allocator>::map(std::function<U(reference)> mapper) {
		vector<U, allocator_type> other(size(), this->allocator());
		iterator first = begin();
		for (typename vector<U, allocator_type>::iterator it = other.begin(); it != other.end() && first != end();
		     ++it, ++first)
			*it = mapper(*first);
		return other;
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::pop_front(size_type count) {
		_destruct(begin(), begin() + count);
		std::move(begin() + count, end(), begin());
		_end -= count;
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::push_back(const_reference value) {
		if (size() < capacity()) {
			_construct_at_end(value);
		} else {
			vector other = vector(_recommend(size() + 1), this->allocator());
			other._end = std::move(begin(), end(), other.begin());
			other._construct_at_end(value);
			swap(other);
		}
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::push_back(size_type count, const_reference value) {
		size_type __capacity = capacity();
		if (size() + count <= __capacity) {
			_construct_at_end(count, value);
		} else {
			vector other = vector(_recommend(size() + count), this->allocator());
			other._end = std::move(begin(), end(), other.begin());
			other._construct_at_end(count, value);
			swap(other);
		}
	}

	template<class T, class Allocator>
	void vector<T, Allocator>::push_back(value_type &&value) {
		iterator _it;
		size_type __capacity = capacity();
		size_type __size = size();
		if (__size + 1 > __capacity) {
			auto sz = _recommend(__size + 1);
			vector other = vector(_recommend(__size + 1), this->allocator());
			other._end = std::move(begin(), end(), other.begin());
			swap(other);

		}
		_it = _end++;
		*_it = std::move(value);
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::push_front(const_reference value) {
		if (size() < capacity()) {
			_end = _move_right(begin() - 1, end() - 1, 1);
		} else {
			vector other = vector(_recommend(size() + 1), this->allocator());
			other._end = std::move(begin(), end(), other.begin() + 1);
			swap(other);
		}
		*_begin = value;
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::push_front(value_type &&value) {
		if (size() < capacity()) {
			_end = _move_right(begin() - 1, end() - 1, 1);
		} else {
			vector other = vector(_recommend(size() + 1), this->allocator());
			other._end = std::move(begin(), end(), other.begin() + 1);
			swap(other);
		}
		*_begin = std::move(value);
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::push_front(size_type count, const_reference value) {
		size_type __capacity = capacity();
		if (size() + count <= __capacity) {
			_end = _move_right(begin() - 1, end() - 1, count);
		} else {
			vector other = vector(_recommend(size() + count), this->allocator());
			other._end = std::move(begin(), end(), other.begin() + count);
			swap(other);
		}
		_construct(begin(), begin() + count, value);
	}

	template<class T, class Allocator>
	vector<T, Allocator> vector<T, Allocator>::prefix(size_type max_length) {
		size_type _length = std::min(max_length, size());
		return vector(begin(), begin() + _length, this->allocator());
	}

	template<class T, class Allocator>
	vector<T, Allocator> vector<T, Allocator>::prefix(conformer predicate) {
		iterator last = begin();
		for (; last != end() && predicate(*last); ++last) {}
		return vector(begin(), last, this->allocator());
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::reserve(size_type n) {
		if (n > this->max_size())
			throw std::length_error("ctl vector");
		if (n > size()) {
			vector other = vector(n, this->allocator());
			other._end = std::move(begin(), end(), other.begin());
			swap(other);
		}
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::remove_all(iterator first, iterator last, const_reference item) {
		iterator result = first;
		for (; first != last; ++first)
			if (!(*first == item))
				*result++ = *first;

		_end = result;
	}

	template<class T, class Allocator>
	void vector<T, Allocator>::remove_all(iterator first, iterator last, conformer predicate) {
		iterator result = first;
		for (; first != last; ++first)
			if (!predicate(*first))
				*result++ = *first;

		_end = result;
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::remove(const_reference item) {
		iterator it = this->first(item);
		if (it != end())
			erase(it);
	}

	template<class T, class Allocator>
	void vector<T, Allocator>::resize(size_type n) {
		size_type _size = size();
		if (n != _size) {
			vector other = vector(n, this->allocator());
			other._end = std::move(begin(), begin() + n, other.begin());
			swap(other);
		}
	}

	template<class T, class Allocator>
	void vector<T, Allocator>::resize(size_type n, const value_type &value) {
		size_type _size = size();
		if (_size < n)
			_construct_at_end(n - _size, value);
		else if (_size > n)
			_destruct_at_end(_begin + n);
	}
	template<class T, class Allocator>
	vector<T, Allocator> vector<T, Allocator>::reversed() {
		vector<value_type, allocator_type> other(*this);
		other.reverse();
		return other;
	}
	template<class T, class Allocator>
	template<typename R>
	R vector<T, Allocator>::accumulate(R initial, R (*next_result)(R, const_reference)) {
		for (iterator it = begin(); it != end(); ++it)
			initial = next_result(initial, *it);

		return initial;
	}
	template<class T, class Allocator>
	vector<T, Allocator> vector<T, Allocator>::subsequence(iterator from, iterator to) {
		return vector(from, to, this->allocator());
	}
	template<class T, class Allocator>
	vector<T, Allocator> vector<T, Allocator>::suffix(conformer predicate) {
		iterator first = end();
		for (; first != begin() && predicate(*first); --first) {}
		return vector(first, end(), this->allocator());
	}

	template<class T, class Allocator>
	vector<T, Allocator> vector<T, Allocator>::suffix(size_type max_length) {
		size_type _size = std::min(max_length, size());
		return vector(end() - _size, end(), this->allocator());
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::shrink_to_fit() noexcept {
		size_type _size = size();
		if (capacity() > _size) {
			vector other = vector(_size, this->allocator());
			other._end = std::move(begin(), end(), other.begin());
			swap(other);

		}
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::swap(vector<value_type, allocator_type> &other) {
		using std::swap;
		swap(other._begin.data_point, _begin.data_point);
		swap(other._end.data_point, _end.data_point);
		swap(other._storage_end.data_point, _storage_end.data_point);
		swap(other._allocator, this->_allocator);
	}
}

#endif //CONTAINERS_VECTOR_HPP
