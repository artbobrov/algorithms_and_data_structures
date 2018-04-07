//
// Created by Artem Bobrov on 20.03.2018.
//
/*
namespace ctl {

	template<class T, class Allocator = std::allocator<T>>
	class list {
	public: // typedefs
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
		typedef std::function<void(reference)> action;
		typedef std::function<bool(const_reference, const_reference)> comparer;

		typedef __node<T> node;
		typedef node *node_point;
	public: // constructors & destructors
		explicit list(const Allocator &alloc = Allocator());

		explicit list(size_type count, const Allocator &alloc = Allocator());
		explicit list(size_type count, const_reference value, const Allocator &alloc = Allocator());

		template<class Iterator>
		explicit list(Iterator begin, Iterator end, const Allocator &alloc = Allocator(),
		                     typename std::enable_if<is_forward_iterator<Iterator>::value>::type * = 0);

		list(const list<value_type, allocator_type> &other);

		list(const list<value_type, allocator_type> &other, const Allocator &alloc);

		list(list &&other) noexcept;

		template<class Container, typename =
		typename std::enable_if<has_begin_end<Container>::value && !std::is_convertible<Container, list>::value>::type>
		list(const Container &__container);

		list(std::initializer_list<value_type> il, const Allocator &alloc = Allocator());

		virtual ~list();
	public: // operators
		list &operator=(const list &other);
		list &operator=(list &&other) noexcept;

		explicit operator std::string() const noexcept;
	public:
		template<typename R>
		R accumulate(R initial, std::function<R(R, const_reference)> next_result);
		allocator_type allocator() const noexcept;
		void append(element_accessible_modifiable<value_type, iterator> &other);
		void append(const_reference value);
		void append(value_type &&value);

		reference back();
		const_reference back() const;
		iterator begin() noexcept;
		const_iterator begin() const noexcept;
		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;
		void clear() noexcept;
		bool contains(const_reference item) const;
		bool contains(conformer predicate) const;
		size_type count(const_reference item) const noexcept;
		size_type count(conformer predicate) const noexcept;

		const_reverse_iterator crbegin() const noexcept;
		const_reverse_iterator crend() const noexcept;

		pointer data() noexcept;
		pointer data() const noexcept;

		template<class... Args>
		reference emplace(const_iterator position, Args &&... args);
		template<class... Args>
		reference emplace(size_type idx, Args &&... args);
		template<class... Args>
		reference emplace_back(Args &&... args);
		template<class... Args>
		reference emplace_front(Args &&... args);
		bool empty() const noexcept;
		iterator end() noexcept;
		const_iterator end() const noexcept;
		iterator erase(const_iterator position);
		iterator erase(const_iterator first, const_iterator last);

		void fill(iterator first, iterator last, const T &value);
		void fill(const T &value, size_type size);
		void fill(const T &value);
		void filter(conformer predicate);
		list filtered(conformer predicate);
		list<value_type, allocator_type> filtered(conformer predicate);
		iterator first(conformer predicate) const noexcept;
		iterator first(const_reference item) const;
		void for_each(action act) noexcept;
		reference front();
		const_reference front() const;

		iterator insert(const_iterator position, const_reference value);
		iterator insert(const_iterator position, value_type &&value);
		iterator insert(const_iterator position, size_type count, const_reference value);
		iterator insert(const_iterator position, std::initializer_list<value_type> il);
		template<class Iterator>
		iterator insert(const_iterator position, Iterator first, Iterator last);
		iterator iterator_of(const_reference value, iterator first, iterator last) const;

		template<typename U>
		list<U, allocator_type> map(std::function<U(reference)> mapper);
		iterator max() const;
		iterator max(comparer comp) const;
		size_type max_size() const noexcept;
		iterator min() const;
		iterator min(comparer comp) const;

		void pop_back();
		void pop_back(size_type count);
		void pop_front();
		void pop_front(size_type count);
		list<value_type, allocator_type> prefix(size_type max_length);
		list<value_type, allocator_type> prefix(conformer predicate);
		void push_back(const_reference value);
		void push_back(size_type count, const_reference value);
		void push_back(value_type &&value);
		void push_front(const_reference value);
		void push_front(value_type &&value);
		void push_front(size_type count, const_reference value);

		reverse_iterator rbegin() noexcept;
		const_reverse_iterator rbegin() const noexcept;
		void remove(const_reference item);
		void remove_all(const_reference item);
		void remove_all(iterator first, iterator last, const_reference item);
		void remove_all(conformer predicate);
		void remove_all(iterator first, iterator last, conformer predicate);
		void remove_first();
		void remove_last();
		reverse_iterator rend() noexcept;
		const_reverse_iterator rend() const noexcept;
		void resize(size_type count);
		void resize(size_type count, const value_type &value);
		void reverse();
		void reverse(iterator first, iterator last);

		size_type size() const noexcept;
		list<value_type, allocator_type> subsequence(iterator from, iterator to);
		list<value_type, allocator_type> suffix(size_type max_length);
		list<value_type, allocator_type> suffix(conformer predicate);
		void swap(list<value_type, allocator_type> &other);

		std::vector<value_type> to_std_vector() const;
		std::list<value_type> to_std_list() const;
		std::set<value_type> to_std_set() const;
		bool true_for_all(conformer conform) const;
	};

	//! Non member functions

	template<class T, class Allocator>
	bool operator==(const list<T, Allocator> &lhs, const list<T, Allocator> &rhs);

	template<class T, class Allocator>
	bool operator!=(const list<T, Allocator> &lhs, const list<T, Allocator> &rhs);

	template<class T, class Allocator>
	bool operator<(const list<T, Allocator> &lhs, const list<T, Allocator> &rhs);

	template<class T, class Allocator>
	bool operator>(const list<T, Allocator> &lhs, const list<T, Allocator> &rhs);

	template<class T, class Allocator>
	bool operator<=(const list<T, Allocator> &lhs, const list<T, Allocator> &rhs);

	template<class T, class Allocator>
	bool operator>=(const list<T, Allocator> &lhs, const list<T, Allocator> &rhs);
}
*/
#ifndef CONTAINERS_LIST_HPP
#define CONTAINERS_LIST_HPP

#include <iterator>
#include "../abstract/abstracts.hpp"

namespace ctl {
	template<class T>
	struct __node {
		typedef T value_type;

		value_type data;
		__node *prev;
		__node *next;

		inline bool operator==(const __node &other) {
			return other.data == data && other.next == next && other.prev == prev;
		}
	};

	template<class T>
	class __list_iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
	public:
		typedef T value_type;
		typedef value_type &reference;
		typedef value_type const &const_reference;

		typedef __node<T> node;
		typedef node *node_point;
		typedef size_t size_type;
	public:
		mutable node_point data_point;
	public:
		explicit __list_iterator(node_point point = nullptr) : data_point(point) {}

		__list_iterator(const __list_iterator &other) : data_point(other.data_point) {}

		const __list_iterator &operator=(const __list_iterator &other) const {
			if (this != &other) {
				data_point = other.data_point;
			}
			return *this;
		}

		__list_iterator &operator=(__list_iterator &&other) noexcept {
			data_point = other.data_point;
			other.data_point = nullptr;
			return *this;
		}

		inline __list_iterator &operator++() {
			data_point = data_point->next;
			return *this;
		}
		inline __list_iterator operator++(int) {
			__list_iterator value = *this;
			++*this;
			return value;
		}
		inline const __list_iterator &operator++() const {
			data_point = data_point->next;
			return *this;
		}
		inline const __list_iterator operator++(int) const {
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
		inline const __list_iterator &operator--() const {
			data_point = data_point->prev;
			return *this;
		}
		inline const __list_iterator operator--(int) const {
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
	};

	template<class T, class Allocator = std::allocator<__node<T>>>
	class list : public bidirectional_access_collection<T, __list_iterator<T>, Allocator>,
	             public bidirectional_element_accessible_modifiable<T, __list_iterator<T>>,
	             public data_accessible<__node<T>> {
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
		typedef std::function<void(reference)> action;
		typedef std::function<bool(const_reference, const_reference)> comparer;

		typedef __node<T> node;
		typedef node *node_point;
	private:
		iterator _head;
		iterator _tail;
		size_type _size = 0;
	private:
		template<class... Args>
		inline node_point _new_node(Args &&...args);
		inline void _delete_node(node_point point);
		template<class... Args>
		inline void _construct_at(node_point node, Args &&...args);
	public:
		inline explicit list(const Allocator &alloc = Allocator())
			: list::bidirectional_access_collection(alloc), _head(_new_node()), _tail(_head), _size(0) {
			_head.data_point->next = _tail.data_point;
			_tail.data_point->prev = _head.data_point;
		}

		inline explicit list(size_type count, const Allocator &alloc = Allocator()) :
			list(alloc) { for (; count > 0; --count) push_back(value_type()); }
		inline explicit list(size_type count, const_reference value, const Allocator &alloc = Allocator()) :
			list(alloc) { for (; count > 0; --count) push_back(value); }

		template<class Iterator>
		inline explicit list(Iterator begin, Iterator end, const Allocator &alloc = Allocator(),
		                     typename std::enable_if<is_forward_iterator<Iterator>::value>::type * = 0);

		inline list(const list<value_type, allocator_type> &other) :
			list(other.begin(), other.end(), other.allocator()) {}

		inline list(const list<value_type, allocator_type> &other, const Allocator &alloc) :
			list(other.begin(), other.end(), alloc) {}

		inline list(list &&other) noexcept;

		template<class Container, typename = typename std::enable_if<has_begin_end<Container>::value &&
			!std::is_convertible<Container, list>::value>::type>
		inline list(const Container &__container): list(__container.begin(), __container.end()) {}

		inline list(std::initializer_list<value_type> il, const Allocator &alloc = Allocator()) :
			list(il.begin(), il.end(), alloc) {}

		inline virtual ~list() { clear(); }
	public: // operators
		list &operator=(const list &other);
		list &operator=(list &&other) noexcept;

	public:
		inline void append(bidirectional_element_accessible_modifiable<value_type, iterator> &value) override;

		inline reference back() override { return _tail.data_point->prev->data; }
		inline const_reference back() const override { return _tail.data_point->prev->data; }
		inline iterator begin() noexcept override { return _head; }
		inline const_iterator begin() const noexcept override { return _head; }

		inline void clear() noexcept override;

		inline node_point data() noexcept override { return _head.data_point; }
		inline const node_point data() const noexcept override { return _head.data_point; }

		template<class... Args>
		inline reference emplace(const_iterator position, Args &&... args);
		template<class... Args>
		inline reference emplace_back(Args &&... args);
		template<class... Args>
		inline reference emplace_front(Args &&... args);
		inline iterator end() noexcept override { return _tail; }
		inline const_iterator end() const noexcept override { return _tail; }
		inline iterator erase(const_iterator position) override;
		inline iterator erase(const_iterator first, const_iterator last) override;

		using bidirectional_access_sequence<value_type, iterator>::fill;
		inline void fill(iterator first, iterator last, const_reference value) override;
		inline void fill(const_reference value, size_type size) override;
		inline void filter(conformer predicate) override;
		inline list filtered(conformer predicate);
		inline reference front() override { return *_head; }
		inline const_reference front() const override { return *_head; }

		inline iterator insert(const_iterator position, const_reference value) override;
		inline iterator insert(const_iterator position, value_type &&value) override;
		inline iterator insert(const_iterator before, size_type count, const_reference value) override;
		inline iterator insert(const_iterator position, std::initializer_list<value_type> il) override;
		template<class Iterator, typename = typename std::enable_if<is_forward_iterator<Iterator>::value>::type>
		inline iterator insert(const_iterator position, Iterator first, Iterator last);

		template<typename U>
		inline list<U, allocator_type> map(std::function<U(reference)> mapper);

		inline void pop_back() override;
		inline void pop_back(size_type count) override;
		inline void pop_front() override;
		inline void pop_front(size_type count) override;
		inline void push_back(const_reference value) override;
		inline void push_back(size_type count, const_reference value) override;
		inline void push_back(value_type &&value) override;
		inline void push_front(const_reference value) override;
		inline void push_front(value_type &&value) override;
		inline void push_front(size_type count, const_reference value) override;
		inline list prefix(size_type max_length);
		inline list prefix(conformer predicate);

		using bidirectional_element_accessible_modifiable<value_type, iterator>::remove_all;
		inline void remove_all(iterator first, iterator last, const_reference item) override;
		inline void remove_all(iterator first, iterator last, conformer predicate) override;
		inline void remove(const_reference item) override;
		inline void resize(size_type count) override;
		inline void resize(size_type count, const value_type &value) override;
		inline list reversed();

		inline list suffix(size_type max_length);
		inline list suffix(conformer predicate);
		inline size_type size() const noexcept override { return _size; }
		inline list<value_type, allocator_type> subsequence(iterator from, iterator to);
		inline void swap(list<value_type, allocator_type> &other);
	};
	/*
	 * Non member functions: BEGIN
	 */
	template<class T, class Allocator>
	inline bool operator==(const list<T, Allocator> &lhs, const list<T, Allocator> &rhs) {
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
	inline bool operator!=(const list<T, Allocator> &lhs, const list<T, Allocator> &rhs) {
		return !(lhs == rhs);
	}

	template<class T, class Allocator>
	inline bool operator<(const list<T, Allocator> &lhs, const list<T, Allocator> &rhs) {
		auto lfirst = lhs.begin(), llast = lhs.end();
		auto rfirst = rhs.begin(), rlast = rhs.end();
		for (; rfirst != rlast; ++lfirst, ++rfirst) {
			if (lfirst == llast || *lfirst < *rfirst)
				return true;
			if (*rfirst < *lfirst)
				return false;
		}
		return false;
	}
	template<class T, class Allocator>
	inline bool operator>(const list<T, Allocator> &lhs, const list<T, Allocator> &rhs) {
		return rhs < lhs;
	}
	template<class T, class Allocator>
	inline bool operator<=(const list<T, Allocator> &lhs, const list<T, Allocator> &rhs) {
		return !(rhs < lhs);
	}
	template<class T, class Allocator>
	inline bool operator>=(const list<T, Allocator> &lhs, const list<T, Allocator> &rhs) {
		return !(lhs < rhs);
	}
	/*
	 * Non member functions: END
	 */

	template<class T, class Allocator>
	template<class... Args>
	typename list<T, Allocator>::node_point list<T, Allocator>::_new_node(Args &&...args) {
		node_point new_node = this->_allocator.allocate(1);
		_construct_at(new_node, std::forward<Args>(args)...);
		new_node->prev = new_node->next = nullptr;

		return new_node;
	}
	template<class T, class Allocator>
	void list<T, Allocator>::_delete_node(node_point point) {
		this->_allocator.destroy(point);
		this->_allocator.deallocate(point, 1);
	}
	template<class T, class Allocator>
	template<class... Args>
	void list<T, Allocator>::_construct_at(node_point node, Args &&... args) {
		this->_allocator.construct(&(node->data), std::forward<Args>(args)...);
	}

	template<class T, class Allocator>
	template<class Iterator>
	list<T, Allocator>::list(Iterator begin,
	                         Iterator end,
	                         const Allocator &alloc,
	                         typename std::enable_if<is_forward_iterator<Iterator>::value>::type *): list(alloc) {
		for (; begin != end; ++begin)
			push_back(*begin);
	}

	template<class T, class Allocator>
	list<T, Allocator>::list(list &&other) noexcept {
		_head = other._head;
		_tail = other._tail;
		_size = other._size;
		other._head.data_point = other._tail.data_point = nullptr;
		other._size = 0;
	}
	template<class T, class Allocator>
	list<T, Allocator> &list<T, Allocator>::operator=(const list &other) {
		if (this != &other) {
			list(other).swap(*this);
		}
		return *this;
	}
	template<class T, class Allocator>
	list<T, Allocator> &list<T, Allocator>::operator=(list &&other) noexcept {
		_head = other._head;
		_tail = other._tail;
		_size = other._size;
		other._head.data_point = other._tail.data_point = nullptr;
		other._size = 0;

		return *this;
	}

	template<class T, class Allocator>
	void list<T, Allocator>::fill(iterator first, iterator last, const_reference value) {
		for (; first != last; ++first)
			*first = value;
	}
	template<class T, class Allocator>
	void list<T, Allocator>::fill(const_reference value, size_type size) {
		resize(size);
		fill(begin(), end(), value);
	}
	template<class T, class Allocator>
	void list<T, Allocator>::filter(conformer predicate) {
		iterator result = begin();
		for (iterator first = begin(); first != end();) {
			iterator tmp = first++;
			if (!predicate(*tmp))
				erase(tmp);
		}
	}
	template<class T, class Allocator>
	list<T, Allocator> list<T, Allocator>::filtered(conformer predicate) {
		list other(*this);
		other.filter(predicate);
		return other;
	}

	template<class T, class Allocator>
	void list<T, Allocator>::append(bidirectional_element_accessible_modifiable<T, iterator> &value) {
		for (reference element: value)
			push_back(element);
	}
	template<class T, class Allocator>
	template<class... Args>
	typename list<T, Allocator>::reference list<T, Allocator>::emplace(const_iterator position, Args &&... args) {
		if (position == this->begin())
			return emplace_front(std::forward<Args>(args)...);
		else if (position == this->end())
			return emplace_back(std::forward<Args>(args)...);
		node_point new_node = _new_node(std::forward<Args>(args)...);
		new_node->next = position.data_point;
		new_node->prev = position.data_point->prev;
		position.data_point->prev->next = new_node;
		position.data_point->prev = new_node;
		++_size;
		return new_node->data;
	}
	template<class T, class Allocator>
	template<class... Args>
	typename list<T, Allocator>::reference list<T, Allocator>::emplace_back(Args &&... args) {
		node_point new_tail = _new_node();
		_construct_at(_tail.data_point, std::forward<Args>(args)...);
		new_tail->prev = _tail.data_point;
		_tail.data_point->next = new_tail;
		_tail.data_point = new_tail;
		++_size;
		return new_tail->data;
	}
	template<class T, class Allocator>
	template<class... Args>
	typename list<T, Allocator>::reference list<T, Allocator>::emplace_front(Args &&... args) {
		node_point new_head = _new_node(std::forward<Args>(args)...);
		new_head->next = _head.data_point;
		_head.data_point->prev = new_head;
		_head.data_point = new_head;
		++_size;
		return new_head->data;
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::iterator list<T, Allocator>::erase(const_iterator position) {
		if (position == this->begin()) {
			pop_front();
			return begin();
		} else if (position == this->end()) {
			pop_back();
			return end();
		}

		node_point deleted_node = position.data_point;

		deleted_node->next->prev = deleted_node->prev;
		deleted_node->prev->next = deleted_node->next;
		node_point result = deleted_node->next;
		_delete_node(deleted_node);
		--_size;

		return iterator(result);
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::iterator list<T, Allocator>::erase(const_iterator first, const_iterator last) {
		iterator result;
		for (; first != last;) {
			iterator tmp = first++;
			result = erase(tmp);
		}
		return result;
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator position, const_reference value) {
		if (position == this->begin()) {
			push_front(value);
			return this->begin();
		} else if (position == this->end()) {
			push_back(value);
			return this->end();
		}

		node_point new_node = _new_node(value);
		position.data_point->prev->next = new_node;
		new_node->prev = position.data_point->prev;
		position.data_point->prev = new_node;
		new_node->next = position.data_point;
		++_size;
		return iterator(new_node);
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator position, value_type &&value) {
		node_point new_node = _new_node(std::move(value));
		position.data_point->prev->next = new_node;
		new_node->prev = position.data_point->prev;
		position.data_point->prev = new_node;
		new_node->next = position.data_point;
		++_size;
		return iterator(new_node);
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator before,
	                                                                 size_type count,
	                                                                 const_reference value) {
		iterator result = before;
		for (; count > 0; --count)
			before = insert(before, value);
		return before;
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator position,
	                                                                 std::initializer_list<value_type> il) {
		return this->insert(position, il.begin(), il.end());
	}
	template<class T, class Allocator>
	template<class Iterator, typename>
	typename list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator position,
	                                                                 Iterator first,
	                                                                 Iterator last) {
		iterator result = position;
		iterator pos = position;
		for (Iterator it = first; it != last; ++it, ++pos) {
			pos = insert(pos, *it);
			if (it == first)
				result = pos;
		}
		return result;
	}
	template<class T, class Allocator>
	template<typename U>
	list<U, Allocator> list<T, Allocator>::map(std::function<U(reference)> mapper) {
		list<U, allocator_type> other(size(), this->allocator());
		iterator first = begin();
		for (typename list<U, allocator_type>::iterator it = other.begin(); it != other.end() && first != end();
		     ++it, ++first)
			*it = mapper(*first);
		return other;
	}

	template<class T, class Allocator>
	void list<T, Allocator>::pop_back() {
		if (size() > 0) {
			node_point deleted_node = _tail.data_point->prev;
			_tail.data_point->prev = deleted_node->prev;
			_tail.data_point->prev->next = _tail.data_point;
			_delete_node(deleted_node);
			--_size;
		}
	}
	template<class T, class Allocator>
	void list<T, Allocator>::pop_back(size_type count) {
		for (; count > 0; --count)
			pop_back();
	}
	template<class T, class Allocator>
	void list<T, Allocator>::pop_front() {
		if (size() > 0) {
			auto oldNode = _head.data_point;
			_head.data_point = oldNode->next;
			_head.data_point->prev = nullptr;
			_delete_node(oldNode);
			--_size;
		}
	}
	template<class T, class Allocator>
	void list<T, Allocator>::pop_front(size_type count) {
		for (; count > 0; --count)
			pop_front();
	}
	template<class T, class Allocator>
	void list<T, Allocator>::push_back(const_reference value) {
		node_point new_tail = _new_node();
		_tail.data_point->data = value;
		_tail.data_point->next = new_tail;
		new_tail->prev = _tail.data_point;
		_tail.data_point = new_tail;
		++_size;
	}
	template<class T, class Allocator>
	void list<T, Allocator>::push_back(size_type count, const_reference value) {
		for (; count > 0; --count)
			push_back(value);
	}
	template<class T, class Allocator>
	void list<T, Allocator>::push_back(value_type &&value) {
		node_point new_tail = _new_node();
		_tail.data_point->data = std::move(value);
		_tail.data_point->next = new_tail;
		new_tail->prev = _tail.data_point;
		_tail.data_point = new_tail;
		++_size;
	}
	template<class T, class Allocator>
	void list<T, Allocator>::push_front(const_reference value) {
		node_point new_node = _new_node(value);
		_head.data_point->prev = new_node;
		new_node->next = _head.data_point;
		_head.data_point = new_node;
		++_size;
	}
	template<class T, class Allocator>
	void list<T, Allocator>::push_front(value_type &&value) {
		node_point new_node = _new_node(std::move(value));
		_head.data_point->prev = new_node;
		new_node->next = _head.data_point;
		_head.data_point = new_node;
		++_size;
	}
	template<class T, class Allocator>
	void list<T, Allocator>::push_front(size_type count, const_reference value) {
		for (; count > 0; --count)
			push_front(value);
	}
	template<class T, class Allocator>
	list<T, Allocator> list<T, Allocator>::prefix(size_type max_length) {
		size_type _length = std::min(max_length, size());
		iterator last = this->begin();
		for (; _length > 0; --_length, ++last);
		return list(this->begin(), last, this->allocator());
	}
	template<class T, class Allocator>
	list<T, Allocator> list<T, Allocator>::prefix(conformer predicate) {
		iterator last = begin();
		for (; last != end() && predicate(*last); ++last) {}
		return list(begin(), last, this->allocator());
	}
	template<class T, class Allocator>
	void list<T, Allocator>::remove_all(iterator first, iterator last, const_reference item) {
		iterator result = first;
		for (; first != last; ++first)
			if (!(*first == item))
				*result++ = *first;

		_tail = result;
	}
	template<class T, class Allocator>
	void list<T, Allocator>::remove_all(iterator first, iterator last, conformer predicate) {
		iterator result = first;
		for (; first != last; ++first)
			if (!predicate(*first))
				*result++ = *first;

		_tail = result;

	}
	template<class T, class Allocator>
	void list<T, Allocator>::remove(const_reference item) {
		iterator it = this->first(item);
		if (it != end())
			erase(it);
	}
	template<class T, class Allocator>
	void list<T, Allocator>::resize(size_type count) {
		long long diff = count - size();
		if (diff > 0)
			push_back(diff, value_type());
		else if (diff < 0)
			pop_back(diff);
	}
	template<class T, class Allocator>
	void list<T, Allocator>::resize(size_type count, const_reference value) {
		long long diff = count - size();
		if (diff > 0)
			push_back(diff, value);
		else if (diff < 0)
			pop_back(diff);
	}
	template<class T, class Allocator>
	list<T, Allocator> list<T, Allocator>::reversed() {
		return list(this->rbegin(), this->rend(), this->allocator());
	}

	template<class T, class Allocator>
	void list<T, Allocator>::clear() noexcept {
		if (!this->empty()) {
			node_point begin = _head.data_point, end = _tail.data_point;
			_head.data_point = _tail.data_point = nullptr;
			_size = 0;

			while (begin != end) {
				node_point deleted_node = begin;
				begin = begin->next;
				_delete_node(deleted_node);
			}
		}
	}
	template<class T, class Allocator>
	list<T, Allocator> list<T, Allocator>::suffix(conformer predicate) {
		iterator first = end();
		for (; first != begin() && predicate(*first); --first) {}
		return list(first, end(), this->allocator());
	}
	template<class T, class Allocator>
	list<T, Allocator> list<T, Allocator>::suffix(size_type max_length) {
		size_type _size = std::min(max_length, size());
		iterator last = this->end();
		for (; _size > 0; --_size, --last) {}
		return list(last, this->end(), this->allocator());
	}
	template<class T, class Allocator>
	list<T, Allocator> list<T, Allocator>::subsequence(iterator from, iterator to) {
		return list<value_type, allocator_type>(from, to);
	}
	template<class T, class Allocator>
	void list<T, Allocator>::swap(list<value_type, allocator_type> &other) {
		using std::swap;

		swap(other._tail, _tail);
		swap(other._head, _head);
		swap(other._size, _size);
	}
}

#endif //CONTAINERS_LIST_HPP
