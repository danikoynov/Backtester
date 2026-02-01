#include <gtest/gtest.h>

#include <optional>
#include <cstdint>
#include <stdexcept>

#include "engine/action.hpp"
#include "domain/order.hpp"
#include "execution/fill.hpp"
#include "domain/ticker.hpp"

using namespace bt;

namespace {

Ticker make_ticker() {
    return Ticker{"TEST"};
}

Order make_order() {
    return Order::Market(Side::Buy, 10);
}

Fill make_fill() {
    return Fill(/*quantity=*/5, /*value=*/100.0, make_ticker());
}

} // namespace

TEST(Action_Factory, InsertOrder_HasOnlyOrder) {
    Order o = make_order();
    Action a = Action::InsertOrder(o);

    EXPECT_EQ(a.action_type(), ActionType::InsertOrder);
    EXPECT_NO_THROW(a.order());
    EXPECT_THROW(a.fill(), std::runtime_error);
    EXPECT_THROW(a.order_id(), std::runtime_error);
}

TEST(Action_Factory, CancelOrder_HasOnlyOrderId) {
    Action a = Action::CancelOrder(std::uint64_t{42});

    EXPECT_EQ(a.action_type(), ActionType::CancelOrder);
    EXPECT_NO_THROW(a.order_id());
    EXPECT_THROW(a.order(), std::runtime_error);
    EXPECT_THROW(a.fill(), std::runtime_error);
}

TEST(Action_Factory, ExecuteOrder_HasOnlyOrderId) {
    Action a = Action::ExecuteOrder(std::uint64_t{7});

    EXPECT_EQ(a.action_type(), ActionType::ExecuteOrder);
    EXPECT_NO_THROW(a.order_id());
    EXPECT_THROW(a.order(), std::runtime_error);
    EXPECT_THROW(a.fill(), std::runtime_error);
}

TEST(Action_Factory, ApplyFill_HasOrderAndFill) {
    Order o = make_order();
    Fill f = make_fill();

    Action a = Action::ApplyFill(f, o);

    EXPECT_EQ(a.action_type(), ActionType::ApplyFill);
    EXPECT_NO_THROW(a.order());
    EXPECT_NO_THROW(a.fill());
    EXPECT_THROW(a.order_id(), std::runtime_error);
}

TEST(Action_FactoryInvalid, InsertOrder_NulloptOrder_ThrowsInvalidArgument) {
    EXPECT_THROW(Action::InsertOrder(std::nullopt), std::invalid_argument);
}

TEST(Action_FactoryInvalid, CancelOrder_NulloptId_ThrowsInvalidArgument) {
    EXPECT_THROW(Action::CancelOrder(std::nullopt), std::invalid_argument);
}

TEST(Action_FactoryInvalid, ExecuteOrder_NulloptId_ThrowsInvalidArgument) {
    EXPECT_THROW(Action::ExecuteOrder(std::nullopt), std::invalid_argument);
}

TEST(Action_FactoryInvalid, ApplyFill_NulloptFill_ThrowsInvalidArgument) {
    Order o = make_order();
    EXPECT_THROW(Action::ApplyFill(std::nullopt, o), std::invalid_argument);
}
