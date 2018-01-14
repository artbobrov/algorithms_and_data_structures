//
// Created by Artem Bobrov on 02.01.2018.
//

#ifndef COLLECTIONS_VECTOR_HPP
#define COLLECTIONS_VECTOR_HPP
#include "abstract/collection.hpp"

namespace ctl {

	template<class T>
	struct __vector_iterator : public std::iterator<std::random_access_iterator_tag, T> {
	public:
		typedef T value_type;
		typedef value_type &reference;
		typedef value_type const &const_reference;

		typedef value_type *data;
		typedef size_t size_type;
	public:
		mutable data data_point;
	public:
		explicit __vector_iterator(data point = nullptr) : data_point(point) {}

		const __vector_iterator &operator=(const __vector_iterator &__x) const {
			this->data_point = __x.data_point;
			return *this;
		}
		__vector_iterator &operator=(const __vector_iterator &__x) {
			this->data_point = __x.data_point;
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
		inline __vector_iterator &operator--() {
			--data_point;
			return *this;
		}
		inline __vector_iterator operator--(int) {
			__vector_iterator value = *this;
			--*this;
			return value;
		}

		inline __vector_iterator &operator=(T *point) {
			data_point = point;
			return *this;
		}

		inline reference operator*() { return *data_point; }
		inline const_reference operator*() const { return *data_point; }
		inline T *operator->() { return &(operator*()); }

		friend inline bool operator==(const __vector_iterator &__x, const __vector_iterator &__y) {
			return __x.data_point == __y.data_point;
		}
		friend inline bool operator!=(const __vector_iterator &__x, const __vector_iterator &__y) {
			return !(__x == __y);
		}
		friend inline size_type operator-(__vector_iterator __x, __vector_iterator __y) {
			return static_cast<size_type>(__x.data_point - __y.data_point);
		}

		inline __vector_iterator &plus(size_type idx) {
			data_point += idx;
			return *this;
		}
		inline __vector_iterator &minus(size_type idx) {
			data_point -= idx;
			return *this;
		}

		friend inline __vector_iterator operator+(const __vector_iterator &__x, size_type idx) {
			__vector_iterator value = __x;
			value.plus(idx);
			return value;
		}
		friend inline __vector_iterator operator-(const __vector_iterator &__x, size_type idx) {
			__vector_iterator value = __x;
			value.minus(idx);
			return value;
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

	template<class T, class Allocator = std::allocator<T> >
	class vector : public collection<T, __vector_iterator<T>, Allocator> {
	public:
		typedef Allocator allocator_type;
		typedef T value_type;
		typedef __vector_iterator<T> iterator;
		typedef iterator const_iterator;
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
	public:
		inline vector() noexcept : _begin(nullptr), _end(nullptr), _storage_end(nullptr) {}
		inline explicit vector(const Allocator &alloc);
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
		vector(const Container &__container): vector(__container.begin(), __container.end()) {}

		inline explicit vector(const vector<value_type, allocator_type> &other, const Allocator &alloc);

		inline vector(std::initializer_list<value_type> il, const Allocator &alloc = Allocator());

		inline virtual ~vector();
	public:
		inline reference at(size_type i) override;
		inline const_reference at(size_type i) const override;
		inline void append(const collection<value_type, iterator, allocator_type> &value) override;

		inline reference back() override;
		inline const_reference back() const override;
		inline iterator begin() noexcept override;
		inline const_iterator begin() const noexcept override;

		inline const_iterator cbegin() const noexcept override;
		inline const_iterator cend() const noexcept override;
		inline void clear() noexcept override;
		inline size_type capacity() const noexcept override;

		inline pointer data() noexcept override;
		inline const pointer data() const noexcept override;

		inline iterator end() noexcept override;
		inline const_iterator end() const noexcept override;
		inline iterator erase(const_iterator position) override;
		inline iterator erase(const_iterator first, const_iterator last) override;
		inline bool empty() const noexcept override;

		inline reference front() override;
		inline const_reference front() const override;
		collection<value_type, iterator, Allocator> &filter(conformer conform) override;
		inline vector<value_type, allocator_type> &filled(const T &value);
		inline vector<value_type, allocator_type> &filled(const T &value, size_type size);

		iterator insert(const_iterator position, const T &value) override;
		inline iterator insert(size_type idx, const T &value) override;
		inline iterator insert(const_iterator position, value_type &&value) override;
		inline iterator insert(const_iterator position, size_type count, const T &value) override;
		inline iterator insert(const_iterator position, std::initializer_list<value_type> il) override;
		inline iterator insert(size_type idx, size_type count, const T &value) override;
		template<class Iterator>
		iterator insert(const_iterator position,
		                Iterator first,
		                Iterator last,
		                typename std::enable_if<is_input_iterator<Iterator>::value>::type * = 0); // OK

		inline void pop_back() override;
		inline void pop_front() override;
		inline void push_back(const_reference value) override;
		inline void push_back(value_type &&value) override;
		inline void push_front(const_reference value) override;
		inline void push_front(value_type &&value) override;

		inline void resize(size_type count) override;
		inline void resize(size_type count, const T &value) override;
		inline void reserve(size_type n) override;
		inline vector<value_type, allocator_type> &reversed();
		inline vector<value_type, allocator_type> &reversed(iterator first, iterator last);

		inline void swap(vector<value_type, allocator_type> &other) noexcept;
		inline explicit operator std::string() const noexcept override;
		inline size_type size() const noexcept override;
		inline void shrink_to_fit() noexcept override;

		inline collection<value_type, iterator, allocator_type> &subsequence(const_iterator from,
		                                                                     const_iterator to) override;
		inline collection<value_type, iterator, allocator_type> &subsequence(size_type from, size_type to) override;
	public:
		vector &operator=(const vector &other);
		vector &operator=(vector &&other);
	protected:
		iterator _begin;
		iterator _end;
		iterator _storage_end;
		inline void _delete_data(iterator from, iterator to);
		inline iterator _copy_data(iterator begin, iterator end, iterator to); // returns end iterator to copied data
		inline size_type _recommend(size_type new_size);
	};

	template<class T, class Allocator>
	vector<T, Allocator>::vector(const Allocator &alloc): collection<value_type, iterator, allocator_type>(alloc) {
	}
	template<class T, class Allocator>
	vector<T, Allocator>::vector(size_type count, const Allocator &alloc):
		collection<value_type, iterator, allocator_type>(alloc) {
		_end = _begin = this->_allocator.allocate(count);
		_storage_end = _begin + count;
	}

	template<class T, class Allocator>
	vector<T, Allocator>::vector(size_type count, const T &value, const Allocator &alloc): vector(count, alloc) {
		_end = _storage_end;
		this->fill(value);
	}

	template<class T, class Allocator>
	template<class Iterator>
	vector<T, Allocator>::vector(Iterator begin,
	                             Iterator end,
	                             const Allocator &alloc,
	                             typename std::enable_if<is_random_access_iterator<Iterator>::value>::type *):
		vector(end - begin, alloc) {
		Iterator start = begin;
		for (size_type _distance = 0; begin != end; ++begin, ++_distance) {
			this->_allocator.construct(_begin.data_point + _distance, *begin);
		}
		_end = _storage_end;
	}
	template<class T, class Allocator>
	template<class Iterator>
	vector<T, Allocator>::vector(Iterator begin,
	                             Iterator end,
	                             const Allocator &alloc,
	                             typename std::enable_if<is_input_iterator<Iterator>::value
		                                                     && !is_random_access_iterator<Iterator>::value>::type *):
		vector(alloc) {
		for (; begin != end; ++begin) {
			this->push_back(*begin);
		}
	}

	template<class T, class Allocator>
	vector<T, Allocator>::vector(const vector<value_type, allocator_type> &other):
		collection<value_type, iterator, allocator_type>(other._allocator) {
		*this = other;
	}

	template<class T, class Allocator>
	vector<T, Allocator>::vector(const vector<value_type, allocator_type> &other, const Allocator &alloc)
		:collection<value_type, iterator, allocator_type>(alloc), vector(other) {
	}
	template<class T, class Allocator>
	vector<T, Allocator>::vector(vector &&other) noexcept: vector(other._allocator) {
		*this = std::move(other);
	}

	template<class T, class Allocator>
	vector<T, Allocator>::~vector() {
		_delete_data(_begin, _storage_end);
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::_delete_data(iterator from, iterator to) {

		iterator _start = from;
		size_type _sz = this->distance(to, from);
		for (; from != to; ++from)
			this->_allocator.destroy(from.data_point);
		if (capacity() && _begin.data_point != nullptr)
			this->_allocator.deallocate(_start.data_point, _sz);
	}
	template<class T, class Allocator>
	vector<T, Allocator>::vector(std::initializer_list<value_type> il, const Allocator &alloc): vector(il.size(),
	                                                                                                   alloc) {
		for (typename std::initializer_list<value_type>::iterator it = il.begin(); it != il.end(); ++it)
			this->_allocator.construct(_begin + this->distance(it, il.begin()), *it);

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
	typename vector<T, Allocator>::reference vector<T, Allocator>::at(size_type i) {
		if (i < capacity() && i > size())
			_end = _begin + i;
		return *(_begin + i);
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::at(size_type i) const {
		if (i < capacity() && i > size())
			_end = _begin + i;
		return *(_begin + i);
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::append(const collection<value_type, iterator, allocator_type> &value) {
		if (size() + value.size() > capacity()) {
			size_type __capacity = _recommend(size() + value.size());
			reserve(__capacity);
		}

		for (const_reference element: value)
			this->_allocator.construct((_end++).data_point, element);
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::back() {
		return *(_end - 1);
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::back() const {
		return *(_end - 1);
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::begin() noexcept {
		return _begin;
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::const_iterator vector<T, Allocator>::begin() const noexcept {
		return _begin;
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cbegin() const noexcept {
		return _begin;
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cend() const noexcept {
		return _end;
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::clear() noexcept {
		_delete_data(_begin, _storage_end);
		_begin = _end = _storage_end = nullptr;
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::size_type vector<T, Allocator>::capacity() const noexcept {
		return static_cast<size_type>(_storage_end - _begin);
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::pointer vector<T, Allocator>::data() noexcept {
		return _begin.data_point;
	}
	template<class T, class Allocator>
	const typename vector<T, Allocator>::pointer vector<T, Allocator>::data() const noexcept {
		return _begin.data_point;
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::end() noexcept {
		return _end;
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::const_iterator vector<T, Allocator>::end() const noexcept {
		return _end;
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::erase(const_iterator position) {
		if (position < _begin || position > end())
			throw std::invalid_argument("invalid position; position should be in vector; erase(const_iterator)");
		size_type __capacity = capacity();
		iterator _start(this->_allocator.allocate(__capacity));
		iterator middle = _copy_data(_begin, position, _start);
		_end = _copy_data(_begin + this->distance(position, _begin) + 1, _end, middle);
		_delete_data(_begin, _storage_end);
		_begin = _start;
		_storage_end = _begin + __capacity;
		return middle;
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::erase(const_iterator first,
	                                                                    const_iterator last) {
		if (first < _begin || first > _end || last < _begin || last > end())
			throw std::invalid_argument("invalid slice; slice should be in vector; erase(const_iterator,const_iterator)");
		size_type __capacity = capacity();
		size_type __count = this->distance(last, first);
		iterator _start(this->_allocator.allocate(__capacity));
		iterator middle = _copy_data(_begin, first, _start);
		_end = _copy_data(_begin + this->distance(first, _begin) + __count, _end, middle);
		_delete_data(_begin, _storage_end);
		_begin = _start;
		_storage_end = _begin + __capacity;

		return middle;
	}
	template<class T, class Allocator>
	bool vector<T, Allocator>::empty() const noexcept {
		return _begin == _end;
	}

	template<class T, class Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::front() {
		return *_begin;
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::front() const {
		return *_begin;
	}
	template<class T, class Allocator>
	collection<typename vector<T, Allocator>::value_type,
	           typename vector<T, Allocator>::iterator,
	           typename vector<T, Allocator>::allocator_type> &
	vector<T, Allocator>::filter(conformer conform) {
		auto *other = new vector<value_type, allocator_type>(size(), this->allocator());
		for (const_reference element: *this)
			if (conform(element))
				other->push_back(element);
		return *other;
	}

	template<class T, class Allocator>
	vector<typename vector<T, Allocator>::value_type, typename vector<T, Allocator>::allocator_type> &
	vector<T, Allocator>::filled(
		const T &value) {
		auto *other = new vector<value_type, allocator_type>(*this);
		other->fill(value);
		return *other;
	}
	template<class T, class Allocator>
	vector<typename vector<T, Allocator>::value_type, typename vector<T, Allocator>::allocator_type> &
	vector<T, Allocator>::filled(const T &value, size_type size) {
		auto *other = new vector<value_type, allocator_type>(*this);
		other->fill(value, size);
		return *other;
	}

	template<class T, class allocator>
	typename vector<T, allocator>::iterator vector<T, allocator>::insert(size_type idx, const T &value) {
		return insert(_begin + idx, value);
	}

	template<class T, class allocator>
	typename vector<T, allocator>::iterator vector<T, allocator>::insert(size_type idx,
	                                                                     size_type count,
	                                                                     const T &value) {
		return insert(_begin + idx, count, value);
	}

	template<class T, class allocator>
	typename vector<T, allocator>::iterator vector<T, allocator>::insert(const_iterator position,
	                                                                     size_type count,
	                                                                     const T &value) {
		iterator middle = position;
		if (size() + count > capacity() || position - _begin + count > capacity()) {
			size_type __capacity = _recommend(std::max(size() + count, position - _begin + count));
			iterator _start(this->_allocator.allocate(__capacity));
			middle = _copy_data(_begin, std::min(_end, position), _start);
			if (_end < position) {
				middle = middle + this->distance(position, _end);
				_end = middle + count;

			} else {
				_end = _copy_data(position, _end, middle + count);
			}
			for (size_t idx = 0; idx < count; idx++)
				this->_allocator.construct(middle.data_point + idx, value);
			_delete_data(_begin, _storage_end);
			_begin = _start;
			_storage_end = _begin + __capacity;
		} else {
			using std::swap;
			if (_end > position) {
				for (middle = _end - 1; middle != position; --middle) {
					swap(*middle, *(middle + count));
				}
				_end = _end + count;
			} else
				_end = position + count;

			int i = 0;
			for (iterator first = middle; first - middle != count; ++first)
				this->_allocator.construct(middle.data_point, value);

		}
		return middle;
	}

	template<class T, class allocator>
	typename vector<T, allocator>::iterator vector<T, allocator>::insert(const_iterator position, const T &value) {
		return insert(position, 1, value);
	}

	template<class T, class allocator>
	typename vector<T, allocator>::iterator vector<T, allocator>::insert(const_iterator position, value_type &&value) {
		size_type __capacity = _recommend(std::max(size() + 1, this->distance(position, _begin) + 1));
		iterator _start(this->_allocator.allocate(__capacity));
		iterator middle = _copy_data(_begin, std::min(_end, position), _start);
		if (_end < position) {
			middle = middle + this->distance(position, _end);
			_end = middle + 1;
		} else
			_end = _copy_data(position, _end, middle + 1);

		this->_allocator.construct(middle.data_point, std::move(value));
		_delete_data(_begin, _storage_end);
		_begin = _start;
		_storage_end = _begin + __capacity;
		return middle;
	}

	template<class T, class allocator>
	typename vector<T, allocator>::iterator vector<T, allocator>::insert(const_iterator position,
	                                                                     std::initializer_list<value_type> il) {
		return insert(position, il.begin(), il.end());
	}
	template<class T, class Allocator>
	template<class Iterator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator position,
	                                                                     Iterator first,
	                                                                     Iterator last,
	                                                                     typename std::enable_if<is_input_iterator<
		                                                                     Iterator>::value>::type *) {
		iterator middle;
		size_type count = last - first;
		if (size() + count > capacity() || position - _begin + count > capacity()) {
			size_type __capacity = _recommend(std::max(size() + count, position - _begin + count));
			iterator _start(this->_allocator.allocate(__capacity));
			middle = _copy_data(_begin, std::min(_end, position), _start);
			if (_end < position) {
				middle = middle + this->distance(position, _end);
				_end = middle + count;

			} else
				_end = _copy_data(position, _end, middle + count);

			for (size_t idx = 0; idx < count; idx++)
				this->_allocator.construct(middle.data_point + idx, *(first + idx));

			_delete_data(_begin, _storage_end);
			_begin = _start;
			_storage_end = _begin + __capacity;
		} else {
			using std::swap;
			for (middle = _end - 1; middle != position; --middle) {
				swap(*middle, *(middle + count));
			}
			_end = _end + count;

			for (size_t idx = 0; idx < count; idx++)
				this->_allocator.construct(middle.data_point + idx, *(first + idx));

		}
		return middle;
	}

	template<class T, class allocator>
	void vector<T, allocator>::pop_back() {
		--_end;
		this->_allocator.destroy(_end.data_point);
	}

	template<class T, class allocator>
	void vector<T, allocator>::pop_front() {
		size_type __capacity = capacity();
		iterator _start(this->_allocator.allocate(__capacity, _begin.data_point));
		_end = _copy_data(_begin + 1, _end, _start);
		_begin = _start;
		_storage_end = _begin + __capacity;
	}
	template<class T, class allocator>
	void vector<T, allocator>::push_back(const_reference value) {
		if (size() + 1 > capacity()) {
			size_type __capacity = _recommend(size() + 1);
			reserve(__capacity);
		}
		this->_allocator.construct(_end++.data_point, value);

	}
	template<class T, class allocator>
	void vector<T, allocator>::push_back(value_type &&value) {
		if (size() + 1 > capacity()) {
			size_type __capacity = _recommend(size() + 1);
			reserve(__capacity);
		}
		this->_allocator.construct(_end++.data_point, std::move(value));
	}

	template<class T, class allocator>
	void vector<T, allocator>::push_front(const_reference value) {
		size_type __capacity = (size() == capacity()) ? _recommend(size() + 1) : size();

		iterator _start(this->_allocator.allocate(__capacity, _begin.data_point));
		this->_allocator.construct(_start.data_point, value);

		_end = _copy_data(_begin, _end, _start + 1);
		_delete_data(_begin, _storage_end);

		_begin = _start;
		_storage_end = _begin + __capacity;
	}

	template<class T, class allocator>
	void vector<T, allocator>::push_front(value_type &&value) {
		size_type __capacity = (size() == capacity()) ? _recommend(size() + 1) : size();

		iterator _start(this->_allocator.allocate(__capacity, _begin.data_point));
		this->_allocator.construct(_start.data_point, std::move(value));

		_end = _copy_data(_begin, _end, _start + 1);
		_delete_data(_begin, _storage_end);

		_begin = _start;
		_storage_end = _begin + __capacity;
	}
	template<class T, class allocator>
	void vector<T, allocator>::resize(size_type sz) {
		if (!sz)
			clear();

		reserve(sz);
	}

	template<class T, class allocator>
	void vector<T, allocator>::resize(size_type sz, const T &value) {
		size_type __capacity = capacity();
		resize(sz);
		if (sz > __capacity) {
			_end = _storage_end;
			this->fill(_begin + __capacity, _storage_end, value);
		}
	}
	template<class T, class allocator>
	void vector<T, allocator>::reserve(size_type n) {
		if (!n)
			clear();
		iterator _start(this->_allocator.allocate(n, _begin.data_point));
		_end = _copy_data(_begin, std::min(_begin + n, _end), _start);
		_delete_data(_begin, _storage_end);
		_begin = _start;
		_storage_end = _begin + n;
	}
	template<class T, class Allocator>
	vector<typename vector<T, Allocator>::value_type, typename vector<T, Allocator>::allocator_type> &
	vector<T, Allocator>::reversed() {
		auto *other = new vector<value_type, allocator_type>(*this);
		other->reverse();
		return *other;
	}
	template<class T, class Allocator>
	vector<typename vector<T, Allocator>::value_type, typename vector<T, Allocator>::allocator_type> &
	vector<T, Allocator>::reversed(iterator first, iterator last) {
		auto *other = new vector<value_type, allocator_type>(*this);
		other->reverse(other->_begin + this->distance(first, _begin),
		               other->_begin + this->distance(last, _begin));
		return *other;
	}
	template<class T, class allocator>
	void vector<T, allocator>::shrink_to_fit() noexcept {
		if (size() < capacity())
			resize(size());
	}
	template<class T, class allocator>
	void vector<T, allocator>::swap(vector<value_type, allocator_type> &other) noexcept {
		using std::swap;
		swap(this->_begin, other._begin);
		swap(this->_end, other._end);
		swap(this->_storage_end, other._storage_end);
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::size_type vector<T, Allocator>::size() const noexcept {
		return static_cast<size_type>(_end - _begin);
	}
	template<class T, class allocator>
	vector<T, allocator>::operator std::string() const noexcept {
		using std::to_string;
		std::string output = '[' + to_string(this->size()) + ',' + to_string(this->capacity()) + "] ";
		for (reference element: *this)
			output += to_string(element) + " ";
		return output;
	}
	template<class T, class Allocator>
	collection<typename vector<T, Allocator>::value_type,
	           typename vector<T, Allocator>::iterator,
	           typename vector<T, Allocator>::allocator_type> &
	vector<T, Allocator>::subsequence(const_iterator from, const_iterator to) {
		auto *other = new vector<value_type, allocator_type>(from, to);
		return *other;
	}
	template<class T, class Allocator>
	collection<typename vector<T, Allocator>::value_type,
	           typename vector<T, Allocator>::iterator,
	           typename vector<T, Allocator>::allocator_type> &
	vector<T, Allocator>::subsequence(size_type from, size_type to) {
		return subsequence(_begin + from, _begin + to);
	}

	template<class T, class Allocator>
	vector<T, Allocator> &vector<T, Allocator>::operator=(const vector &other) {
		this->_allocator = other._allocator;
		_begin = this->_allocator.allocate(other.capacity());
		_end = _copy_data(other.begin(), other.end(), _begin);
		_storage_end = _begin + other.capacity();
		return *this;
	}
	template<class T, class Allocator>
	vector<T, Allocator> &vector<T, Allocator>::operator=(vector &&other) {
		this->_allocator = std::move(other._allocator);
		_begin = other._begin;
		_end = other._end;
		_storage_end = other._storage_end;
		other._end = other._begin = other._storage_end = nullptr;
		return *this;
	}

}

#endif //COLLECTIONS_VECTOR_HPP

