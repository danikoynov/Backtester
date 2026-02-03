#pragma once
#include <vector>
#include <string>
#include <functional>
#include "domain/timeframe.hpp"
#include "domain/portfolio.hpp"
#include "domain/ticker.hpp"
#include "execution/order_book.hpp"
#include "engine/Broker.hpp"
#include "data/bar.hpp"

namespace bt {

    class Strategy {
        public:
            Strategy(
                const std::vector<Ticker>& tickers,
                Timeframe timeframe,
                std::function<void(
                    const std::unordered_map<std::string, Bar>&, 
                    const Portfolio&,
                    const std::unordered_map<std::string, OrderBook>&,
                    Broker&
                )> signal_function
            );
            void on_data(
                const std::unordered_map<std::string, Bar>&, 
                const Portfolio&,
                const std::unordered_map<std::string, OrderBook>&,
                Broker&
            );
            const std::vector<Ticker> &tickers() const;
            const Timeframe &timeframe() const;
        private:
            std::function<void(
                const std::unordered_map<std::string, Bar>&, 
                const Portfolio&,
                const std::unordered_map<std::string, OrderBook>&,
                Broker&
            )> signal_function_;
            std::vector<Ticker> tickers_;
            Timeframe timeframe_;
    };
}