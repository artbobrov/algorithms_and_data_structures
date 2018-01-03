//
// Created by Artem Bobrov on 02.01.2018.
//

#ifndef COLLECTIONS_VECTOR_HPP
#define COLLECTIONS_VECTOR_HPP
#include "../abstract/collection.hpp"

namespace ctl {
	template<class T, class Allocator = std::allocator<T> >
	class vector : public collection<T, Allocator> {
	public:
		typedef Allocator allocator_type;
		typedef T value_type;
		typedef value_type *iterator;
		typedef const iterator const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef const std::reverse_iterator<iterator> const_reverse_iterator;
		typedef iterator pointer;
		typedef value_type &reference;
		typedef const reference const_reference;
		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef std::function<bool(const_reference)> conformer;
		typedef std::function<void(reference)> action;
		typedef std::function<bool(const_reference, const_reference)> comparer;
	public:
		vector() : _begin(nullptr), _end(nullptr), _storage_end(nullptr) {}

		explicit vector(size_type count, const Allocator &alloc = Allocator()) {
			this->_allocator = alloc;
			_begin = this->_allocator.allocate(count);
			_storage_end = _end = _begin + count;
		}

		explicit vector(size_type count, const T &value = T(), const Allocator &alloc = Allocator()) {
			this->_allocator = alloc;

			_begin = this->_allocator.allocate(count);
			_storage_end = _end = _begin + count;
			this->fill(value);
		}
		explicit vector(const vector<value_type, allocator_type> &other, const Allocator &alloc = Allocator()) {
			this->_allocator = alloc;
			_begin = this->_allocator.allocate(other.capacity());
			_storage_end = _end = _copy_data(other.begin(), other.end(), _begin);
		}
		inline virtual ~vector() { _delete_data(begin(), _storage_end); }
	public:
		reference at(size_type i) override;
		const_reference at(size_type i) const override;
		void append(const collection<value_type, allocator_type> &value) override;

		reference back() override; // OK
		const_reference back() const override; // OK
		iterator begin() noexcept override; // OK
		const_iterator begin() const noexcept override; // OK

		const_iterator cbegin() const noexcept override; // OK
		const_iterator cend() const noexcept override; // OK
		void clear() noexcept override; // OK
		size_type capacity() const noexcept override; // OK

		pointer data() noexcept override; // OK
		const pointer data() const noexcept override; // OK

		iterator end() noexcept override; // OK
		const_iterator end() const noexcept override; // OK
		iterator erase(const_iterator position) override; // OK
		iterator erase(const_iterator first, const_iterator last) override; // OK
		bool empty() const noexcept override; // OK
		template<class... Args>
		inline iterator emplace(const_iterator position, Args &&... args);
		template<class... Args>
		inline iterator emplace_hint(const_iterator hint, Args &&... args);
		template<class... Args>
		inline reference emplace_back(Args &&... args);

		reference front() override; // OK
		const_reference front() const override; // OK
		inline vector<value_type, allocator_type> &filled(const T &value); // qt+swift +/- OK
		inline vector<value_type, allocator_type> &filled(const T &value, size_type size); // qt+swift N_I


		iterator insert(const_iterator position, const T &value) override; // OK
		iterator insert(size_type idx, const T &value) override; // OK
		iterator insert(const_iterator position, value_type &&value) override; // OK
		iterator insert(const_iterator position, size_type count, const T &value) override; // OK
		iterator insert(const_iterator position, std::initializer_list<value_type> il) override;
		iterator insert(size_type idx, size_type count, const T &value) override;

		template<class InputIterator>
		iterator insert(const_iterator position, InputIterator first, InputIterator last);

		void pop_back() override; // OK
		void pop_front() override; // OK
		void push_back(const_reference value) override; // OK
		void push_back(value_type &&value) override; // OK
		void push_front(const_reference value) override; // OK
		void push_front(value_type &&value) override; // OK

		void resize(size_type count) override; // OK
		void resize(size_type count, const T &value) override; // OK
		void reserve(size_type n) override; // OK
		inline vector<value_type, allocator_type> &reversed(); // c# +/- OK
		inline vector<value_type, allocator_type> &reversed(iterator first, iterator last); // c# +/- OK

		void swap(vector<value_type, allocator_type> &other) noexcept; // OK
		explicit operator std::string() const noexcept override; // OK
		size_type size() const noexcept override; // OK
		void shrink_to_fit() noexcept override; // OK
	protected:
		iterator _begin;
		iterator _end;
		iterator _storage_end;
		inline void _delete_data(iterator from, iterator to);
		inline iterator _copy_data(iterator begin, iterator end, iterator to); // returns end iterator to copied data
		inline size_type _recommend(size_type new_size);
	};
	template<class T, class Allocator>
	void vector<T, Allocator>::_delete_data(iterator from, iterator to) {

		iterator _start = from;
		size_type _sz = static_cast<size_type>(to - from);
		for (; from != to; ++from)
			this->_allocator.destroy(from);
		if (this->capacity() && this->begin() != nullptr)
			this->_allocator.deallocate(_start, _sz);
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::_copy_data(iterator begin,
	                                                                         iterator end,
	                                                                         iterator to) {
		for (; begin != end; ++begin, ++to)
			this->_allocator.construct(to, *begin);

		return to;
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::size_type vector<T, Allocator>::_recommend(size_type new_size) {
		size_type _max_size = this->max_size();
		if (_max_size < new_size)
			throw std::length_error("vector");
		if (capacity() > _max_size / 2)
			return _max_size;
		return std::max(2 * capacity(), new_size);
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::at(size_type i) {
		return *(_begin + i);
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::at(size_type i) const {
		return *(_begin + i);
	}
	template<class T, class Allocator>
	void vector<T, Allocator>::append(const collection<value_type, allocator_type> &value) {
		if (size() + value.size() > capacity()) {
			size_type __capacity = _recommend(size() + value.size());
			iterator _start = this->_allocator.allocate(__capacity, _begin);

			_end = _copy_data(begin(), end(), _start);

			_delete_data(begin(), _storage_end);
			_begin = _start;
			_storage_end = _begin + __capacity;
		}

		for (const_reference element: value)
			this->_allocator.construct(_end++, element);
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
		_delete_data(begin(), _storage_end);
		_begin = _end = _storage_end = nullptr;
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::size_type vector<T, Allocator>::capacity() const noexcept {
		return static_cast<size_type>(_storage_end - _begin);
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::pointer vector<T, Allocator>::data() noexcept {
		return _begin;
	}
	template<class T, class Allocator>
	const typename vector<T, Allocator>::pointer vector<T, Allocator>::data() const noexcept {
		return _begin;
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

		_NOT_IMPLEMENTED_;
		return nullptr;
	}
	template<class T, class Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::erase(const_iterator first,
	                                                                    const_iterator last) {
		_NOT_IMPLEMENTED_;
		return nullptr;
	}
	template<class T, class Allocator>
	bool vector<T, Allocator>::empty() const noexcept {
		return _begin == _end;
	}
	template<class T, class Allocator>
	template<class... Args>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::emplace(const_iterator position,
	                                                                      Args &&... args) {
		_NOT_IMPLEMENTED_;
		return nullptr;
	}

	template<class T, class Allocator>
	template<class... Args>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::emplace_hint(const_iterator hint,
	                                                                           Args &&... args) {
		_NOT_IMPLEMENTED_;
		return nullptr;
	}

	template<class T, class Allocator>
	template<class... Args>
	typename vector<T, Allocator>::reference vector<T, Allocator>::emplace_back(Args &&... args) {
		_NOT_IMPLEMENTED_;
		return nullptr;
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
	vector<typename vector<T, Allocator>::value_type, typename vector<T, Allocator>::allocator_type> &vector<T,
	                                                                                                         Allocator>::filled(
		const T &value) {

		// TODO: IS WITH THE ONLY WAY?
		//		_NOT_IMPLEMENTED_;
		vector<value_type, allocator_type> *other = new vector<value_type, allocator_type>(*this);
		other->fill(value);
		return *other;
	}
	template<class T, class Allocator>
	vector<typename vector<T, Allocator>::value_type, typename vector<T, Allocator>::allocator_type> &vector<T,
	                                                                                                         Allocator>::filled(
		const T &value,
		size_type size) {
		// TODO: IS WITH THE ONLY WAY?
		//		_NOT_IMPLEMENTED_;
		vector<value_type, allocator_type> *other = new vector<value_type, allocator_type>(*this);
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
		size_type __capacity = _recommend(std::max(size() + count, static_cast<size_type>(position - _begin + count)));
		iterator _start = this->_allocator.allocate(__capacity);
		iterator middle = _copy_data(_begin, std::min(_end, position), _start);
		if (_end < position) {
			middle += static_cast<size_type>(position - _end);
			_end = middle + count;

		} else {
			_end = _copy_data(position, _end, middle + count);
		}
		for (size_t idx = 0; idx < count; idx++)
			this->_allocator.construct(middle + idx, value);
		_delete_data(_begin, _storage_end);
		_begin = _start;
		_storage_end = _begin + __capacity;
		return middle;
	}

	template<class T, class allocator>
	typename vector<T, allocator>::iterator vector<T, allocator>::insert(const_iterator position, const T &value) {
		return insert(position, 1, value);
	}

	template<class T, class allocator>
	typename vector<T, allocator>::iterator vector<T, allocator>::insert(const_iterator position, value_type &&value) {
		size_type __capacity = _recommend(std::max(size() + 1, static_cast<size_type>(position - _begin + 1)));
		iterator _start = this->_allocator.allocate(__capacity);
		iterator middle = _copy_data(_begin, std::min(_end, position), _start);
		if (_end < position) {
			middle += static_cast<size_type>(position - _end);
			_end = middle + 1;
		} else {
			_end = _copy_data(position, _end, middle + 1);
		}
		this->_allocator.construct(middle, std::move(value));
		_delete_data(_begin, _storage_end);
		_begin = _start;
		_storage_end = _begin + __capacity;
		return middle;
	}

	template<class T, class allocator>
	typename vector<T, allocator>::iterator vector<T, allocator>::insert(const_iterator position,
	                                                                     std::initializer_list<value_type> il) {
		_NOT_IMPLEMENTED_;
		return nullptr;
	}
	template<class T, class Allocator>
	template<class InputIterator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator position,
	                                                                     InputIterator first,
	                                                                     InputIterator last) {

		std::cout << typeid(first).name() << std::endl;
		std::cout << *first << " " << *(last - 1) << std::endl; // Y it is not compiling
		return nullptr;

	}
	template<class T, class allocator>
	void vector<T, allocator>::pop_back() {
		--_end;
		this->_allocator.destroy(end());
	}

	template<class T, class allocator>
	void vector<T, allocator>::pop_front() {
		size_type __capacity = capacity();
		iterator _start = this->_allocator.allocate(__capacity, _begin);
		_end = _copy_data(begin() + 1, end(), _start);
		_begin = _start;
		_storage_end = _begin + __capacity;
	}
	template<class T, class allocator>
	void vector<T, allocator>::push_back(const_reference value) {
		if (size() == capacity()) {
			size_type __capacity = _recommend(size() + 1);
			iterator _start = this->_allocator.allocate(__capacity, _begin);

			_end = _copy_data(begin(), end(), _start);

			_delete_data(begin(), _storage_end);
			_begin = _start;
			_storage_end = _begin + __capacity;
		}
		this->_allocator.construct(_end++, value);

	}
	template<class T, class allocator>
	void vector<T, allocator>::push_back(value_type &&value) {
		if (size() == capacity()) {
			size_type __capacity = _recommend(size() + 1);
			iterator _start = this->_allocator.allocate(__capacity, _begin);

			_end = _copy_data(begin(), end(), _start);

			_delete_data(begin(), _storage_end);
			_begin = _start;
			_storage_end = _begin + __capacity;
		}
		this->_allocator.construct(_end++, std::move(value));
	}

	template<class T, class allocator>
	void vector<T, allocator>::push_front(const_reference value) {
		size_type __capacity = (size() == capacity()) ? _recommend(size() + 1) : size();

		iterator _start = this->_allocator.allocate(__capacity, _begin);
		this->_allocator.construct(_start, value);

		_end = _copy_data(begin(), end(), _start + 1);
		_delete_data(begin(), _storage_end);

		_begin = _start;
		_storage_end = _begin + __capacity;
	}

	template<class T, class allocator>
	void vector<T, allocator>::push_front(value_type &&value) {
		size_type __capacity = (size() == capacity()) ? _recommend(size() + 1) : size();

		iterator _start = this->_allocator.allocate(__capacity, _begin);
		this->_allocator.construct(_start, std::move(value));

		_end = _copy_data(begin(), end(), _start + 1);
		_delete_data(begin(), _storage_end);

		_begin = _start;
		_storage_end = _begin + __capacity;
	}
	template<class T, class allocator>
	void vector<T, allocator>::resize(size_type sz) {
		if (sz == 0)
			clear();

		reserve(sz);
	}

	template<class T, class allocator>
	void vector<T, allocator>::resize(size_type sz, const T &value) {
		size_type __capacity = capacity();
		resize(sz);
		if (sz > __capacity) {
			_end = _storage_end;
			this->fill(begin() + __capacity, _storage_end, value);
		}
	}
	template<class T, class allocator>
	void vector<T, allocator>::reserve(size_type n) {
		size_type __capacity = capacity();
		iterator _start = this->_allocator.allocate(n);
		_end = _copy_data(begin(), begin() + std::min(n, __capacity), _start);
		_delete_data(_begin, _begin + __capacity);
		_begin = _start;
		_storage_end = _begin + n;
	}
	template<class T, class Allocator>
	vector<typename vector<T, Allocator>::value_type,
	       typename vector<T, Allocator>::allocator_type> &vector<T, Allocator>::reversed() {
		// TODO: IS WITH THE ONLY WAY? //		_NOT_IMPLEMENTED_;
		vector<value_type, allocator_type> *other = new vector<value_type, allocator_type>(*this);
		other->reverse();
		return *other;
	}
	template<class T, class Allocator>
	vector<typename vector<T, Allocator>::value_type,
	       typename vector<T, Allocator>::allocator_type> &vector<T, Allocator>::reversed(iterator first,
	                                                                                      iterator last) {
		// TODO:  IS WITH THE ONLY WAY? //		_NOT_IMPLEMENTED_;
		vector<value_type, allocator_type> *other = new vector<value_type, allocator_type>(*this);
		other->reverse(other->begin() + static_cast<difference_type >(first - _begin ),
		               other->begin() + static_cast<difference_type >(last - _begin));
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
		for (reference element: *this) {
			//			try {
			output += to_string(element) + " ";
			//			}
			//			catch(std::exception e) {}
		}
		output.pop_back();
		return output;
	}
}

#endif //COLLECTIONS_VECTOR_HPP

