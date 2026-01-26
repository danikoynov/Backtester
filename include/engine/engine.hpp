#pragma once
#include "data/data_fetcher.hpp"
#include "execution/order_book.hpp"
#include "execution/execution_model.hpp"
#include "engine/broker.hpp"
#include "strategy/strategy.hpp"
#include "engine/action.hpp"


namespace bt {


    class Engine{
        public:
            explicit Engine(const Strategy& strategy, double initial_value);
            void process_bar();
            void apply_action(const Action& action);
            void apply_actions();
            void check_order_execution(const std::unordered_map<std::string, Bar>& bars);
            void backtest();
            
        private:
            std::unordered_map<std::string, OrderBook> order_books_;
            Broker broker_;   
            Strategy strategy_;
            DataFetcher data_fetcher_;
            Portfolio portfolio_;
    };

} 
