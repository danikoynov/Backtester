#include <gtest/gtest.h>
#include "execution/order_book.hpp"
#include "domain/order.hpp"
#include <algorithm>

using namespace bt;

TEST(OrderBookCtor, OrderBookConstructsSuccessfully) {
    EXPECT_NO_THROW(OrderBook order_book = OrderBook());
}

TEST(OrderBookInsert, OrderInsertsSuccessfully) {
    EXPECT_NO_THROW(
        OrderBook order_book = OrderBook();
        std::vector < Order > orders;
        orders.push_back(Order::Limit(Side::Sell, 40, 30.0));
        orders.push_back(Order::StopLimit(Side::Buy, 40, 30.0, 50));
        orders.push_back(Order::Market(Side::Buy, 33));

        for (const Order &order : orders) {
            order_book.insert_order(order);
        }
    );
}


TEST(OrderBookCancel, OrderCancelsSuccessfully) {
    OrderBook order_book = OrderBook();
    std::vector < Order > orders;
    orders.push_back(Order::Limit(Side::Sell, 40, 30.0));
    orders.push_back(Order::StopLimit(Side::Buy, 40, 30.0, 50));
    orders.push_back(Order::Market(Side::Buy, 33));

    for (const Order &order : orders) {
        order_book.insert_order(order);
    }

    EXPECT_NO_THROW(
        order_book.set_cancelled(2);
        order_book.set_cancelled(1);
        order_book.set_cancelled(3)
    );

    std::vector<std::pair<
        std::uint64_t, 
        std::reference_wrapper<const Order>>> cancelled =  
        order_book.get_cancelled_orders();
    
    std::vector<std::uint64_t> cancelled_ids;
    for (std::pair<
        std::uint64_t, 
        std::reference_wrapper<const Order>> order : cancelled)
        cancelled_ids.push_back(order.first);

    std::sort(cancelled_ids.begin(), cancelled_ids.end());

    ASSERT_EQ(cancelled_ids[0], 1);
    ASSERT_EQ(cancelled_ids[1], 2);
    ASSERT_EQ(cancelled_ids[2], 3);

}

TEST(OrderBookCancel, OrderCancelOutOfRangeFails) {
    OrderBook order_book = OrderBook();
    std::vector < Order > orders;
    orders.push_back(Order::Limit(Side::Sell, 40, 30.0));
    orders.push_back(Order::StopLimit(Side::Buy, 40, 30.0, 50));
    orders.push_back(Order::Market(Side::Buy, 33));

    for (const Order &order : orders) {
        order_book.insert_order(order);
    }

    EXPECT_THROW(order_book.set_cancelled(4), 
        std::runtime_error);
}


TEST(OrderBookExecute, OrderExecutesSuccessfully) {
    OrderBook order_book = OrderBook();
    std::vector < Order > orders;
    orders.push_back(Order::Limit(Side::Sell, 40, 30.0));
    orders.push_back(Order::StopLimit(Side::Buy, 40, 30.0, 50));
    orders.push_back(Order::Market(Side::Buy, 33));

    for (const Order &order : orders) {
        order_book.insert_order(order);
    }

    EXPECT_NO_THROW(
        order_book.set_executed(2);
        order_book.set_executed(1);
        order_book.set_executed(3);
    );

    std::vector<std::pair<
        std::uint64_t, 
        std::reference_wrapper<const Order>>> executed =  
        order_book.get_executed_orders();
    
    std::vector<std::uint64_t> executed_ids;
    for (std::pair<
        std::uint64_t, 
        std::reference_wrapper<const Order>> order : executed)
        executed_ids.push_back(order.first);

    std::sort(executed_ids.begin(), executed_ids.end());

    ASSERT_EQ(executed_ids[0], 1);
    ASSERT_EQ(executed_ids[1], 2);
    ASSERT_EQ(executed_ids[2], 3);
}

TEST(OrderBookExecute, OrderExecuteOutOfRangeFails) {
    OrderBook order_book = OrderBook();
    std::vector < Order > orders;
    orders.push_back(Order::Limit(Side::Sell, 40, 30.0));
    orders.push_back(Order::StopLimit(Side::Buy, 40, 30.0, 50));
    orders.push_back(Order::Market(Side::Buy, 33));

    for (const Order &order : orders) {
        order_book.insert_order(order);
    }

    EXPECT_THROW(order_book.set_executed(4), 
        std::runtime_error);
}