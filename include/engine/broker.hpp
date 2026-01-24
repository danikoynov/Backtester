#pragma once
#include "engine/action.hpp"

#include <queue>

namespace bt {

    class Broker { 
        public:
            Broker() = default;
            size_t actions_count() const;
            const Action &get_action();
            void submit_order(const Order &order);
            void cancel_order(std::uint64_t order_id);
            void execute_order(std::uint64_t order_id);
            void submit_fill(const Fill &fill);


        private:
            std::queue<Action> actions_;

    };
}