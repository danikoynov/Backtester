#include "engine/action.hpp"

namespace bt {

    Action::Action(
        ActionType action_type,
        std::optional<Order> order,
        std::optional<Fill> fill,
        std::optional<std::uint64_t> order_id
    )
        : action_type_(action_type),
          order_(order),
          fill_(fill),
          order_id_(order_id) {
    
    };

    const ActionType &Action::action_type() const {
        return action_type_;
    }

    const Order &Action::order() const {
        return order_.value();
    }

    const Fill &Action::fill() const {
        return fill_.value();
    }

    std::uint64_t Action::order_id() const {
        return order_id_.value();
    }
}