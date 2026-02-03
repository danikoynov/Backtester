// tests/test_broker.cpp
#include <gtest/gtest.h>

#include <cstdint>
#include <stdexcept>

#include "engine/broker.hpp"
#include "engine/action.hpp"
#include "domain/ticker.hpp"
#include "domain/order.hpp"
#include "execution/fill.hpp"

namespace bt {


TEST(BrokerTests, ActionsCountInitiallyZero) {
    Broker b;
    EXPECT_EQ(b.actions_count(), 0u);
}

TEST(BrokerTests, GetActionOnEmptyThrows) {
    Broker b;
    EXPECT_THROW((void)b.get_action(), std::runtime_error);
}

TEST(BrokerTests, SubmitOrderPushesOneActionWithTicker) {
    Broker b;
    Ticker t("AAPL");

    Order o = Order::Market(Side::Buy, 10);
    b.submit_order(o, t);

    EXPECT_EQ(b.actions_count(), 1u);

    auto [ticker, action] = b.get_action();
    EXPECT_EQ(b.actions_count(), 0u);

    EXPECT_EQ(ticker.str(), "AAPL");

    EXPECT_EQ(action.action_type(), ActionType::InsertOrder);
    EXPECT_EQ(action.order().type(), OrderType::Market);
    EXPECT_EQ(action.order().side(), Side::Buy);
    EXPECT_EQ(action.order().quantity(), 10);

    // market order must not have prices; accessing should throw
    EXPECT_THROW((void)action.order().limit_price(), std::runtime_error);
    EXPECT_THROW((void)action.order().stop_price(), std::runtime_error);
}

TEST(BrokerTests, SubmitLimitOrderKeepsLimitPrice) {
    Broker b;
    Ticker t("AAPL");

    Order o = Order::Limit(Side::Sell, 2, 123.0);
    b.submit_order(o, t);

    auto [ticker, action] = b.get_action();
    EXPECT_EQ(ticker.str(), "AAPL");

    EXPECT_EQ(action.action_type(), ActionType::InsertOrder);
    EXPECT_EQ(action.order().type(), OrderType::Limit);
    EXPECT_EQ(action.order().side(), Side::Sell);
    EXPECT_EQ(action.order().quantity(), 2);

    EXPECT_DOUBLE_EQ(action.order().limit_price(), 123.0);
    EXPECT_THROW((void)action.order().stop_price(), std::runtime_error);
}

// -----------------------------
// Cancel / Execute carry order_id
// -----------------------------
TEST(BrokerTests, CancelOrderPushesCancelActionWithTickerAndId) {
    Broker b;
    Ticker t("MSFT");
    std::uint64_t id = 42;

    b.cancel_order(id, t);
    EXPECT_EQ(b.actions_count(), 1u);

    auto [ticker, action] = b.get_action();
    EXPECT_EQ(ticker.str(), "MSFT");

    EXPECT_EQ(action.action_type(), ActionType::CancelOrder);
    EXPECT_EQ(action.order_id(), id);
}

TEST(BrokerTests, ExecuteOrderPushesExecuteActionWithTickerAndId) {
    Broker b;
    Ticker t("NVDA");
    std::uint64_t id = 7;

    b.execute_order(id, t);
    EXPECT_EQ(b.actions_count(), 1u);

    auto [ticker, action] = b.get_action();
    EXPECT_EQ(ticker.str(), "NVDA");

    EXPECT_EQ(action.action_type(), ActionType::ExecuteOrder);
    EXPECT_EQ(action.order_id(), id);
}


TEST(BrokerTests, SubmitFillPushesApplyFillActionWithTicker) {
    Broker b;
    Ticker t("TSLA");

    Order o = Order::Market(Side::Buy, 5);
    Fill f(/*quantity=*/5, /*value=*/500.0, t);

    b.submit_fill(f, o, t);
    EXPECT_EQ(b.actions_count(), 1u);

    auto [ticker, action] = b.get_action();
    EXPECT_EQ(ticker.str(), "TSLA");

    EXPECT_EQ(action.action_type(), ActionType::ApplyFill);

    EXPECT_EQ(action.fill().quantity(), 5);
    EXPECT_DOUBLE_EQ(action.fill().value(), 500.0);
    EXPECT_EQ(action.fill().ticker().str(), "TSLA");

    EXPECT_EQ(action.order().type(), OrderType::Market);
    EXPECT_EQ(action.order().side(), Side::Buy);
    EXPECT_EQ(action.order().quantity(), 5);
    EXPECT_THROW((void)action.order().limit_price(), std::runtime_error);
    EXPECT_THROW((void)action.order().stop_price(), std::runtime_error);
}


TEST(BrokerTests, FIFOOrderAcrossDifferentTickersIsPreserved) {
    Broker b;

    Ticker a("AAPL");
    Ticker m("MSFT");
    Ticker n("NVDA");

    Order o1 = Order::Market(Side::Buy, 1);
    Order o2 = Order::Limit(Side::Sell, 2, 123.0);

    b.submit_order(o1, a);     
    b.cancel_order(10, m);     
    b.execute_order(11, n);    
    b.submit_order(o2, a);     

    ASSERT_EQ(b.actions_count(), 4u);

    {
        auto [ticker, action] = b.get_action();
        EXPECT_EQ(ticker.str(), "AAPL");
        EXPECT_EQ(action.action_type(), ActionType::InsertOrder);
        EXPECT_EQ(action.order().type(), OrderType::Market);
        EXPECT_EQ(action.order().quantity(), 1);
    }
    {
        auto [ticker, action] = b.get_action();
        EXPECT_EQ(ticker.str(), "MSFT");
        EXPECT_EQ(action.action_type(), ActionType::CancelOrder);
        EXPECT_EQ(action.order_id(), 10u);
    }
    {
        auto [ticker, action] = b.get_action();
        EXPECT_EQ(ticker.str(), "NVDA");
        EXPECT_EQ(action.action_type(), ActionType::ExecuteOrder);
        EXPECT_EQ(action.order_id(), 11u);
    }
    {
        auto [ticker, action] = b.get_action();
        EXPECT_EQ(ticker.str(), "AAPL");
        EXPECT_EQ(action.action_type(), ActionType::InsertOrder);

        EXPECT_EQ(action.order().type(), OrderType::Limit);
        EXPECT_EQ(action.order().side(), Side::Sell);
        EXPECT_EQ(action.order().quantity(), 2);
        EXPECT_DOUBLE_EQ(action.order().limit_price(), 123.0);
        EXPECT_THROW((void)action.order().stop_price(), std::runtime_error);
    }

    EXPECT_EQ(b.actions_count(), 0u);
}

TEST(BrokerTests, ActionsCountDecrementsAsActionsArePopped) {
    Broker b;
    Ticker t("AAPL");

    b.submit_order(Order::Market(Side::Buy, 1), t);
    b.cancel_order(1, t);
    b.execute_order(2, t);

    EXPECT_EQ(b.actions_count(), 3u);

    (void)b.get_action();
    EXPECT_EQ(b.actions_count(), 2u);

    (void)b.get_action();
    EXPECT_EQ(b.actions_count(), 1u);

    (void)b.get_action();
    EXPECT_EQ(b.actions_count(), 0u);
}

}