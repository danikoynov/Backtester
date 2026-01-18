#include <gtest/gtest.h>

#include "domain/order.hpp" 

using namespace bt;

/// Test factory constructors
TEST(OrderCtor, MarketOrderConstructsSuccesfuly) {
    EXPECT_NO_THROW(Order::Market(Side::Buy, 50));
}

TEST(OrderCtor, LimitOrderConstructsSuccesfuly) {
    EXPECT_NO_THROW(Order::Limit(Side::Sell, 28, 60.5));
}

TEST(OrderCtor, StopOrderConstructsSuccesfuly) {
    EXPECT_NO_THROW(Order::Stop(Side::Buy, 10, 40.5));
}

TEST(OrderCtor, StopLimitOrderConstructsSuccesfuly) {
    EXPECT_NO_THROW(Order::StopLimit(Side::Sell, 60, 30.2, 30.5));
}


/// Test quantity constraints

TEST(OrderCtor, NonPositiveQuantityThrows) {
    EXPECT_THROW(Order::Market(Side::Buy, -40), 
        std::invalid_argument);
}

/// Test limit price constraints

TEST(OrderCtor, NegativeLimitPriceThrows) {
    EXPECT_THROW(Order::Limit(Side::Sell, 40, -30.0),
        std::invalid_argument);
    EXPECT_THROW(Order::StopLimit(Side::Buy, 40, -30.0, 50),
        std::invalid_argument);    
}

/// Test stop price constraints

TEST(OrderCtor, NegativeStopPriceThrows) {
    EXPECT_THROW(Order::Stop(Side::Sell, 40, -30.0),
        std::invalid_argument);
    EXPECT_THROW(Order::StopLimit(Side::Buy, 40, 30.0, -50),
        std::invalid_argument);    
}


