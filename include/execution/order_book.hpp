#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <functional>
#include <domain/order.hpp>

namespace bt {

    class OrderBook {
        public:
            OrderBook() = default;
            void insert_order(const Order &order);
            void set_executed(std::uint64_t id);
            void set_cancelled(std::uint64_t id);
            std::vector<std::pair<std::uint64_t, std::reference_wrapper<const Order>>> 
                get_active_orders() const;
            std::vector<std::pair<std::uint64_t, std::reference_wrapper<const Order>>> 
                get_executed_orders() const;
            std::vector<std::pair<std::uint64_t, std::reference_wrapper<const Order>>> 
                get_cancelled_orders() const;

        private:
            std::vector<std::pair<std::uint64_t, std::reference_wrapper<const Order>>> 
                get_orders(const std::unordered_set<std::uint64_t>& ids) const;
            
            std::uint64_t order_counter = 0;
            std::unordered_map<std::uint64_t, Order> orders_;
            std::unordered_set<std::uint64_t> active_;
            std::unordered_set<std::uint64_t> executed_;
            std::unordered_set<std::uint64_t> cancelled_;
    };
}