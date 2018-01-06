//
// Created by Artem Bobrov on 05.01.2018.
//

#ifndef COLLECTIONS_LIST_HPP
#define COLLECTIONS_LIST_HPP

#include "../abstract/collection.hpp"
#include "../abstract/iterator.hpp"
#include <iterator>

namespace ctl {
	struct __list_iterator;
	template<class T, class Allocator = std::allocator<T> >
	class list : public collection<T, __list_iterator, Allocator> {
	private:
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

		struct __list_iterator : public __iterator<std::bidirectional_iterator_tag, T> {
		public:
			typedef T value_type;
			typedef value_type &reference;
			typedef value_type const &const_reference;

			typedef __node node;
			typedef node *node_point;
			typedef size_t size_type;
		public:
			node_point data_point;
		public:
			explicit __list_iterator(node_point point = nullptr) : data_point(point) {}

			inline __list_iterator &operator++() override {
				data_point = data_point->next;
				return *this;
			}
			inline __list_iterator operator++(int) {
				__list_iterator value = *this;
				++*this;
				return value;
			}
			inline __list_iterator &operator--() override {
				data_point = data_point->prev;
				return *this;
			}
			inline __list_iterator operator--(int) {
				__list_iterator value = *this;
				--*this;
				return value;
			}
			inline T &operator*() override { return data_point->data; }
			inline T *operator->() override { return &(operator*()); }

			inline bool operator==(const __list_iterator &rhs) override {
				return this->data_point == rhs.data_point;
			}

			inline __list_iterator &plus(size_type idx) override {
				for (; idx > 0; --idx)
					++*this;
				return *this;
			}
			inline __list_iterator &minus(size_type idx) override {
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
	public:
		typedef Allocator allocator_type;
		typedef T value_type;
		typedef __list_iterator iterator;
		typedef const iterator const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef const std::reverse_iterator<iterator> const_reverse_iterator;
		typedef iterator pointer;
		typedef value_type &reference;
		typedef value_type const &const_reference;
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef std::function<bool(const_reference)> conformer;
	private:
		iterator _head;
		iterator _tail;
		size_type _size = 0;
	public:
		list() : list(Allocator()), _head(iterator()), _tail(iterator()), _size(0) {}
		explicit list(const Allocator &alloc);

		explicit list(size_type count, const T &value = T(), const Allocator &alloc = Allocator());
		explicit list(size_type count, const Allocator &alloc = Allocator());
		list(const list &other);
		list(const list &other, const Allocator &alloc);
		list(list &&other) noexcept: list(other, other.allocator()) {}
		list(list &&other, const Allocator &alloc) noexcept;
		list(std::initializer_list<T> il, const Allocator &alloc = Allocator());

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
		inline list<value_type, allocator_type> &filled(const T &value);
		inline list<value_type, allocator_type> &filled(const T &value, size_type size);

		inline iterator insert(const_iterator position, const T &value) override;
		inline iterator insert(size_type idx, const T &value) override;
		inline iterator insert(const_iterator position, value_type &&value) override;
		inline iterator insert(const_iterator position, size_type count, const T &value) override;
		inline iterator insert(const_iterator position, std::initializer_list<value_type> il) override;
		inline iterator insert(size_type idx, size_type count, const T &value) override;
		inline iterator insert(const_iterator position, const_iterator first, const_iterator last);

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
		inline void shrink_to_fit() noexcept override;

		inline collection<value_type, iterator, allocator_type> &subsequence(const_iterator from,
		                                                                     const_iterator to) override;
		inline collection<value_type, iterator, allocator_type> &subsequence(size_type from, size_type to) override;
	};

	template<class T, class Allocator>
	list<T, Allocator>::list(const Allocator &alloc):
		collection<value_type, iterator, allocator_type>(alloc), _head(iterator()), _tail(iterator()), _size(0) {
	}
	template<class T, class Allocator>
	list<T, Allocator>::list(size_type count, const Allocator &alloc):
		collection<value_type, iterator, allocator_type>(alloc), _size(count) {
		this->reserve(count);
	}

	template<class T, class Allocator>
	list<T, Allocator>::list(size_type count, const T &value, const Allocator &alloc): list(count, alloc) {
		this->fill(value);
	}

	template<class T, class Allocator>
	list<T, Allocator>::list(const list<value_type, allocator_type> &other):
		collection<value_type, iterator, allocator_type>(other.allocator()) {
		_NOT_IMPLEMENTED_;
	}
	template<class T, class Allocator>
	list<T, Allocator>::list(const list<value_type, allocator_type> &other, const Allocator &alloc)
		: collection<value_type, iterator, allocator_type>(alloc), list(other) {

	}
	template<class T, class Allocator>
	list<T, Allocator>::list(list &&other, const Allocator &alloc) noexcept:
		list(other.size(), alloc) {
		_NOT_IMPLEMENTED_;
	}

	template<class T, class Allocator>
	list<T, Allocator>::~list() {
		_NOT_IMPLEMENTED_;

	}
	template<class T, class Allocator>
	list<T, Allocator>::list(std::initializer_list<value_type> il, const Allocator &alloc): list(il.size(),
	                                                                                             alloc) {
		_NOT_IMPLEMENTED_;
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
		_NOT_IMPLEMENTED_;
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::reference list<T, Allocator>::back() {
		return *(_tail - 1);
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::const_reference list<T, Allocator>::back() const {
		return *(_tail - 1);
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
		_NOT_IMPLEMENTED_;
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::size_type list<T, Allocator>::capacity() const noexcept {
		return _size;
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::pointer list<T, Allocator>::data() noexcept {
		_NOT_IMPLEMENTED_;
	}
	template<class T, class Allocator>
	const typename list<T, Allocator>::pointer list<T, Allocator>::data() const noexcept {
		_NOT_IMPLEMENTED_;
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
		_NOT_IMPLEMENTED_;
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::iterator list<T, Allocator>::erase(const_iterator first,
	                                                                const_iterator last) {
		_NOT_IMPLEMENTED_;
	}
	template<class T, class Allocator>
	bool list<T, Allocator>::empty() const noexcept {
		return _tail == _head;
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
		auto *other = new list<value_type, allocator_type>(size(), this->allocator());
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
		_NOT_IMPLEMENTED_;
	}

	template<class T, class allocator>
	typename list<T, allocator>::iterator list<T, allocator>::insert(size_type idx,
	                                                                 size_type count,
	                                                                 const T &value) {
		_NOT_IMPLEMENTED_;
	}

	template<class T, class allocator>
	typename list<T, allocator>::iterator list<T, allocator>::insert(const_iterator position,
	                                                                 size_type count,
	                                                                 const T &value) {
		_NOT_IMPLEMENTED_;
	}

	template<class T, class allocator>
	typename list<T, allocator>::iterator list<T, allocator>::insert(const_iterator position, const T &value) {
		_NOT_IMPLEMENTED_;
	}

	template<class T, class allocator>
	typename list<T, allocator>::iterator list<T, allocator>::insert(const_iterator position, value_type &&value) {
		_NOT_IMPLEMENTED_;

	}

	template<class T, class allocator>
	typename list<T, allocator>::iterator list<T, allocator>::insert(const_iterator position,
	                                                                 std::initializer_list<value_type> il) {
		_NOT_IMPLEMENTED_;
	}

	template<class T, class Allocator>
	typename list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator position,
	                                                                 const_iterator first,
	                                                                 const_iterator last) {
		_NOT_IMPLEMENTED_;
	}
	template<class T, class allocator>
	void list<T, allocator>::pop_back() {
		_NOT_IMPLEMENTED_;
	}

	template<class T, class allocator>
	void list<T, allocator>::pop_front() {
		_NOT_IMPLEMENTED_;
	}
	template<class T, class allocator>
	void list<T, allocator>::push_back(const_reference value) {
		_NOT_IMPLEMENTED_;
	}
	template<class T, class allocator>
	void list<T, allocator>::push_back(value_type &&value) {
		_NOT_IMPLEMENTED_;
	}

	template<class T, class allocator>
	void list<T, allocator>::push_front(const_reference value) {
		_NOT_IMPLEMENTED_;
	}

	template<class T, class allocator>
	void list<T, allocator>::push_front(value_type &&value) {
		_NOT_IMPLEMENTED_;
	}
	template<class T, class allocator>
	void list<T, allocator>::resize(size_type sz) {
		if (!sz)
			clear();

		reserve(sz);
	}

	template<class T, class allocator>
	void list<T, allocator>::resize(size_type sz, const T &value) {
		_NOT_IMPLEMENTED_;
	}
	template<class T, class allocator>
	void list<T, allocator>::reserve(size_type n) {
		_NOT_IMPLEMENTED_;
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
	void list<T, allocator>::shrink_to_fit() noexcept {
	}
	template<class T, class allocator>
	void list<T, allocator>::swap(list<value_type, allocator_type> &other) noexcept {
		using std::swap;
		_NOT_IMPLEMENTED_;
	}
	template<class T, class Allocator>
	typename list<T, Allocator>::size_type list<T, Allocator>::size() const noexcept {
		return _size;
	}
	template<class T, class allocator>
	list<T, allocator>::operator std::string() const noexcept {
		using std::to_string;
		std::string output = '[' + to_string(this->size()) + ',' + to_string(this->capacity()) + "] ";
		for (reference element: *this)
			output += to_string(element) + " ";
		return output;
	}
	template<class T, class Allocator>
	collection<typename list<T, Allocator>::value_type,
	           typename list<T, Allocator>::iterator,
	           typename list<T, Allocator>::allocator_type> &
	list<T, Allocator>::subsequence(const_iterator from, const_iterator to) {
		_NOT_IMPLEMENTED_;
	}
	template<class T, class Allocator>
	collection<typename list<T, Allocator>::value_type,
	           typename list<T, Allocator>::iterator,
	           typename list<T, Allocator>::allocator_type> &
	list<T, Allocator>::subsequence(size_type from, size_type to) {
		_NOT_IMPLEMENTED_;
	}
}

#endif //COLLECTIONS_LIST_HPP
