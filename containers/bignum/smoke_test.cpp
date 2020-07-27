#include <cassert>

// Include guard should prevent double inclusion.
#include "bignum.hpp"
#include <limits>

void assert_sum(std::string lhs, std::string rhs, std::string result) {
    using mp::bignum;
    {
        bignum l(lhs);
        bignum r(rhs);

        assert((l + r).to_string() == result);
    }
    {
        bignum l(lhs);
        bignum r(rhs);

        l += r;
        assert(l.to_string() == result);
    }

}

void check_constructors() {
    mp::bignum n1;
    assert(n1.to_string() == "0");

    mp::bignum n2(42);
    assert(n2.to_string() == "42");

    mp::bignum n3(0xffffffff);
    assert(n3.to_string() == "4294967295");

    mp::bignum n4(n2);
    assert(n2.to_string() == "42");
    assert(n4.to_string() == "42");

    mp::bignum n5("42");
    assert(n5.to_string() == "42");

    mp::bignum n6("18446744073709551615");
    assert(n6.to_string() == "18446744073709551615");

    mp::bignum n7("100000000000000");
    assert(n7.to_string() == "100000000000000");

    mp::bignum n9("200000000000000100000000000000");
    assert(n9.to_string() == "200000000000000100000000000000");


    assert(mp::bignum("999999999").to_string() == "999999999");
    assert(mp::bignum("1000000000").to_string() == "1000000000");
    assert(mp::bignum("1000000001").to_string() == "1000000001");

}

void check_assignment() {
    mp::bignum n1;
    assert(n1.to_string() == "0");

    n1 = 1;
    assert(n1.to_string() == "1");

    n1 = 0xffffffff;
    assert(n1.to_string() == "4294967295");

    mp::bignum n2;
    assert(n2.to_string() == "0");

    n2 = n1;
    assert(n1.to_string() == "4294967295");
    assert(n2.to_string() == "4294967295");

    mp::bignum n3("18446744073709551615"), n4, n5;
    assert(n3.to_string() == "18446744073709551615");
    assert(n4.to_string() == "0");
    n4 = n3;
    assert(n3.to_string() == "18446744073709551615");
    assert(n4.to_string() == "18446744073709551615");

    n3 = n5;
    assert(n3.to_string() == "0");
    assert(n5.to_string() == "0");

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
    n4 = n4;
#pragma clang diagnostic pop
    assert(n4.to_string() == "18446744073709551615");
}

void check_sum() {
    assert_sum("0", "0", "0");
    assert_sum("1", "1", "2");
    assert_sum("134234", "23423423432", "23423557666");
    assert_sum("200000000000000100000000000000", "0", "200000000000000100000000000000");
    assert_sum("0", "200000000000000100000000000000", "200000000000000100000000000000");
    assert_sum("200000000000000100000000000000", "200000000000000100000000000000", "400000000000000200000000000000");
    assert_sum("1", "200000000000000100000000000000", "200000000000000100000000000001");
    assert_sum("200000000000000100000000000000",
               "40000000000000040000000000000010000000000000000000000000000",
               "40000000000000040000000000000210000000000000100000000000000");
    assert_sum("1000000000", "1", "1000000001");

    assert_sum("123593995", "9343804982", "9467398977");
    assert_sum("999999999", "1", "1000000000");
    assert_sum("8371987509543857324075349254305739057432987592435",
               "4325239045790342985234095984390430940945945984359804908453890590825890",
               "4325239045790342985242467971899974798270021333614110647511323578418325");
    assert_sum("99999999999999999999999999999999999999999999",
               "99999999999",
               "100000000000000000000000000000000099999999998");
    assert_sum("99999999999",
               "99999999999999999999999999999999999999999999",
               "100000000000000000000000000000000099999999998");

}

void check_to_int() {
    mp::bignum n1(42);
    assert(std::uint32_t(n1) == 42);

    mp::bignum n2("1311768467284833366");  // 0x1234567890123456
    assert(std::uint32_t(n2) == 0x90123456);
}

void check_bool() {
    assert(!mp::bignum(0));
    assert(mp::bignum(123));
}

void check_io() {
    mp::bignum const n1(1234);

    std::ostringstream ostr;
    ostr << n1;
    assert(ostr.str() == "1234");

    std::istringstream istr1("5678");
    mp::bignum n2(42);
    istr1 >> n2;
    assert(n2.to_string() == "5678");

    // Optional

    std::istringstream istr2("0001234");
    mp::bignum n3;
    istr2 >> n3;
    assert(n3.to_string() == "1234");


    std::istringstream istr3("000000000000000000001234");
    mp::bignum n4;
    istr2 >> n4;
    assert(n3.to_string() == "1234");

//    std::istringstream istr3("+1234");
//    mp::bignum n4;
//    istr3 >> n4;
//    assert(n4.to_string() == "1234");
//
//    std::istringstream istr4("+01234");
//    mp::bignum n5;
//    istr4 >> n5;
//    assert(n5.to_string() == "1234");
}

void check_operators() {
    mp::bignum n1(1234);
    mp::bignum n2(5678);
    assert((n1 + n2).to_string() == std::to_string(1234 + 5678));
    assert((n1 * n2).to_string() == std::to_string(1234 * 5678));

    mp::bignum n3(0xffffffff);
    mp::bignum n4(0xaaaaaaaa);
    assert((n3 + n4).to_string() == "7158278825");
    assert((n3 * n4).to_string() == "12297829376746411350");

    n3 *= 2;
    assert(n3.to_string() == "8589934590");

    n3 += n4;
    assert(n3.to_string() == "11453246120");
}

void check_const() {
    mp::bignum const n1(42);
    mp::bignum n2;
    n2 = n1;
    assert(n1.to_string() == "42");
    assert(n2.to_string() == "42");

    mp::bignum const n3(10);
    assert(uint32_t(n3) == 10);

    assert(n3);

    assert((n1 + n3).to_string() == "52");
    assert((n1 * n3).to_string() == "420");

    n2 *= n3;
    assert(n2.to_string() == "420");
    assert(n3.to_string() == "10");

    n2 += n3;
    assert(n2.to_string() == "430");
    assert(n3.to_string() == "10");
}

void check_polynomial() {
    using mp::polynomial;
    using mp::bignum;
    {
        polynomial polynom("5*x^3+2*x^1+6*x^0");
        assert(polynom.at(0) == 6);
        polynom.at(0) = 0;
        assert(polynom.at(0) == 0);
        assert(polynom.at(99) == 0);
    }
    {
        polynomial polynom("5*x^3+2*x^1+6*x^0");
        assert(polynom(1) == 13);
        assert(polynom(2) == 50);
        assert(polynom(bignum(2)).to_string() == "50");
    }
}

int main() {
    check_constructors();
    check_assignment();
    check_sum();
    check_to_int();
    check_bool();
    check_io();
    check_operators();
    check_const();
    check_polynomial();
}
