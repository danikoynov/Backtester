#pragma once
#include "engine/action.hpp"

#include <queue>

namespace bt {

    class Broker { 
        public:
            Broker() = default;
            size_t actions_count() const;
            Action get_action();
            void submit_order(const Order& order);
            void cancel_order(std::uint64_t order_id);
            void execute_order(std::uint64_t order_id);
            void submit_fill(const Fill& fill, const Order& order);


        private:
            std::queue<Action> actions_;

    };
}