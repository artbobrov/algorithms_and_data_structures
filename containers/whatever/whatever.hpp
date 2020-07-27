#pragma once

#include <typeinfo>
#include <string>
#include <stdexcept>

namespace utils {
    struct bad_whatever_cast : std::runtime_error {
        explicit bad_whatever_cast(std::string string) : runtime_error(std::move(string)) {}
    };

    class whatever final {
        struct base_holder {
            virtual ~base_holder() = default;

            [[nodiscard]] virtual base_holder *copy() const = 0;

            [[nodiscard]] virtual const std::type_info &type_info() const = 0;
        };

        template<class T>
        struct object_holder : base_holder {
            explicit object_holder(const T &object) : object(object) {}

            ~object_holder() override = default;

            [[nodiscard]] const std::type_info &type_info() const override {
                return typeid(T);
            }

            [[nodiscard]] base_holder *copy() const override {
                return new object_holder(object);
            }

            T object;
        };

    public:

        template<class T>
        whatever(const T &value): _holder(new object_holder(value)) {}

        explicit whatever() = default;

        ~whatever() {
            delete _holder;
        }

        whatever(const whatever &other) : _holder(other.copy()) {}

        whatever &operator=(whatever other) {
            swap(other);
            return *this;
        }

        void swap(whatever &other) {
            using std::swap;
            swap(_holder, other._holder);
        }

        void clear() {
            delete _holder;
            _holder = nullptr;
        }

        [[nodiscard]] bool empty() const {
            return !_holder;
        }

        template<typename T>
        friend const T *whatever_cast(const whatever *any);

    private:
        base_holder *copy() const {
            if (_holder) {
                return _holder->copy();
            }
            return nullptr;
        }

        [[nodiscard]] const std::type_info &type_info() const {
            if (_holder) {
                return _holder->type_info();
            }
            return typeid(void);
        }


        base_holder *_holder = nullptr;
    };

    template<typename T>
    const T *whatever_cast(const whatever *any) {
        if (!any) {
            return nullptr;
        }

        auto const &type = any->type_info();
        if (type != typeid(T)) {
            return nullptr;
        }
        auto holder = static_cast<whatever::object_holder<T> *> (any->_holder);
        return &holder->object;
    }

    template<typename T>
    T *whatever_cast(whatever *any) {
        return const_cast<T *>(whatever_cast<const T>(const_cast<const whatever *>(any)));
    }

    template<typename T>
    T whatever_cast(whatever &any) {
        using R = std::decay_t<T>;

        auto object = whatever_cast<R>(&any);
        if (!object) {
            throw bad_whatever_cast(std::string("Invalid cast type ") + typeid(T).name());
        }
        return *object;
    }

    template<typename T>
    T whatever_cast(const whatever &any) {
        using R = std::decay_t<T>;

        auto object = whatever_cast<R>(&any);
        if (!object) {
            throw bad_whatever_cast(std::string("Invalid cast type ") + typeid(T).name());
        }
        return *object;
    }


    inline void swap(whatever &lhs, whatever &rhs) {
        lhs.swap(rhs);
    }
}