#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <functional>
#include <domain/order.hpp>

namespace bt {

    /*
    OrderBook

    Purpose:
        Owns orders and tracks their lifecycle state (active, executed, cancelled).
        Assigns a unique increasing ID to each inserted order.

    Invariants:
        - Order IDs are assigned sequentially starting from 1.
        - Every ID contained in active_, executed_, or cancelled_ exists as a key in orders_.
        - An ID exists in at most one of { active_, executed_, cancelled_ }.
        - Once an order leaves active_, it never returns.

    Errors:
        - set_executed(id) throws std::runtime_error if id is not currently active.
        - set_cancelled(id) throws std::runtime_error if id is not currently active.
    */
    class OrderBook {
        public:
            /*
            Purpose:
                Constructs an empty OrderBook.

            Invariants:
                - All internal containers are empty.
                - The next inserted order will receive ID 1.

            Errors:
                - Does not throw.
            */
            OrderBook() = default;

            /*
            Purpose:
                Inserts a new order and marks it as active.

            Invariants:
                - A new unique ID is assigned to the order.
                - The inserted order's ID is added to active_.
                - The order is stored in orders_.

            Errors:
                - Does not throw.
            */
            void insert_order(const Order &order);

            /*
            Purpose:
                Marks an active order as executed.

            Invariants:
                - The order ID is removed from active_.
                - The order ID is inserted into executed_.

            Errors:
                - Throws std::runtime_error if id is not currently active.
            */
            void set_executed(std::uint64_t id);

            /*
            Purpose:
                Marks an active order as cancelled.

            Invariants:
                - The order ID is removed from active_.
                - The order ID is inserted into cancelled_.

            Errors:
                - Throws std::runtime_error if id is not currently active.
            */
            void set_cancelled(std::uint64_t id);

            /*
            Purpose:
                Returns all active orders.

            Invariants:
                - Returned references refer to orders owned by the OrderBook.

            Errors:
                - Does not throw.
            */
            std::vector<std::pair<std::uint64_t, std::reference_wrapper<const Order>>> 
                get_active_orders() const;

            /*
            Purpose:
                Returns all executed orders.

            Invariants:
                - Returned references refer to orders owned by the OrderBook.

            Errors:
                - Does not throw.
            */
            std::vector<std::pair<std::uint64_t, std::reference_wrapper<const Order>>> 
                get_executed_orders() const;

            /*
            Purpose:
                Returns all cancelled orders.

            Invariants:
                - Returned references refer to orders owned by the OrderBook.

            Errors:
                - Does not throw.
            */
            std::vector<std::pair<std::uint64_t, std::reference_wrapper<const Order>>> 
                get_cancelled_orders() const;

        private:
            /*
            Purpose:
                Returns orders corresponding to a given set of IDs.

            Invariants:
                - Each ID in ids exists as a key in orders_.

            Errors:
                - May throw std::out_of_range if an ID is missing from orders_.
            */
            std::vector<std::pair<std::uint64_t, std::reference_wrapper<const Order>>> 
                get_orders(const std::unordered_set<std::uint64_t>& ids) const;

            std::uint64_t order_counter = 0;
            std::unordered_map<std::uint64_t, Order> orders_;
            std::unordered_set<std::uint64_t> active_;
            std::unordered_set<std::uint64_t> executed_;
            std::unordered_set<std::uint64_t> cancelled_;
    };

}
