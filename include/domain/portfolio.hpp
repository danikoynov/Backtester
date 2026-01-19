#pragma once
#include <unordered_map>
#include <string>
#include <cstdint>

namespace bt {

    class Portfolio {
        public:
            Portfolio(double initial_value, 
                      const std::unordered_map<std::string, double> &initial_prices);
            double initial_value() const;
            double current_value() const;
            double pnl() const;
            void update_price(const std::string &symbol, double latest_price);
            void update_prices(const std::unordered_map<std::string, double> &latest_prices);
            void update_position(const std::string &symbol, int64_t quantity);

        private:
            const double initial_value_;
            double cash_;
            std::unordered_map<std::string, std::int64_t> holding_size_;
            std::unordered_map<std::string, double> instrument_price_;

    };
}