#include "engine/action.hpp"
#include <stdexcept>

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
        switch (action_type_) {

            case ActionType::InsertOrder:
                if (!order_ || fill_ || order_id_) {
                    throw std::invalid_argument(
                        "InsertOrder requires order only"
                    );
                }
                break;

            case ActionType::CancelOrder:
                if (!order_id_ || order_ || fill_) {
                    throw std::invalid_argument(
                        "CancelOrder requires order_id only"
                    );
                }
                break;

            case ActionType::ExecuteOrder:
                if (!order_id_ || order_ || fill_) {
                    throw std::invalid_argument(
                        "ExecuteOrder requires order_id only"
                    );
                }
                break;

            case ActionType::ApplyFill:
                if (!order_ || !fill_ || order_id_) {
                    throw std::invalid_argument(
                        "ApplyFill requires order and fill"
                    );
                }
                break;
        }
    };

    const ActionType &Action::action_type() const {
        if (!action_type_) [[unlikely]] {
            throw std::runtime_error("Accessed action does not contain action_type.");
        }
        return action_type_;
    }

    const Order &Action::order() const {
        if (!order_) [[unlikely]] {
            throw std::runtime_error("Accessed action does not contain order.");
        }
        return order_.value();
    }

    const Fill &Action::fill() const {
        if (!fill_) [[unlikely]] {
            throw std::runtime_error("Accessed action does not contain fill.");
        }
        return fill_.value();
    }

    std::uint64_t Action::order_id() const {
        if (!order_id_) [[unlikely]] {
            throw std::runtime_error("Accessed action does not contain order_id.");
        }
        return order_id_.value();
    }
}