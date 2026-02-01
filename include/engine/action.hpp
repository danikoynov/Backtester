

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

    /*
    Action
    Purpose:
        Represents an action which is to be executed by the engine.

    Invariants:
        - If type is InsertOrder:
            - order has a value
            - fill is nullopt
            - order_id is nullopt
        - If type is CancelOrder or ExecuteOrder:
            - order_id has a vlue
            - order is nullopt
            - fill is nullopt
        - If type is ApplyFill:
            - order has a value
            - fill has a value
            - order_id is nullopt
    
    Errors:
        Throws std::invalid_argument if any of the invariants is violated.
        Throws std::runtime_error if any member which is nullopt is accessed.
    */
    class Action {
        public:

            /*
            InsertOrder
            Purpose:
                Constructs an InsertOrder action.
            
            Invariants:
                - order has a value
                - fill is nullopt
                - order_id is nullopt
            
            Errors:
                Throws std::invalid_argument if any of the invariants is violated.
            */
            static Action InsertOrder(std::optional<Order> order) {
                return Action(
                    ActionType::InsertOrder,
                    order,
                    std::nullopt,
                    std::nullopt
                );
            }

            /*
            CancelOrder
            Purpose:
                Constructs an CancelOrder action.
            
            Invariants:
                - order_id has a vlue
                - order is nullopt
                - fill is nullopt
            
            Errors:
                Throws std::invalid_argument if any of the invariants is violated.
            */
            static Action CancelOrder(std::optional<std::uint64_t> order_id) {
                return Action(
                    ActionType::CancelOrder,
                    std::nullopt,
                    std::nullopt,
                    order_id
                );
            }


            /*
            ExecuteOrder
            Purpose:
                Constructs an ExecuteOrder action.
            
            Invariants:
                - order_id has a vlue
                - order is nullopt
                - fill is nullopt
            
            Errors:
                Throws std::invalid_argument if any of the invariants is violated.
            */
            static Action ExecuteOrder(std::optional<std::uint64_t> order_id) {
                return Action(
                    ActionType::ExecuteOrder,
                    std::nullopt,
                    std::nullopt,
                    order_id
                );
            }

            /*
            ApplyFill
            Purpose:
                Constructs an ApplyFill action.
            
            Invariants:
                - order has a value
                - fill has a value
                - order_id is nullopt
            
            Errors:
                Throws std::invalid_argument if any of the invariants is violated.
            */
            static Action ApplyFill(std::optional<Fill> fill, const Order &order) {
                return Action(
                    ActionType::ApplyFill,
                    order,
                    fill,
                    std::nullopt
                );
            }

            /*
            action_type
            Purpose:
                Returns the action_type of the action.
            
            Errors:
                Does not throw.
            */
            const ActionType &action_type() const;

            /*
            order
            Purpose:
                Returns the order of the action.
            
            Errors:
                Throws std::runtime_error if order is accessed but does not exist. 
            */
            const Order &order() const;

            /*
            fill
            Purpose:
                Returns the fill of the action.
            
            Errors:
                Throws std::runtime_error if fill is accessed but does not exist. 
            */
            const Fill &fill() const;

            /*
            order_id
            Purpose:
                Returns the order_id of the action.
            
            Errors:
                Throws std::runtime_error if order_id is accessed but does not exist. 
            */
            std::uint64_t order_id() const;

        private:

            /*
            Action::Action
            Purpose:
                Constructs an Action.

            Note*:
                Invariants of the class must be met.
            
            Errors:
                Throws std::invalid_argument if any of the invariants is violated.
            */
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