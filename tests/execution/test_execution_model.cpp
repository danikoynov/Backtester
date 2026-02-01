// tests/test_execution_model.cpp
#include <gtest/gtest.h>
#include <optional>
#include <chrono>

#include "execution/execution_model.hpp"   // <-- rename to your actual header
#include "data/bar.hpp"
#include "domain/order.hpp"
#include "execution/fill.hpp"

using namespace bt;

namespace {

    Timestamp ts0() { return Timestamp{}; }

    Bar make_bar(double open, double high, double low, double close, std::int64_t volume = 100) {
        // Invariant reminder:
        // low <= open <= high, low <= close <= high, low <= high, low >= 0, volume >= 0
        return Bar(ts0(), open, high, low, close, volume);
    }

    Ticker make_ticker() {
        return Ticker{"AAPL"};
    }

} // namespace


// ==================== try_fill_market ====================

TEST(ExecutionModel, TryFillMarket_FillsAtOpenPrice) {
    Ticker ticker = make_ticker();
    Bar bar = make_bar(/*open=*/10.0, /*high=*/12.0, /*low=*/9.0, /*close=*/11.0);

    std::int64_t qty = 7;
    Order order = Order::Market(Side::Buy, qty);

    std::optional<Fill> fill = try_fill_market(order, bar, ticker);

    ASSERT_TRUE(fill.has_value());
    EXPECT_EQ(fill->quantity(), qty);
    EXPECT_DOUBLE_EQ(fill->value(), static_cast<double>(qty) * bar.open());
    EXPECT_EQ(fill->ticker().str(), ticker.str());
}


// ==================== try_fill_limit (BUY) ====================

TEST(ExecutionModel, TryFillLimit_Buy_FillsWhenLowLeqLimit) {
    Ticker ticker = make_ticker();
    Bar bar = make_bar(/*open=*/10.0, /*high=*/12.0, /*low=*/9.0, /*close=*/11.0);

    std::int64_t qty = 5;
    double limit = 9.5;
    Order order = Order::Limit(Side::Buy, qty, limit);

    std::optional<Fill> fill = try_fill_limit(order, bar, ticker);

    ASSERT_TRUE(fill.has_value());
    EXPECT_EQ(fill->quantity(), qty);
    EXPECT_DOUBLE_EQ(fill->value(), static_cast<double>(qty) * limit);
    EXPECT_EQ(fill->ticker().str(), ticker.str());
}

TEST(ExecutionModel, TryFillLimit_Buy_DoesNotFillWhenLowAboveLimit) {
    Ticker ticker = make_ticker();
    // low is ABOVE limit, but still <= open/close <= high
    Bar bar = make_bar(/*open=*/10.2, /*high=*/12.0, /*low=*/10.1, /*close=*/11.0);

    double limit = 10.0;
    Order order = Order::Limit(Side::Buy, /*qty=*/5, limit);

    std::optional<Fill> fill = try_fill_limit(order, bar, ticker);

    EXPECT_FALSE(fill.has_value());
}


// ==================== try_fill_limit (SELL) ====================

TEST(ExecutionModel, TryFillLimit_Sell_FillsWhenHighGeqLimit) {
    Ticker ticker = make_ticker();
    Bar bar = make_bar(/*open=*/10.0, /*high=*/12.0, /*low=*/9.0, /*close=*/11.0);

    std::int64_t qty = 4;
    double limit = 11.5;
    Order order = Order::Limit(Side::Sell, qty, limit);

    std::optional<Fill> fill = try_fill_limit(order, bar, ticker);

    ASSERT_TRUE(fill.has_value());
    EXPECT_EQ(fill->quantity(), qty);
    EXPECT_DOUBLE_EQ(fill->value(), static_cast<double>(qty) * limit);
    EXPECT_EQ(fill->ticker().str(), ticker.str());
}

TEST(ExecutionModel, TryFillLimit_Sell_DoesNotFillWhenHighBelowLimit) {
    Ticker ticker = make_ticker();
    // high is BELOW limit, but bar still valid
    Bar bar = make_bar(/*open=*/10.0, /*high=*/10.9, /*low=*/9.0, /*close=*/10.1);

    double limit = 11.0;
    Order order = Order::Limit(Side::Sell, /*qty=*/4, limit);

    std::optional<Fill> fill = try_fill_limit(order, bar, ticker);

    EXPECT_FALSE(fill.has_value());
}


// ==================== try_activate_stop ====================

TEST(ExecutionModel, TryActivateStop_Buy_TriggersWhenHighGeqStop) {
    Ticker ticker = make_ticker();
    Bar bar = make_bar(/*open=*/10.0, /*high=*/15.0, /*low=*/9.0, /*close=*/14.0);

    std::int64_t qty = 3;
    double stop = 13.0;
    Order stop_order = Order::Stop(Side::Buy, qty, stop);

    std::optional<Order> activated = try_activate_stop(stop_order, bar, ticker);

    ASSERT_TRUE(activated.has_value());
    EXPECT_EQ(activated->side(), Side::Buy);
    EXPECT_EQ(activated->quantity(), qty);

    // If you have order.type(), you can assert Market here.
    // EXPECT_EQ(activated->type(), OrderType::Market);
}

TEST(ExecutionModel, TryActivateStop_Buy_DoesNotTriggerWhenHighBelowStop) {
    Ticker ticker = make_ticker();
    Bar bar = make_bar(/*open=*/10.0, /*high=*/12.9, /*low=*/9.0, /*close=*/12.0);

    Order stop_order = Order::Stop(Side::Buy, /*qty=*/3, /*stop=*/13.0);

    std::optional<Order> activated = try_activate_stop(stop_order, bar, ticker);

    EXPECT_FALSE(activated.has_value());
}

TEST(ExecutionModel, TryActivateStop_Sell_TriggersWhenLowLeqStop) {
    Ticker ticker = make_ticker();
    Bar bar = make_bar(/*open=*/10.0, /*high=*/12.0, /*low=*/7.0, /*close=*/8.0);

    std::int64_t qty = 6;
    double stop = 8.0;
    Order stop_order = Order::Stop(Side::Sell, qty, stop);

    std::optional<Order> activated = try_activate_stop(stop_order, bar, ticker);

    ASSERT_TRUE(activated.has_value());
    EXPECT_EQ(activated->side(), Side::Sell);
    EXPECT_EQ(activated->quantity(), qty);

    // EXPECT_EQ(activated->type(), OrderType::Market);
}


// ==================== try_activate_stop_limit ====================

TEST(ExecutionModel, TryActivateStopLimit_Buy_TriggersToLimitOrder) {
    Ticker ticker = make_ticker();
    Bar bar = make_bar(/*open=*/10.0, /*high=*/15.0, /*low=*/9.0, /*close=*/14.0);

    std::int64_t qty = 2;
    double stop = 13.0;
    double limit = 13.2;

    Order stop_limit = Order::StopLimit(Side::Buy, qty, limit, stop);

    std::optional<Order> activated = try_activate_stop_limit(stop_limit, bar, ticker);

    ASSERT_TRUE(activated.has_value());
    EXPECT_EQ(activated->side(), Side::Buy);
    EXPECT_EQ(activated->quantity(), qty);
    EXPECT_DOUBLE_EQ(activated->limit_price(), limit);

    // EXPECT_EQ(activated->type(), OrderType::Limit);
}

TEST(ExecutionModel, TryActivateStopLimit_Sell_DoesNotTriggerWhenLowAboveStop) {
    Ticker ticker = make_ticker();
    // low is ABOVE stop -> should not trigger; bar must remain valid
    Bar bar = make_bar(/*open=*/10.0, /*high=*/12.0, /*low=*/9.1, /*close=*/9.5);

    Order stop_limit = Order::StopLimit(Side::Sell, /*qty=*/2, /*limit=*/9.0, /*stop=*/9.0);

    std::optional<Order> activated = try_activate_stop_limit(stop_limit, bar, ticker);

    EXPECT_FALSE(activated.has_value());
}
