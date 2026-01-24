#include "engine/broker.hpp"

#include <vector>
#include <stdexcept>

namespace bt {

    size_t Broker::actions_count() const {
        return actions_.size();
    }
    const Action &Broker::get_action() {
        if (actions_count() == 0) {
            throw std::runtime_error("Action requested from broker but the queue is empty");
        }

        Action next_action = actions_.front();
        actions_.pop();
        return next_action;
    }

    void Broker::submit_order(const Order &order) {
        actions_.push(Action::InsertOrder(order));
    }

    void Broker::submit_fill(const Fill &fill) {
        actions_.push(Action::ApplyFill(fill));
    }

    void Broker::cancel_order(std::uint64_t order_id) {
        actions_.push(Action::CancelOrder(order_id));
    }

    void Broker::execute_order(std::uint64_t order_id) {
        actions_.push(Action::ExecuteOrder(order_id));
    }

    
}