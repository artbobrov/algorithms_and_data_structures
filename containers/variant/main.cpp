#include <cassert>
#include <stdexcept>
#include <iostream>

#include "variant.hpp"

using namespace util;
struct helper{
    int tag;
    int * destructor_cnt;
    int * copy_cnt;
    bool throw_on_copy;
    helper(int tag, int * destructor_cnt, int * copy_cnt, bool throw_on_copy = false)
            : tag(tag)
            , destructor_cnt(destructor_cnt)
            , copy_cnt(copy_cnt)
            , throw_on_copy(throw_on_copy)
    {}

    helper(const helper& other)
            : tag(other.tag)
            , destructor_cnt(other.destructor_cnt)
            , copy_cnt(other.copy_cnt)
            , throw_on_copy(other.throw_on_copy)
    {
        (*copy_cnt)++;
        if (throw_on_copy)
            throw std::bad_alloc();
    }

    helper(helper&& other) = default;
    helper & operator=(helper const & other) = default;
    helper & operator=(helper && other) = default;

    virtual ~helper() { (*destructor_cnt)++; }
};
struct trap_t {
    int _value;
    explicit trap_t(int value) : _value(value) {}

    trap_t(trap_t &&) noexcept = default;
    trap_t &operator=(trap_t &&) noexcept = default;

    trap_t(const trap_t &o) {
        _value = o._value;
        throw std::runtime_error("it's a trap");
    }

    trap_t &operator=(const trap_t &o) {
        _value = o._value;
        throw std::runtime_error("it's a trap");
    }

    friend std::ostream &operator<<(std::ostream &os, const trap_t &trap) {
        os << "_value: " << trap._value;
        return os;
    }
};

void test_1() {
    variant<int, trap_t> variant;
    variant = 42;

    assert(get<int>(variant) == 42);
    trap_t trap{24};
    try {
        variant = trap;
        assert(false);
    } catch (...) {
        assert(get<int>(variant) == 42);
    }
}

void test_2() {
    variant<int, trap_t> var;
    var = 42;
    assert(get<int>(var) == 42);

    variant<int, trap_t> other = trap_t{24};
    try {
        var = other;
        assert(false);
    } catch (...) {
        assert(get<int>(var) == 42);
    }
}

void test_3() {
    int destructor_count = 0;
    int copy_cnt = 0;
    {
        variant<helper> a(helper(5, &destructor_count, &copy_cnt));
        variant<helper> b(a);
        assert(get<helper>(a).tag == 5);
        assert(get<helper>(b).tag == 5);
        assert(copy_cnt > 0);
    }
    assert(destructor_count == 3);
}

struct visitor_t {
    template<class A, class B>
    void operator()(A lhs, B rhs) {
        std::cout << "A: " << lhs << std::endl;
        std::cout << "B: " << rhs << std::endl;
    }

    void operator()() {
    }
};

int main() {
    test_1();
    test_2();
    test_3();
    return 0;
}