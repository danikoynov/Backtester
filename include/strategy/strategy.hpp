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
                    const std::vector<Bar>&, 
                    const Portfolio&,
                    const std::vector<OrderBook>&,
                    Broker&
                )> signal_function
            );
            void on_data(
                const std::vector<Bar>&, 
                const Portfolio&,
                const std::vector<OrderBook>&,
                Broker&
            );
            const std::vector<Ticker> &tickers() const;
            const Timeframe &timeframe() const;
        private:
            std::function<void(
                const std::vector<Bar>&, 
                const Portfolio&,
                const std::vector<OrderBook>&,
                Broker&
            )> signal_function_;
            std::vector<Ticker> tickers_;
            Timeframe timeframe_;
    };
}