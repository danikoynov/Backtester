#include <gtest/gtest.h>
#include <stdexcept>
#include <cstdint>

#include "execution/fill.hpp"
#include "domain/ticker.hpp"

using namespace bt;

namespace {
    Ticker make_ticker() { return Ticker{"AAPL"}; }
}

// ==================== Constructor invariants ====================

TEST(Fill, Constructor_ThrowsIfQuantityNonPositive) {
    Ticker t = make_ticker();

    EXPECT_THROW(Fill(0, 100.0, t), std::invalid_argument);
    EXPECT_THROW(Fill(-5, 100.0, t), std::invalid_argument);
}

TEST(Fill, Constructor_ThrowsIfValueNonPositive) {
    Ticker t = make_ticker();

    EXPECT_THROW(Fill(10, 0.0, t), std::invalid_argument);
    EXPECT_THROW(Fill(10, -1.0, t), std::invalid_argument);
}

// ==================== Happy path ====================

TEST(Fill, Constructor_SetsFieldsCorrectly) {
    Ticker t = make_ticker();

    std::int64_t qty = 7;
    double val = 123.45;

    Fill f(qty, val, t);

    EXPECT_EQ(f.quantity(), qty);
    EXPECT_DOUBLE_EQ(f.value(), val);
    EXPECT_EQ(f.ticker().str(), t.str());
}

// ==================== Accessors ====================

TEST(Fill, Accessors_ReturnStableValues) {
    Ticker t = make_ticker();
    Fill f(1, 1.0, t);

    EXPECT_EQ(f.quantity(), 1);
    EXPECT_DOUBLE_EQ(f.value(), 1.0);
    EXPECT_EQ(f.ticker().str(), t.str());
}
