#include "engine/engine.hpp"

namespace bt {

    Engine::Engine(const Strategy &strategy, double initial_value)
        : strategy_(strategy),
          data_fetcher_(DataFetcher(strategy.tickers(), strategy.timeframe())),
          portfolio_(initial_value) {
        data_fetcher_.populate_tickers();
        data_fetcher_.load_data_from_csvs();
        std::unordered_map<std::string, double> prices;
        for (auto ticker : strategy.tickers()) { /// Set initial prices
            Bar bar = data_fetcher_.next_bar(ticker.str());
            prices[ticker.str()] = bar.close();
        }

        for (auto ticker : strategy.tickers()) {
            order_books_.try_emplace(ticker.str(), OrderBook());
        }

        portfolio_.update_prices(prices);
    };


    void Engine::apply_action(const Action &action) {
        switch(action.action_type()) {
            case(ActionType::InsertOrder):
                order_book_.insert_order(action.order());
                break;

            case(ActionType::CancelOrder):
                order_book_.set_cancelled(action.order_id());   
                break;

            case(ActionType::ExecuteOrder):
                order_book_.set_executed(action.order_id());
                break;

            case(ActionType::ApplyFill):
                Fill fill = action.fill();
                Order order = action.order();
                if (order.side() == Side::Buy) {
                    portfolio_.update_position(fill.ticker().str(), fill.quantity());
                    portfolio_.update_cash(- fill.value());
                }
                else {
                    portfolio_.update_position(fill.ticker().str(), - fill.quantity());
                    portfolio_.update_cash(fill.value());
                }
                break;
        }
    }

    void Engine::apply_actions() {
        while(broker_.actions_count() != 0) {
            Action current_action = broker_.get_action();
            apply_action(current_action);
        }
    }


    void Engine::check_order_execution(const std::unordered_map<std::string, Bar>& bars) {  
        for (const Ticker& ticker : strategy.tickers()) {
            std::vector<std::pair<
                std::uint64_t, 
                std::reference_wrapper<const Order>>
            > active_orders = order_books_[ticker.str()].get_active_orders();

            for (const auto& [id, order_ref] : active_orders) {
                const Order& order = order_ref.get();
                switch(order.type()) {
                    case(OrderType::Market):
                        std::optional<Fill> fill = try_fill_market(order, bars[ticker.str()], ticker);
                        if (fill) {
                            broker_.execute_order(id);
                            broker_.submit_fill(fill.value(), order);
                        }
                        break;

                    case(OrderType::Limit):
                        std::optional<Fill> fill = try_fill_limit(order, bars[ticker.str()], ticker);
                        if (fill) {
                            broker_.execute_order(id);
                            broker_.submit_fill(fill.value(), order);
                        }
                        break;

                    case(OrderType::Stop):
                        std::optional<Order> new_order = try_activate_stop(order, bars[ticker.str()], ticker);
                        if (new_order) {
                            broker_.execute_order(id);
                            broker_.submit_order(new_order.value());
                        }
                        break;

                    case(OrderType::StopLimit):
                        std::optional<Order> new_order = try_activate_stop_limit(order, bars[ticker.str()], ticker);
                        if (new_order) {
                            broker_.execute_order(id);
                            broker_.submit_order(new_order.value());
                        }
                        break;                    
                }
            }
        }
    }

    void Engine::process_bar() { 

        std::unordered_map<std::string, Bar> bars = data_fetcher_.next_bars();
        
        check_order_execution(bars);
        
        apply_actions();

        strategy_.on_data(
            bars,
            portfolio_,
            order_books_,
            broker_
        );

        apply_actions();
    }


    void Engine::backtest() {
        while(data_fetcher_.global_cursor() < data_fetcher_.number_of_bars()) {
            process_bar();
        }
    }

    

}