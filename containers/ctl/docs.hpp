/*
namespace ctl {

	template<class T, class Allocator = std::allocator<T>>
	class vector {
	public: // typedefs
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
	private: // properties
		iterator _begin, _end, _storage_end;
	public: // constructors / destructors
		explicit vector(const Allocator &alloc = Allocator());

		explicit vector(size_type count, const Allocator &alloc = Allocator());
		explicit vector(size_type count, const T &value, const Allocator &alloc = Allocator());

		template<class Iterator>
		explicit vector(Iterator begin, Iterator end, const Allocator &alloc = Allocator());

		vector(const vector<value_type, allocator_type> &other);

		vector(const vector<value_type, allocator_type> &other, const Allocator &alloc);

		vector(vector &&other) noexcept;

		template<class Container>
		vector(const Container &__container);

		vector(std::initializer_list<value_type> il, const Allocator &alloc = Allocator());

		virtual ~vector();
	public: // operators
		vector &operator=(const vector &other);
		vector &operator=(vector &&other) noexcept;

		reference operator[](size_type i);
		const_reference operator[](size_type i) const;

		explicit operator std::string() const noexcept;
	public:
		template<typename R>
		R accumulate(R initial, std::function<R(R, const_reference)> next_result);
		allocator_type allocator() const noexcept;
		void append(element_accessible_modifiable<value_type, iterator> &other);
		void append(const_reference value);
		void append(value_type &&value);
		reference at(size_type i);
		const_reference at(size_type i) const;

		reference back();
		const_reference back() const;
		iterator begin() noexcept;
		const_iterator begin() const noexcept;
		size_type capacity() const noexcept;
		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;
		void clear() noexcept;
		bool contains(const_reference item) const;
		bool contains(conformer predicate) const;
		size_type count(const_reference item) const noexcept;
		size_type count(conformer predicate) const noexcept;

		const_reverse_iterator crbegin() const noexcept;
		const_reverse_iterator crend() const noexcept;

		bool empty() const noexcept;
		iterator end() noexcept;
		const_iterator end() const noexcept;
		vector<std::pair<size_type, iterator>, allocator_type> enumerated() noexcept;
		iterator erase(const_iterator position);
		iterator erase(size_type position);
		iterator erase(const_iterator first, const_iterator last);
		iterator erase(size_type first, size_type last);

		void fill(size_type first, size_type last, const T &value);
		void fill(iterator first, iterator last, const T &value);
		void fill(const T &value, size_type size);
		void fill(const T &value);
		void filter(conformer predicate);
		vector<value_type, allocator_type> filtered(conformer predicate);
		iterator first(conformer predicate) const noexcept;
		iterator first(const_reference item) const;
		void for_each(action act) noexcept;
		reference front();
		const_reference front() const;

		size_type index_of(const_reference value, size_type first, size_type last) const;
		iterator insert(const_iterator position, const T &value);
		iterator insert(size_type idx, const T &value);
		iterator insert(const_iterator position, value_type &&value);
		iterator insert(size_type idx, value_type &&value);
		iterator insert(const_iterator position, size_type count, const T &value);
		iterator insert(size_type idx, size_type count, const T &value);
		iterator insert(const_iterator position, std::initializer_list<value_type> il);
		iterator insert(size_type idx, std::initializer_list<value_type> il);
		template<class Iterator>
		iterator insert(const_iterator position, Iterator first, Iterator last);
		template<class Iterator>
		iterator insert(size_type idx, Iterator first, Iterator last);
		size_type iterator_of(const_reference value, iterator first, iterator last) const;

		template<typename U>
		vector<U, allocator_type> map(std::function<U(reference)> mapper);
		iterator max() const;
		iterator max(comparer comp) const;
		size_type max_size() const noexcept;
		iterator min() const;
		iterator min(comparer comp) const;

		void pop_back();
		void pop_back(size_type count);
		void pop_front();
		void pop_front(size_type count);
		vector<value_type, allocator_type> prefix(size_type max_length);
		vector<value_type, allocator_type> prefix(conformer predicate);
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
		void remove_all(conformer predicate);
		void remove_at(int idx);
		void remove_first();
		void remove_last();
		reverse_iterator rend() noexcept;
		const_reverse_iterator rend() const noexcept;
		void reserve(size_type n);
		void resize(size_type count);
		void resize(size_type count, const value_type &value);
		void reverse();
		void reverse(iterator first, iterator last);
		void reverse(size_type first, size_type last);

		void shrink_to_fit() noexcept;
		size_type size() const noexcept;
		vector<value_type, allocator_type> subsequence(iterator from, iterator to);
		vector<value_type, allocator_type> subsequence(size_type from, size_type to);
		vector<value_type, allocator_type> suffix(size_type max_length);
		vector<value_type, allocator_type> suffix(conformer predicate);
		void swap(vector<value_type, allocator_type> &other);

		std::vector<value_type> to_std_vector() const;
		std::list<value_type> to_std_list() const;
		std::set<value_type> to_std_set() const;
		bool true_for_all(conformer conform) const;
	};

	//! Non member functions

	template<class T, class Allocator, class Container>
	bool operator==(const vector<T, Allocator> &lhs, const Container &rhs);

	template<class T, class Allocator, class Container>
	bool operator!=(const vector<T, Allocator> &lhs, const Container &rhs);
}
*/