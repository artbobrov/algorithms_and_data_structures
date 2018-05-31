#include <iostream>

#include <iterator>
#include <vector>

template<class T>
class AbstractIterator: public std::iterator<std::forward_iterator_tag, T> {
public:
	virtual void next() = 0;
	virtual void first() = 0;
	virtual bool isDone() = 0;
	virtual T &object() = 0;

	virtual T &operator*() = 0;
	inline T *operator->() { return &(operator*()); }
};
template<class T>
class Iterator: public AbstractIterator<T> {
public:
	explicit Iterator(std::vector<T> data): data(std::move(data)), _index(0) {}

	void next() override {
		++_index;
	}
	void first() override {
		_index = 0;
	}
	bool isDone() override {
		return data.size() == 0 || _index == data.size() - 1;
	}
	T &object() override {
		return data[_index];
	}
	T &operator*() override {
		return this->object();
	}

private:
	std::vector<T> data;
	size_t _index;
};

int main() {
	std::vector<int> data = {1, 2, 4, 6, 7};
	AbstractIterator<int> *iterator = new Iterator<int>(data);
	iterator->next();
	iterator->next();
	std::cout << "Value: " << **iterator << std::endl;
	delete iterator;
	return 0;
}