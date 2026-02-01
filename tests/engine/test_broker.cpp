#include <gtest/gtest.h>

#include <cstdint>
#include <stdexcept>

#include "engine/broker.hpp"
#include "domain/order.hpp"
#include "execution/fill.hpp"
#include "domain/ticker.hpp"

using namespace bt;

namespace {

Ticker make_ticker() {
    // Adjust if your Ticker API differs
    return Ticker{"TEST"};
}

Order make_order_buy(std::int64_t qty = 10) {
    return Order::Market(Side::Buy, qty);
}

Order make_order_sell(std::int64_t qty = 10) {
    return Order::Market(Side::Sell, qty);
}

Fill make_fill(std::int64_t qty = 5, double value = 100.0) {
    return Fill(qty, value, make_ticker());
}

} // namespace

TEST(Broker_Basics, StartsEmpty) {
    Broker b;
    EXPECT_EQ(b.actions_count(), 0u);
    EXPECT_THROW(b.get_action(), std::runtime_error);
}

TEST(Broker_SubmitOrder, EnqueuesInsertOrder) {
    Broker b;
    Order o = make_order_buy(10);

    b.submit_order(o);

    EXPECT_EQ(b.actions_count(), 1u);

    Action a = b.get_action();
    EXPECT_EQ(a.action_type(), ActionType::InsertOrder);
    EXPECT_NO_THROW(a.order());
    EXPECT_THROW(a.fill(), std::runtime_error);
    EXPECT_THROW(a.order_id(), std::runtime_error);

    EXPECT_EQ(b.actions_count(), 0u);
}

TEST(Broker_CancelOrder, EnqueuesCancelOrder) {
    Broker b;

    b.cancel_order(42);

    EXPECT_EQ(b.actions_count(), 1u);

    Action a = b.get_action();
    EXPECT_EQ(a.action_type(), ActionType::CancelOrder);
    EXPECT_EQ(a.order_id(), 42u);
    EXPECT_THROW(a.order(), std::runtime_error);
    EXPECT_THROW(a.fill(), std::runtime_error);

    EXPECT_EQ(b.actions_count(), 0u);
}

TEST(Broker_ExecuteOrder, EnqueuesExecuteOrder) {
    Broker b;

    b.execute_order(7);

    EXPECT_EQ(b.actions_count(), 1u);

    Action a = b.get_action();
    EXPECT_EQ(a.action_type(), ActionType::ExecuteOrder);
    EXPECT_EQ(a.order_id(), 7u);
    EXPECT_THROW(a.order(), std::runtime_error);
    EXPECT_THROW(a.fill(), std::runtime_error);

    EXPECT_EQ(b.actions_count(), 0u);
}

TEST(Broker_SubmitFill, EnqueuesApplyFill) {
    Broker b;
    Order o = make_order_sell(3);
    Fill f = make_fill(3, 250.0);

    b.submit_fill(f, o);

    EXPECT_EQ(b.actions_count(), 1u);

    Action a = b.get_action();
    EXPECT_EQ(a.action_type(), ActionType::ApplyFill);
    EXPECT_NO_THROW(a.order());
    EXPECT_NO_THROW(a.fill());
    EXPECT_THROW(a.order_id(), std::runtime_error);

    EXPECT_EQ(a.fill().quantity(), f.quantity());
    EXPECT_EQ(a.fill().value(), f.value());

    EXPECT_EQ(b.actions_count(), 0u);
}

TEST(Broker_FIFO, ReturnsActionsInSubmissionOrder) {
    Broker b;

    Order o1 = make_order_buy(10);
    Order o2 = make_order_sell(20);
    Fill f  = make_fill(5, 100.0);

    b.submit_order(o1);          // 1
    b.cancel_order(111);         // 2
    b.execute_order(222);        // 3
    b.submit_fill(f, o2);        // 4

    EXPECT_EQ(b.actions_count(), 4u);

    {
        Action a = b.get_action();
        EXPECT_EQ(a.action_type(), ActionType::InsertOrder);
        EXPECT_NO_THROW(a.order());
    }
    {
        Action a = b.get_action();
        EXPECT_EQ(a.action_type(), ActionType::CancelOrder);
        EXPECT_EQ(a.order_id(), 111u);
    }
    {
        Action a = b.get_action();
        EXPECT_EQ(a.action_type(), ActionType::ExecuteOrder);
        EXPECT_EQ(a.order_id(), 222u);
    }
    {
        Action a = b.get_action();
        EXPECT_EQ(a.action_type(), ActionType::ApplyFill);
        EXPECT_NO_THROW(a.order());
        EXPECT_NO_THROW(a.fill());
    }

    EXPECT_EQ(b.actions_count(), 0u);
    EXPECT_THROW(b.get_action(), std::runtime_error);
}
