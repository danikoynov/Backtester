#include "engine/engine.hpp"

namespace bt {

    Engine::Engine(const Strategy &strategy, double initial_value)
        : strategy_(strategy),
          data_fetcher_(DataFetcher(strategy.tickers(), strategy.timeframe())),
          portfolio_(initial_value) {
        data_fetcher_.populate_tickers();
        data_fetcher_.load_data_from_csvs();
        std::unordered_map<std::string, double> prices;
        for (auto ticker : strategy.tickers()) {
            Bar bar = data_fetcher_.get_latest_bar(ticker.str());
            prices[ticker.str()] = bar.close();
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

    

}