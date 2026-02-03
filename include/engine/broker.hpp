#pragma once
#include "engine/action.hpp"
#include "domain/ticker.hpp"
#include <queue>

namespace bt {

    /*
    Broker
    Purpose:
        Collects actions requested by strategies or other components and
        exposes them to the engine in FIFO order.

    Invariants:
        - actions_ stores actions in the order they were submitted.
        - actions_ is modified only through submit_* methods and get_action().

    Errors:
        Throws std::runtime_error if get_action() is called when no actions exist.
    */
    class Broker { 
        public:
            Broker() = default;

            /*
            actions_count
            Purpose:
                Returns the number of pending actions in the broker.

            Errors:
                Does not throw.
            */
            size_t actions_count() const;

            /*
            get_action
            Purpose:
                Returns and removes the next action from the broker.

            Invariants:
                - Actions are returned in FIFO order.

            Errors:
                Throws std::runtime_error if the broker contains no actions.
            */
            std::pair<Ticker, Action> get_action();

            /*
            submit_order
            Purpose:
                Submits a request to insert an order into the engine.

            Invariants:
                - Exactly one InsertOrder action is added to the queue.

            Errors:
                Propagates any exception thrown by Action::InsertOrder.
            */
            void submit_order(const Order& order, const Ticker& ticker);

            /*
            cancel_order
            Purpose:
                Submits a request to cancel an existing order.

            Invariants:
                - Exactly one CancelOrder action is added to the queue.

            Errors:
                Propagates any exception thrown by Action::CancelOrder.
            */
            void cancel_order(std::uint64_t order_id, const Ticker& ticker);

            /*
            execute_order
            Purpose:
                Submits a request to execute an existing order.

            Invariants:
                - Exactly one ExecuteOrder action is added to the queue.

            Errors:
                Propagates any exception thrown by Action::ExecuteOrder.
            */
            void execute_order(std::uint64_t order_id, const Ticker& ticker);

            /*
            submit_fill
            Purpose:
                Submits a request to apply a fill to an order.

            Invariants:
                - Exactly one ApplyFill action is added to the queue.

            Errors:
                Propagates any exception thrown by Action::ApplyFill.
            */
            void submit_fill(const Fill& fill, const Order& order, const Ticker& ticker);

        private:
            std::queue<std::pair<Ticker, Action>> actions_;
    };
}
