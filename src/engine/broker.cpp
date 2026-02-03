#include "engine/broker.hpp"

#include <vector>
#include <stdexcept>

namespace bt {

    size_t Broker::actions_count() const {
        return actions_.size();
    }

    std::pair<Ticker, Action> Broker::get_action() {
        if (actions_count() == 0) {
            throw std::runtime_error("Action requested from broker but the queue is empty");
        }
    
        std::pair<Ticker, Action> next_action = actions_.front();
        actions_.pop();
        return next_action;
    }

    void Broker::submit_order(const Order &order, const Ticker& ticker) {
        actions_.push({ticker, Action::InsertOrder(order)});
    }

    void Broker::submit_fill(const Fill &fill, const Order &order, const Ticker& ticker) {
        actions_.push({ticker, Action::ApplyFill(fill, order)});
    }

    void Broker::cancel_order(std::uint64_t order_id, const Ticker& ticker) {
        actions_.push({ticker, Action::CancelOrder(order_id)});
    }

    void Broker::execute_order(std::uint64_t order_id, const Ticker& ticker) {
        actions_.push({ticker, Action::ExecuteOrder(order_id)});
    }

    
}