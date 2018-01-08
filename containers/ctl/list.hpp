//
// Created by Artem Bobrov on 05.01.2018.
//

#ifndef COLLECTIONS_LIST_HPP
#define COLLECTIONS_LIST_HPP

#include "../abstract/collection.hpp"
#include <iterator>
#include <type_traits>
namespace ctl {
	template<class T>
	struct __node {
	public:
		typedef T value_type;
	public:
		value_type data;
		__node *prev;
		__node *next;
	public:
		bool operator==(const __node &other) {
			return other.data == data && other.next == next && other.prev == prev;
		}
	};

	template<class T>
	struct __list_iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
	public:
		typedef T value_type;
		typedef value_type &reference;
		typedef value_type const &const_reference;

		typedef __node<T> node;
		typedef node *node_point;
		typedef size_t size_type;
	public:
		node_point data_point;
	public:
		explicit __list_iterator(node_point point = nullptr) : data_point(point) {}

		inline __list_iterator &operator++() {
			data_point = data_point->next;
			return *this;
		}
		inline __list_iterator operator++(int) {
			__list_iterator value = *this;
			++*this;
			return value;
		}
		inline __list_iterator &operator--() {
			data_point = data_point->prev;
			return *this;
		}
		inline __list_iterator operator--(int) {
			__list_iterator value = *this;
			--*this;
			return value;
		}
		inline reference operator*() { return data_point->data; }
		inline const_reference operator*() const { return data_point->data; }
		inline T *operator->() { return &(operator*()); }

		friend inline bool operator==(const __list_iterator &__x, const __list_iterator &__y) {
			return __x.data_point == __y.data_point;
		}
		friend inline bool operator!=(const __list_iterator &__x, const __list_iterator &__y) {
			return !(__x == __y);
		}
		friend inline size_type operator-(__list_iterator __x, __list_iterator __y) {
			size_type distance = 0;
			for (; __x != __y; ++__y, ++distance) {}
			return distance;
		}

		inline __list_iterator &plus(size_type idx) {
			for (; idx > 0; --idx)
				++*this;
			return *this;
		}
		inline __list_iterator &minus(size_type idx) {
			for (; idx > 0; --idx)
				--*this;
			return *this;
		}

		inline __list_iterator operator+(size_type idx) {
			__list_iterator value = *this;
			value.plus(idx);
			return value;
		}
		inline __list_iterator operator-(size_type idx) {
			__list_iterator value = *this;
			value.minus(idx);
			return value;
		}
	};

	template<class T, class Allocator = std::allocator<__node<T>>>
	class list : public collection<T, __list_iterator<T>, Allocator> {
	public:
		typedef Allocator allocator_type;
		typedef T value_type;
		typedef __list_iterator<T> iterator;
		typedef const iterator const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef const std::reverse_iterator<iterator> const_reverse_iterator;
		typedef T *pointer;
		typedef value_type &reference;
		typedef value_type const &const_reference;
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef std::function<bool(const_reference)> conformer;

		typedef __node<T> node;
		typedef node *node_point;
	private:
		iterator _head;
		iterator _tail;
		size_type _size = 0;
	public:
		explicit list(const Allocator &alloc = Allocator());

		explicit list(size_type count, const T &value, const Allocator &alloc = Allocator());
		explicit list(size_type count, const Allocator &alloc = Allocator());
		inline explicit list(iterator begin, iterator end, const Allocator &alloc = Allocator());
		list(const list &other);
		list(const list &other, const Allocator &alloc) : collection<value_type, iterator, allocator_type>(alloc),
		                                                  list(other) {}
		list(list &&other) noexcept: list(other, other.allocator()) {}
		list(list &&other, const Allocator &alloc) noexcept;
		list(std::initializer_list<T> il, const Allocator &alloc = Allocator());

		template<class Iterator>
		list(Iterator first, Iterator last, const Allocator &alloc = Allocator());
		inline virtual ~list();
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
		collection<value_type, iterator, allocator_type> &filter(conformer conform) override;
		inline list<value_type, allocator_type> &filled(const T &value); // +-
		inline list<value_type, allocator_type> &filled(const T &value, size_type size);

		inline iterator insert(const_iterator position, const T &value) override;
		inline iterator insert(size_type idx, const T &value) override;
		inline iterator insert(const_iterator position, value_type &&value) override;
		inline iterator insert(const_iterator position, size_type count, const T &value) override;
		iterator insert(const_iterator position, std::initializer_list<value_type> il) override;
		inline iterator insert(size_type idx, size_type count, const T &value) override;
		template<class Iterator>
		iterator insert(const_iterator position,
		                Iterator first,
		                Iterator last,
		                typename std::enable_if<std::__is_input_iterator<Iterator>::value>::type * = 0);

		inline void pop_back() override;
		inline void pop_front() override;
		inline void push_back(const_reference value) override;
		inline void push_back(value_type &&value) override;
		inline void push_front(const_reference value) override;
		inline void push_front(value_type &&value) override;

		inline void resize(size_type count) override;
		inline void resize(size_type count, const T &value) override;
		inline void reserve(size_type n) override;
		inline list<value_type, allocator_type> &reversed();
		inline list<value_type, allocator_type> &reversed(iterator first, iterator last);

		inline void swap(list<value_type, allocator_type> &other) noexcept;
		inline explicit operator std::string() const noexcept override;
		inline size_type size() const noexcept override;
		inline void shrink_to_fit() noexcept override {}

		inline collection<value_type, iterator, allocator_type> &subsequence(iterator from,
		                                                                     iterator to) override;
		inline collection<value_type, iterator, allocator_type> &subsequence(size_type from, size_type to) override;
	private:
		inline void _delete_data(iterator from, iterator to);
		inline void _delete_node(node_point point);
	};
	template<class T, class Allocator>
	void list<T, Allocator>::_delete_data(iterator from, iterator to) {
		for (; from != to; ++from) {
			_delete_node(from.data_point);
		}
		_delete_node(to.data_point);
	}
	template<class T, class Allocator>
	void list<T, Allocator>::_delete_node(node_point point) {
		this->_allocator.destroy(point);
		this->_allocator.deallocate(point, 1);
	}
	template<class T, class Allocator>
	list<T, Allocator>::list(const Allocator &alloc): collection<value_type, iterator, allocator_type>(alloc),
	                                                  _head(this->_allocator.allocate(1)),
	                                                  _tail(_head),
	                                                  _size(0) {
		_head.data_point->next = _tail.data_point;
		_tail.data_point->prev = _head.data_point;
	}
	template<class T, class Allocator>
	list<T, Allocator>::list(size_type count, const Allocator &alloc): list(alloc) {
		this->reserve(count);
	}

	template<class T, class Allocator>
	list<T, Allocator>::list(size_type count, const T &value, const Allocator &alloc): list(count, alloc) {
		this->fill(value);
	}
	template<class T, class Allocator>
	list<T, Allocator>::list(iterator begin, iterator end, const Allocator &alloc): list(alloc) {
		for (; begin != end; ++begin)
			this->push_back(*begin);
	}

	template<class T, class Allocator>
	list<T, Allocator>::list(const list<value_type, allocator_type> &other):
		list(other.allocator()) {
		for (iterator iter = other.begin(); iter != other.end(); ++iter)
			this->push_back(*iter);
	}
	template<class T, class Allocator>
	list<T, Allocator>::list(list &&other, const Allocator &alloc) noexcept:
		list(alloc) {
		_head.data_point = other._head.data_point;
		_tail.data_point = other._tail.data_point;
		_size = other._size;
		_delete_node(other._head.data_point);
		_delete_node(other._tail.data_point);
		other._size = 0;
	}

	template<class T, class Allocator>
	list<T, Allocator>::~list() {
		_delete_data(begin(), end());
	}
	template<class T, class Allocator>
	list<T, Allocator>::list(std::initializer_list<value_type> il, const Allocator &alloc): list(
		alloc) {
		for (typename std::initializer_list<value_type>::iterator it = il.begin(); it != il.end(); ++it)
			this->push_back(*it);
	}
	template<class T, class Allocator>
	template<class Iterator>
	list<T, Allocator>::list(Iterator first, Iterator last, const Allocator &alloc): list(alloc) {
		for (; first != last; ++first)
			this->push_back(*first);
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::reference list<T, Allocator>::at(size_type i) {
		iterator value = begin();
		for (; i > 0; --i)
			++value;
		return *value;
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::const_reference list<T, Allocator>::at(size_type i) const {
		iterator value = begin();
		for (; i > 0; --i)
			++value;
		return *value;
	}
	template<class T, class Allocator>
	void list<T, Allocator>::append(const collection<value_type, iterator, allocator_type> &value) {
		for (iterator _it = value.begin(); _it != value.end(); ++_it)
			this->push_back(*_it);
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::reference list<T, Allocator>::back() {
		return _tail.data_point->prev->data;
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::const_reference list<T, Allocator>::back() const {
		return _tail.data_point->prev->data;
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::iterator list<T, Allocator>::begin() noexcept {
		return _head;
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::const_iterator list<T, Allocator>::begin() const noexcept {
		return _head;
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::const_iterator list<T, Allocator>::cbegin() const noexcept {
		return _head;
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::const_iterator list<T, Allocator>::cend() const noexcept {
		return _tail;
	}
	template<class T, class Allocator>
	void list<T, Allocator>::clear() noexcept {
		while (!empty())
			pop_back();
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::size_type list<T, Allocator>::capacity() const noexcept {
		return _size;
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::pointer list<T, Allocator>::data() noexcept {
		pointer _data = new T[_size];
		iterator it = _head;
		for (int i = 0; i < _size; ++i, ++it)
			_data[i] = *it;
		return _data;
	}
	template<class T, class Allocator>
	const typename list<T, Allocator>::pointer list<T, Allocator>::data() const noexcept {
		pointer _data = new T[_size];
		iterator it = _head;
		for (int i = 0; i < _size; ++i, ++it)
			_data[i] = *it;
		return _data;
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::iterator list<T, Allocator>::end() noexcept {
		return _tail;
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::const_iterator list<T, Allocator>::end() const noexcept {
		return _tail;
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::iterator list<T, Allocator>::erase(const_iterator position) {
		if (position == _head) {
			pop_front();
			return _head;
		}
		node_point deleted_node = position.data_point;

		node_point result = deleted_node->prev->next = deleted_node->next;
		deleted_node->next->prev = deleted_node->prev;

		_delete_node(deleted_node);
		--_size;
		return iterator(result);
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::iterator list<T, Allocator>::erase(const_iterator first,
	                                                                const_iterator last) {
		iterator result = first;
		for (iterator it = first; it != last; ++it) {
			result = erase(it);
		}
		return result;
	}
	template<class T, class Allocator>
	bool list<T, Allocator>::empty() const noexcept {
		return !_size;
	}

	template<class T, class Allocator>
	typename list<T, Allocator>::reference list<T, Allocator>::front() {
		return *_head;
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::const_reference list<T, Allocator>::front() const {
		return *_head;
	}
	template<class T, class Allocator>
	collection<typename list<T, Allocator>::value_type,
	           typename list<T, Allocator>::iterator,
	           typename list<T, Allocator>::allocator_type> &
	list<T, Allocator>::filter(conformer conform) {
		auto *other = new list<value_type, allocator_type>(this->allocator());
		for (const_reference element: *this)
			if (conform(element))
				other->push_back(element);
		return *other;
	}

	template<class T, class Allocator>
	list<typename list<T, Allocator>::value_type, typename list<T, Allocator>::allocator_type> &
	list<T, Allocator>::filled(const T &value) {
		auto *other = new list<value_type, allocator_type>(*this);
		other->fill(value);
		return *other;

	}
	template<class T, class Allocator>
	list<typename list<T, Allocator>::value_type, typename list<T, Allocator>::allocator_type> &
	list<T, Allocator>::filled(const T &value, size_type size) {
		auto *other = new list<value_type, allocator_type>(*this);
		other->fill(value, size);
		return *other;
	}
	template<class T, class allocator>
	typename list<T, allocator>::iterator list<T, allocator>::insert(size_type idx, const T &value) {
		return insert(begin() + idx, value);
	}

	template<class T, class allocator>
	typename list<T, allocator>::iterator list<T, allocator>::insert(size_type idx,
	                                                                 size_type count,
	                                                                 const T &value) {
		iterator result = begin() + idx;
		for (; count > 0; --count) {
			result = insert(result, value);
		}
		return result;
	}

	template<class T, class allocator>
	typename list<T, allocator>::iterator list<T, allocator>::insert(const_iterator position,
	                                                                 size_type count,
	                                                                 const T &value) {
		iterator result = position;
		for (; count > 0; --count) {
			result = insert(result, value);
		}
		return result;
	}

	template<class T, class allocator>
	typename list<T, allocator>::iterator list<T, allocator>::insert(const_iterator position, const T &value) {
		node_point new_node = this->_allocator.allocate(1);
		new_node->data = value;
		position.data_point->prev->next = new_node;
		new_node->prev = position.data_point->prev;
		position.data_point->prev = new_node;
		new_node->next = position.data_point;
		++_size;
		return iterator(new_node);
	}

	template<class T, class allocator>
	typename list<T, allocator>::iterator list<T, allocator>::insert(const_iterator position, value_type &&value) {
		node_point new_node = this->_allocator.allocate(1);
		new_node->data = std::move(value);
		position.data_point->prev->next = new_node;
		new_node->prev = position.data_point->prev;
		position.data_point->prev = new_node;
		new_node->next = position.data_point;
		++_size;
		return iterator(new_node);
	}

	template<class T, class allocator>
	typename list<T, allocator>::iterator list<T, allocator>::insert(const_iterator position,
	                                                                 std::initializer_list<value_type> il) {
		return this->insert(position, il.begin(), il.end());
	}

	template<class T, class Allocator>
	template<class Iterator>
	typename list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator position,
	                                                                 Iterator first,
	                                                                 Iterator last,
	                                                                 typename std::enable_if<std::__is_input_iterator<
		                                                                 Iterator>::value>::type *) {
		iterator result = position;
		iterator pos = position;
		for (Iterator it = first; it != last; ++it, ++pos) {
			pos = insert(pos, *it);
			if (it == first)
				result = pos;
		}
		return result;
	}
	template<class T, class allocator>
	void list<T, allocator>::pop_back() {
		node_point deleted_node = _tail.data_point->prev;
		_tail.data_point->prev = deleted_node->prev;
		_tail.data_point->prev->next = _tail.data_point;
		_delete_node(deleted_node);
		--_size;
	}

	template<class T, class allocator>
	void list<T, allocator>::pop_front() {
		_head.data_point = _head.data_point->next;
		_delete_node(_head.data_point->prev);
		--_size;
	}
	template<class T, class allocator>
	void list<T, allocator>::push_back(const_reference value) {
		node_point new_tail = this->_allocator.allocate(1);
		*_tail = value;
		_tail.data_point->next = new_tail;
		new_tail->prev = _tail.data_point;
		_tail.data_point = new_tail;
		++_size;
	}
	template<class T, class allocator>
	void list<T, allocator>::push_back(value_type &&value) {
		node_point new_tail = this->_allocator.allocate(1);
		*_tail = std::move(value);
		_tail.data_point->next = new_tail;
		new_tail->prev = _tail.data_point;
		_tail.data_point = new_tail;
		++_size;
	}

	template<class T, class allocator>
	void list<T, allocator>::push_front(const_reference value) {
		node_point new_node = this->_allocator.allocate(1);
		new_node->data = value;
		_head.data_point->prev = new_node;
		new_node->next = _head.data_point;
		_head.data_point = new_node;
		++_size;
	}

	template<class T, class allocator>
	void list<T, allocator>::push_front(value_type &&value) {
		node_point new_node = this->_allocator.allocate(1);
		new_node->data = std::move(value);
		_head.data_point->prev = new_node;
		new_node->next = _head.data_point;
		_head.data_point = new_node;
		++_size;
	}
	template<class T, class allocator>
	void list<T, allocator>::resize(size_type sz) {
		reserve(sz);
	}

	template<class T, class allocator>
	void list<T, allocator>::resize(size_type sz, const T &value) {
		long long difference = sz - size();
		if (difference > 0) {
			for (; difference > 0; --difference)
				push_back(value);
		} else if (difference < 0) {
			for (; difference < 0; ++difference)
				pop_back();
		}
	}
	template<class T, class allocator>
	void list<T, allocator>::reserve(size_type n) {
		long long difference = n - size();
		if (difference > 0) {
			for (; difference > 0; --difference)
				push_back(T());
		} else if (difference < 0) {
			for (; difference < 0; ++difference)
				pop_back();
		}
	}
	template<class T, class Allocator>
	list<typename list<T, Allocator>::value_type, typename list<T, Allocator>::allocator_type> &
	list<T, Allocator>::reversed() {
		auto *other = new list<value_type, allocator_type>(*this);
		other->reverse();
		return *other;
	}
	template<class T, class Allocator>
	list<typename list<T, Allocator>::value_type, typename list<T, Allocator>::allocator_type> &
	list<T, Allocator>::reversed(iterator first, iterator last) {
		auto *other = new list<value_type, allocator_type>(*this);
		other->reverse(other->_head + this->distance(first, _head),
		               other->_head + this->distance(last, _head));
		return *other;
	}
	template<class T, class allocator>
	void list<T, allocator>::swap(list<value_type, allocator_type> &other) noexcept {
		using std::swap;
		swap(_head, other._head);
		swap(_tail, other._tail);
		swap(_size, other._size);
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::size_type list<T, Allocator>::size() const noexcept {
		return _size;
	}
	template<class T, class allocator>
	list<T, allocator>::operator std::string() const noexcept {
		using std::to_string;
		std::string output = '[' + to_string(this->size()) + "] ";
		for (reference element: *this)
			output += to_string(element) + " ";
		return output;
	}
	template<class T, class Allocator>
	collection<typename list<T, Allocator>::value_type,
	           typename list<T, Allocator>::iterator,
	           typename list<T, Allocator>::allocator_type> &
	list<T, Allocator>::subsequence(iterator from, iterator to) {
		auto *other = new list<value_type, allocator_type>(from, to);
		return *other;
	}
	template<class T, class Allocator>
	collection<typename list<T, Allocator>::value_type,
	           typename list<T, Allocator>::iterator,
	           typename list<T, Allocator>::allocator_type> &
	list<T, Allocator>::subsequence(size_type from, size_type to) {
		return subsequence(_head + from, _head + to);
	}

}

#endif //COLLECTIONS_LIST_HPP
