#include "strategy/strategy.hpp"

namespace bt {
    Strategy::Strategy(
        const std::vector<Ticker>& tickers,
        Timeframe timeframe,
        std::function<void(
            const std::unordered_map<std::string, Bar>&, 
            const Portfolio&,
            const std::unordered_map<std::string, OrderBook>&,
            Broker&
        )> signal_function)
        : tickers_(tickers),
          timeframe_(timeframe),
          signal_function_(signal_function) {
    };

    const std::vector<Ticker> &Strategy::tickers() const{
        return tickers_;
    }

    const Timeframe &Strategy::timeframe() const {
        return timeframe_;
    }

    void Strategy::on_data(
        const std::unordered_map<std::string, Bar>& bars, 
        const Portfolio& portfolio,
        const std::unordered_map<std::string, OrderBook>& order_books,
        Broker& broker
    ) {
        signal_function_(
            bars,
            portfolio,
            order_books,
            broker            
        );
    }
}