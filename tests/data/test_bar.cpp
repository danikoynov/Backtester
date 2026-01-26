#include <gtest/gtest.h>
#include "data/bar.hpp"
#include <chrono>   // for std::chrono::seconds
#include <cstdint>

using namespace bt;

static Timestamp t0() {
    return Timestamp{std::chrono::seconds{0}};
}

// A baseline valid bar we can tweak per test
static Bar make_valid_bar() {
    return Bar(t0(), 100.0, 110.0, 95.0, 105.0, 1000);
}

TEST(BarCtor, HighBelowLowThrows) {
    EXPECT_THROW(
        Bar(t0(), 100.0, 90.0, 95.0, 105.0, 1000),  // high < low
        std::invalid_argument
    );
}

TEST(BarCtor, NegativeLowThrows) {
    EXPECT_THROW(
        Bar(t0(), 1.0, 2.0, -0.01, 1.5, 1000),
        std::invalid_argument
    );
}

TEST(BarCtor, NegativeVolumeThrows) {
    EXPECT_THROW(
        Bar(t0(), 100.0, 110.0, 95.0, 105.0, -1),
        std::invalid_argument
    );
}

TEST(BarCtor, OpenBelowLowThrows) {
    EXPECT_THROW(
        Bar(t0(), 90.0, 110.0, 95.0, 105.0, 1000),  // open < low
        std::invalid_argument
    );
}

TEST(BarCtor, OpenAboveHighThrows) {
    EXPECT_THROW(
        Bar(t0(), 120.0, 110.0, 95.0, 105.0, 1000), // open > high
        std::invalid_argument
    );
}

TEST(BarCtor, CloseBelowLowThrows) {
    EXPECT_THROW(
        Bar(t0(), 100.0, 110.0, 95.0, 90.0, 1000),  // close < low
        std::invalid_argument
    );
}

TEST(BarCtor, CloseAboveHighThrows) {
    EXPECT_THROW(
        Bar(t0(), 100.0, 110.0, 95.0, 120.0, 1000), // close > high
        std::invalid_argument
    );
}

// Optional edge-case tests (should be valid if you allow equality)
TEST(BarCtor, BoundaryValuesAreAllowed) {
    EXPECT_NO_THROW(
        Bar(t0(), 95.0, 110.0, 95.0, 110.0, 0) // open==low, close==high, volume==0
    );
}
