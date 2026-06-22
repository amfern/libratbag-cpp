#include "ratbag/lib/utils/bitmask_operators.hpp"

#include "gtest/gtest.h"

enum class A { x = 1, y = 2 };
constexpr bool enable_bitmask_operators(A);

enum class B : unsigned long { x = 0x80000000, y = 0x40000000 };
constexpr bool enable_bitmask_operators(B);


TEST(LedBitmaskOperators, All) {
    A a1=A::x | A::y;
    A a2=a1&A::y; // Turn on A::y only
    ASSERT_EQ(A::y, a2);
    a2^=A::x; // toggle A::x
    ASSERT_EQ(A::x | A::y, a2);
    A a3=a1&~A::x; // Turn off A::X
    ASSERT_EQ(A::y, a3);

    B b1=B::x | B::y;
    B b2=b1&B::y;
    ASSERT_EQ(B::y, b2);
    b2^=B::x;
    ASSERT_EQ(B::x | B::y, b2);
    B b3=b1&~B::x;
    ASSERT_EQ(B::y, b3);
}

