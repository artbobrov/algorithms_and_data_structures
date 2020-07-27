#ifndef VARIANT_VARIANT_HPP
#define VARIANT_VARIANT_HPP

#include <stdexcept>
#include <type_traits>
#include <limits>
#include <algorithm>

namespace util::detail {
    using index_t = size_t;
}

namespace util {
    inline constexpr detail::index_t variant_npos = std::numeric_limits<decltype(variant_npos)>::max();
}

namespace util::detail {
    template<typename ...Ts>
    constexpr size_t max_size = std::max({sizeof(Ts)...});
    template<typename ...Ts>
    constexpr size_t max_align = std::max({alignof(Ts)...});

    template<index_t I, typename U>
    constexpr index_t find_same_type_index() {
        return variant_npos;
    }

    template<index_t I, typename U, typename T, typename ...Ts>
    constexpr index_t find_same_type_index() {
        if constexpr (std::is_same_v<U, T>) {
            return I;
        } else {
            return find_same_type_index<I + 1, U, Ts...>();
        }
    }

    template<typename U, typename ...Ts>
    constexpr index_t index_of_same() {
        return find_same_type_index<0, U, Ts...>();
    }

    template<typename U, typename ...Ts>
    constexpr bool contains_same() {
        return index_of_same<U, Ts...>() != variant_npos;
    }

    template<index_t I, typename U>
    constexpr index_t find_convertible_index() {
        return variant_npos;
    }

    template<index_t I, typename U, typename T, typename ...Ts>
    constexpr index_t find_convertible_index() {
        if constexpr (std::is_convertible_v<U, T>) {
            return I;
        } else {
            return find_convertible_index<I + 1, U, Ts...>();
        }
    }

    template<typename U, typename ...Ts>
    constexpr index_t index_of_convertible() {
        return find_convertible_index<0, U, Ts...>();
    }

    template<typename U, typename ...Ts>
    constexpr bool contains_convertible() {
        return index_of_convertible<U, Ts...>() != variant_npos;
    }

    template<index_t I, typename  ...Ts>
    using type_at = std::tuple_element_t<I, std::tuple<Ts...>>;

    template<typename ...Ts>
    inline constexpr size_t variadic_count = sizeof...(Ts);

    template<typename U, typename ...Ts>
    constexpr index_t index_of_init() {
        if constexpr (index_of_same<U, Ts...>() != variant_npos) {
            return index_of_same<U, Ts...>();
        }
        return index_of_convertible<U, Ts...>();
    }

    template<typename U, typename ...Ts>
    constexpr bool contains_init() {
        return contains_same<U, Ts...>() || contains_convertible<U, Ts...>();
    }

    template<typename U, typename ...Ts>
    using selected_type = detail::type_at<detail::index_of_init<U, Ts...>(), Ts...>;
}

namespace util::detail::visitors {
    struct clear_visitor {
        template<typename T>
        std::enable_if_t<!std::is_trivially_destructible_v<T>> operator()(const T &object) {
            object.~T();
        }

        template<typename T>
        std::enable_if_t<std::is_trivially_destructible_v<T>> operator()(const T &) {
        }

        void operator()() {}
    };

    template<typename ...Ts>
    class copy_visitor;

    template<typename ...Ts>
    class move_visitor;
}

namespace util {
    struct bad_get : std::runtime_error {
        using runtime_error::runtime_error;
    };

    template<typename Visitor, typename Variant>
    void apply_visitor(Visitor &&visitor, Variant &&variant);

    template<typename ...Types>
    struct variant final {
        template<typename ...Ts>
        friend
        class detail::visitors::copy_visitor;

        template<typename ...Ts>
        friend
        class detail::visitors::move_visitor;

        using index_t = detail::index_t;
        using storage_t = std::aligned_storage_t<detail::max_size<Types...>, detail::max_align<Types...>>;
        static constexpr size_t arg_count = detail::variadic_count<Types...>;

        variant() : _type_index(variant_npos) {}

        variant(const variant &var);

        variant(variant &&var) noexcept(((std::is_nothrow_move_constructible_v<Types> &&
                                          std::is_nothrow_move_assignable_v<Types>) && ...));

        template<class U, typename = std::enable_if_t<detail::contains_init<U, Types...>()>, typename R = detail::selected_type<U, Types...>>
        variant(U &&value) noexcept(std::is_nothrow_assignable_v<R &, U> && std::is_nothrow_constructible_v<R, U>)
                : _type_index(detail::index_of_init<U, Types...>()) {
            new(&_storage) R(std::forward<U>(value));
        }

        ~variant() {
            clear();
        }

        variant &operator=(const variant &var) {
            if (this != &var) {
                variant other{var};
                other.swap(*this);
            }
            return *this;
        }


        variant &operator=(variant &&var) noexcept(((std::is_nothrow_move_constructible_v<Types> &&
                                                     std::is_nothrow_move_assignable_v<Types>) && ...)) {
            if (this != &var) {
                clear();
                apply_visitor(detail::visitors::move_visitor(std::move(var), *this), var);
                _type_index = var._type_index;
            }
            return *this;
        }

        template<class U, typename = std::enable_if_t<detail::contains_init<U, Types...>()>, typename R = detail::selected_type<U, Types...>>
        variant &
        operator=(U &&value) noexcept(std::is_nothrow_assignable_v<R &, U> && std::is_nothrow_constructible_v<R, U>) {
            variant var(std::forward<U>(value));
            clear();
            swap(var);
            return *this;
        }


        void swap(variant &other) noexcept(((std::is_nothrow_move_constructible_v<Types> &&
                                             std::is_nothrow_move_assignable_v<Types>) && ...));

        [[nodiscard]] bool empty() const { return index() == variant_npos; }

        [[nodiscard]] index_t index() const { return _type_index; }

        void clear() {
            apply_visitor(detail::visitors::clear_visitor(), *this);
            _type_index = variant_npos;
        }

        template<typename U, typename ...Ts>
        friend const U *get(const variant<Ts...> *var);

    private:
        index_t _type_index = variant_npos;
        storage_t _storage;
    };

    template<typename U, typename ...Ts>
    const U *get(const variant<Ts...> *var) {
        if (!var || var->empty())
            return nullptr;
        constexpr detail::index_t type_index = detail::index_of_same<std::decay_t<U>, Ts...>();

        if (type_index != var->index())
            return nullptr;
        const U *value = reinterpret_cast<const U *>(&var->_storage);
        return value;
    }

    template<typename U, typename ...Ts>
    U *get(variant<Ts...> *var) {
        return const_cast<U *>(get<U>(const_cast<const variant<Ts...> *>(var)));
    }

    template<typename U, typename ...Ts>
    const U &get(const variant<Ts...> &var) {
        const U *value = get<U>(&var);
        if (!value)
            throw bad_get("");
        return *value;
    }

    template<typename U, typename ...Ts>
    U &get(variant<Ts...> &var) {
        U *value = get<U>(&var);
        if (!value)
            throw bad_get("");
        return *value;
    }

    template<typename U, typename ...Ts>
    U &&get(variant<Ts...> &&var) {
        U *value = get<U>(&var);
        if (!value)
            throw bad_get("");
        return std::move(*value);
    }

    template<detail::index_t I, typename ...Ts>
    const detail::type_at<I, Ts...> *get(const variant<Ts...> *var) {
        using U = detail::type_at<I, Ts...>;
        return get<const U>(var);
    }

    template<detail::index_t I, typename ...Ts>
    detail::type_at<I, Ts...> *get(variant<Ts...> *var) {
        using U = detail::type_at<I, Ts...>;
        return get<U>(var);
    }

    template<detail::index_t I, typename ...Ts>
    const detail::type_at<I, Ts...> &get(const variant<Ts...> &var) {
        using U = detail::type_at<I, Ts...>;
        return get<U>(var);
    }


    template<detail::index_t I, typename ...Ts>
    detail::type_at<I, Ts...> &get(variant<Ts...> &var) {
        using U = detail::type_at<I, Ts...>;
        return get<U>(var);
    }


    template<detail::index_t I, typename ...Ts>
    detail::type_at<I, Ts...> &&get(variant<Ts...> &&var) {
        using U = detail::type_at<I, Ts...>;
        return get<U>(std::move(var));
    }

    template<typename ...Ts>
    void swap(variant<Ts...> &lhs, variant<Ts...> &rhs) {
        lhs.swap(rhs);
    }
}

namespace util::detail {
    template<typename Visitor, typename Variant, size_t... Indexes>
    void apply_visitor_helper(Visitor &&visitor, Variant &&variant, std::index_sequence<Indexes...> &&) {
        ((get<Indexes>(&variant) ? visitor(get<Indexes>(std::forward<Variant>(variant))) : void()), ...);
    }
}

namespace util {
    template<typename Visitor, typename Variant>
    void apply_visitor(Visitor &&visitor, Variant &&variant) {
        using variant_t = std::decay_t<Variant>;
        if (variant.empty()) {
            visitor();
        } else {
            detail::apply_visitor_helper(
                    std::forward<Visitor>(visitor),
                    std::forward<Variant>(variant),
                    std::make_index_sequence<variant_t::arg_count>{}
            );
        }
    }
}

namespace util::detail::visitors {
    template<typename ...Ts>
    class copy_visitor {
        using variant_t = variant<Ts...>;
    public:
        copy_visitor(const variant_t &source, variant_t &destination) : source(source), destination(destination) {}

        template<class T>
        void operator()(const T &source_object) {
            new(&destination._storage) T(source_object);
        }

        void operator()() {}

    private:
        const variant_t &source;
        variant_t &destination;
    };

    template<typename ...Ts>
    class move_visitor {
        using variant_t = variant<Ts...>;
    public:
        move_visitor(variant_t &&source, variant_t &destination) : source(std::move(source)),
                                                                   destination(destination) {}

        template<class T>
        void operator()(T &&source_object) {
            using U = std::decay_t<T>;
            new(&destination._storage) U(std::move(source_object));
        }

        void operator()() {}

    private:
        variant_t &&source;
        variant_t &destination;
    };

    template<typename ...Ts>
    class swap_lhs_visitor {
        using variant_t = variant<Ts...>;

        template<typename V>
        struct swap_rhs_visitor {
            swap_rhs_visitor(V &rhsValue) : _lhs_value(rhsValue) {}

            template<typename T>
            void operator()(T &rhs_object) {
                if constexpr (std::is_same_v<V, T>) {
                    std::swap(_lhs_value, rhs_object);
                }
            }

            void operator()() {}

        private:
            V &_lhs_value;
        };

    public:
        swap_lhs_visitor(variant_t &lhs, variant_t &rhs) : lhs(lhs), rhs(rhs) {}

        template<class T>
        void operator()(T &lhs_object) {
            swap_rhs_visitor<T> visitor(lhs_object);
            apply_visitor(visitor, rhs);
        }

        void operator()() {}

    private:
        variant_t &lhs;
        variant_t &rhs;
    };
}

namespace util {
    template<typename ...Ts>
    variant<Ts...>::variant(const variant &var): _type_index(var._type_index) {
        apply_visitor(detail::visitors::copy_visitor(var, *this), var);
    }

    template<typename ...Types>
    variant<Types...>::variant(variant &&var) noexcept(((std::is_nothrow_move_constructible_v<Types> &&
                                                         std::is_nothrow_move_assignable_v<Types>) && ...))
            : _type_index(var._type_index) {
        apply_visitor(detail::visitors::move_visitor(std::move(var), *this), var);
        var._type_index = variant_npos;
    }

    template<typename ...Types>
    void variant<Types...>::swap(variant &other) noexcept(((std::is_nothrow_move_constructible_v<Types> &&
                                                            std::is_nothrow_move_assignable_v<Types>) && ...)) {
        if (empty() && other.empty())
            return;
        if (_type_index == other._type_index) {
            detail::visitors::swap_lhs_visitor visitor(*this, other);
            apply_visitor(visitor, *this);
        } else {
            std::swap(*this, other);
        }
    }
}


#endif //VARIANT_VARIANT_HPP
