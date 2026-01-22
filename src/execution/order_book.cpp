#include <execution/order_book.hpp>
#include <domain/order.hpp>
#include <stdexcept>
#include <cassert>

namespace bt {

    std::vector<std::pair<std::uint64_t, std::reference_wrapper<const Order>>> 
        OrderBook::get_orders(const std::unordered_set<std::uint64_t> &ids) const {
        std::vector<std::pair<std::uint64_t, std::reference_wrapper<const Order>>> res;
        res.reserve(ids.size());
        for (std::uint64_t id : ids)
            res.push_back({id, orders_.at(id)});
        return res;
    }

    std::vector<std::pair<std::uint64_t, std::reference_wrapper<const Order>>> 
        OrderBook::get_active_orders() const { 
            return get_orders(active_);
        }

    std::vector<std::pair<std::uint64_t, std::reference_wrapper<const Order>>> 
        OrderBook::get_executed_orders() const { 
            return get_orders(executed_);
        }       

    std::vector<std::pair<std::uint64_t, std::reference_wrapper<const Order>>> 
        OrderBook::get_cancelled_orders() const { 
            return get_orders(cancelled_);
        }

    void OrderBook::insert_order(const Order &order){
        order_counter ++;
        orders_.emplace(order_counter, order);
        active_.insert(order_counter);
    }

    void OrderBook::set_executed(std::uint64_t id) {
        if (active_.find(id) == active_.end()) {
            throw std::runtime_error("Order id not found in set of active ids");
        }
        active_.erase(id);
        executed_.insert(id);
    }

    void OrderBook::set_cancelled(std::uint64_t id) {
        if (active_.find(id) == active_.end()) {
            throw std::runtime_error("Order id not found in set of active ids");
        }
        active_.erase(id);
        cancelled_.insert(id);
    }

}
