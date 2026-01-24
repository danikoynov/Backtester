#pragma once
#include <domain/order.hpp>
#include <execution/fill.hpp>
#include <optional>

namespace bt {

    enum class ActionType {
        InsertOrder,
        CancelOrder,
        ExecuteOrder,
        ApplyFill
    };

    class Action {
        public:
            static Action InsertOrder(std::optional<Order> order) {
                return Action(
                    ActionType::InsertOrder,
                    order,
                    std::nullopt,
                    std::nullopt
                );
            }

            static Action CancelOrder(std::optional<std::uint64_t> order_id) {
                return Action(
                    ActionType::CancelOrder,
                    std::nullopt,
                    std::nullopt,
                    order_id
                );
            }

            static Action ExecuteOrder(std::optional<std::uint64_t> order_id) {
                return Action(
                    ActionType::ExecuteOrder,
                    std::nullopt,
                    std::nullopt,
                    order_id
                );
            }

            static Action ApplyFill(std::optional<Fill> fill, const Order &order) {
                return Action(
                    ActionType::ApplyFill,
                    order,
                    fill,
                    std::nullopt
                );
            }

            const ActionType &action_type() const;
            const Order &order() const;
            const Fill &fill() const;
            std::uint64_t order_id() const;

        private:
            Action(
                ActionType action_type,
                std::optional<Order> order,
                std::optional<Fill> fill,
                std::optional<std::uint64_t> order_id
            );

            ActionType action_type_;
            std::optional<Order> order_;
            std::optional<Fill> fill_;
            std::optional<std::uint64_t> order_id_;

    };
}