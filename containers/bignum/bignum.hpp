//
// Created by Artem Bobrov on 09/11/2019.
//

#ifndef INC_2019_HA2_BIGNUM_ARTBOBROV_BIGNUM_HPP
#define INC_2019_HA2_BIGNUM_ARTBOBROV_BIGNUM_HPP

#include <string>
#include <ostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>

namespace mp {
    namespace detail {
        inline std::string to_binary(const std::vector<std::uint32_t> &container) {
            std::string string;
            for (size_t offset = 0; offset < container.size(); ++offset) {
                auto digit = container[offset];
                std::string binary_digit;
                for (size_t bit_offset = 0; digit || (bit_offset < 32 && offset + 1 < container.size()); ++bit_offset) {
                    binary_digit.push_back(digit % 2 + '0');
                    digit /= 2;
                }
                string.append(binary_digit);
            }
            return string;
        }

        inline std::string from_binary_to_decimal(const std::string &string) {
            std::string result = "0";

            for (size_t offset = string.size(); offset > 0; --offset) {
                std::uint8_t carry = 0;

                for (auto &digit: result) {
                    std::uint8_t current = digit - '0';
                    current *= 2;
                    current += carry;
                    carry = current / 10;
                    current %= 10;
                    digit = current + '0';
                }

                if (carry)
                    result.push_back(carry + '0');


                carry = string[offset - 1] - '0';

                for (size_t index = 0; carry && (index < result.size()); ++index) {
                    std::uint8_t current = result[index] - '0' + carry;
                    carry = current / 10;
                    current %= 10;
                    result[index] = current + '0';
                }

                if (carry)
                    result.push_back(carry + '0');
            }
            return result;
        }

        inline std::uint32_t to_uint32_t(std::string::const_iterator begin, std::string::const_iterator end) {
            std::stringstream stream({begin, end});
            std::uint32_t number = 0;
            stream >> number;
            return number;
        }

        inline bool is_digit(char character) {
            return isdigit(character);
        }
    }

    class bignum {
        using word = std::uint32_t;
        using word_provider = uint64_t;
        using container = std::vector<word>;

        static constexpr uint64_t BASE = static_cast<uint64_t>(1) << (sizeof(word) * 8);
    public:
        bignum(word number = 0) : _container({number}) {}

        explicit bignum(std::string number);

        bignum &operator=(bignum other) {
            swap(other);
            return *this;
        }

        explicit operator std::uint32_t() const;

        explicit operator bool() const;

        bignum &operator+=(const bignum &rhs);

        bignum &operator*=(const bignum &rhs);

        void swap(bignum &other) {
            other._container.swap(_container);
        }

        [[nodiscard]] std::string to_string() const;

    private:
        static bignum from_decimal(const std::string &number);

        void add(word_provider value, size_t position = 0);

        container _container;
    };


    class polynomial {
        using coefficient_t = std::uint32_t;
        using container = std::vector<coefficient_t>;
    public:
        explicit polynomial(const std::string &string);

        coefficient_t &at(size_t index);

        coefficient_t at(size_t index) const;

        template<typename T>
        T operator()(const T &point) const;

    private:
        container _container;
    };

    inline std::istream &operator>>(std::istream &os, bignum &number) {
        std::string content;
        os >> content;
        number = bignum(content);
        return os;
    }

    inline std::ostream &operator<<(std::ostream &os, const bignum &bignum) {
        os << bignum.to_string();
        return os;
    }

    inline bignum operator+(bignum lhs, const bignum &rhs);

    inline bignum operator*(bignum lhs, const bignum &rhs);
}

inline void mp::bignum::add(word_provider value, size_t position) {
    word_provider provider = value;
    while (provider) {
        while (_container.size() <= position)
            _container.push_back(0);

        auto digit = _container[position];
        word_provider carry = provider % BASE;
        provider /= BASE;
        carry += digit;
        _container[position] = carry % BASE;
        provider += carry / BASE;
        ++position;
    }
}

inline mp::bignum mp::bignum::from_decimal(const std::string &number) {
    mp::bignum result = 0;
    for (const auto character: number) {
        auto digit = character - '0';
        result *= 10;
        result += digit;
    }
    return result;
}

inline mp::bignum::bignum(std::string number) : bignum(mp::bignum::from_decimal(number)) {}

inline mp::bignum &mp::bignum::operator+=(const mp::bignum &rhs) {
    for (size_t offset = 0; offset < rhs._container.size(); ++offset) {
        auto digit = rhs._container[offset];
        add(digit, offset);
    }
    return *this;
}

inline mp::bignum &mp::bignum::operator*=(const mp::bignum &rhs) {
    bignum number;
    for (size_t rhs_offset = 0; rhs_offset < rhs._container.size(); ++rhs_offset) {
        for (size_t lhs_offset = 0; lhs_offset < _container.size(); ++lhs_offset) {
            word_provider digit = rhs._container[rhs_offset];
            digit *= _container[lhs_offset];

            number.add(digit, rhs_offset + lhs_offset);
        }
    }

    swap(number);
    return *this;
}


inline std::string mp::bignum::to_string() const {
    auto binary = detail::to_binary(_container);
    auto decimal = detail::from_binary_to_decimal(binary);
    std::reverse(decimal.begin(), decimal.end());
    return decimal;
}

inline mp::bignum::operator std::uint32_t() const {
    return _container[0];
}

inline mp::bignum::operator bool() const {
    return _container.size() > 1 || _container[0];
}

inline mp::bignum mp::operator+(mp::bignum lhs, const mp::bignum &rhs) {
    lhs += rhs;
    return lhs;
}

inline mp::bignum mp::operator*(mp::bignum lhs, const mp::bignum &rhs) {
    lhs *= rhs;
    return lhs;
}

inline mp::polynomial::polynomial(const std::string &string) {
    for (auto it = string.begin(); it != string.end();) {
        auto coefficient_end_it = std::find_if_not(it, string.end(), detail::is_digit);
        coefficient_t coefficient = detail::to_uint32_t(it, coefficient_end_it);
        it = std::find_if(coefficient_end_it, string.end(), detail::is_digit);
        auto power_end_it = std::find_if_not(it, string.end(), detail::is_digit);
        coefficient_t power = detail::to_uint32_t(it, power_end_it);

        if (_container.size() <= power) {
            _container.resize(power + 1);
        }

        _container[power] = coefficient;

        it = std::find_if(power_end_it, string.end(), detail::is_digit);;
    }
}

inline mp::polynomial::coefficient_t &mp::polynomial::at(size_t index) {
    if (index >= _container.size()) {
        _container.resize(index + 1);
    }
    return _container.at(index);
}

inline mp::polynomial::coefficient_t mp::polynomial::at(size_t index) const {
    if (index >= _container.size())
        return 0;

    return _container[index];
}

template<typename T>
T mp::polynomial::operator()(const T &point) const {
    T result = 0;
    for (auto it = _container.rbegin(); it != _container.rend(); ++it) {
        result *= point;
        result += *it;
    }
    return result;
}


#endif //INC_2019_HA2_BIGNUM_ARTBOBROV_BIGNUM_HPP
