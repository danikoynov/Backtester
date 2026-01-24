#include "domain/portfolio.hpp"
#include <stdexcept>
#include <cstdint>

namespace bt {

    Portfolio::Portfolio(double initial_value, 
                         const std::unordered_map<std::string, double> &initial_prices)
        : initial_value_(initial_value), 
          cash_(initial_value), 
          instrument_price_(initial_prices){    
        
        if (initial_value < 0) {
            throw std::invalid_argument("Initial value should be non-negative");
        }

        for (auto it = initial_prices.begin(); it != initial_prices.end(); ++ it) {
            holding_size_[it -> first] = 0; /// initial position is empty
        }
    }

    Portfolio::Portfolio(double initial_value) 
        : initial_value_(initial_value) {
    }

    double Portfolio::initial_value() const {
        return initial_value_;
    }

    double Portfolio::current_value() const {
        double current_value = cash_;
        
        for (auto it = holding_size_.begin(); it != holding_size_.end(); ++it) {
            double price = instrument_price_.at(it -> first);
            std::int64_t quantity = it -> second;
            current_value += price * static_cast<double>(quantity);
        }

        return current_value;
    }

    double Portfolio::pnl() const {
        return current_value() - initial_value_;
    }


    void Portfolio::update_price(const std::string &symbol, double latest_price) {
        if (latest_price < 0) {
            throw std::invalid_argument("Latest price should be non-negative");
        }

        instrument_price_.at(symbol) = latest_price;
    }    

    void Portfolio::update_prices(const std::unordered_map<std::string, double> &latest_prices) {
        for (auto it = latest_prices.begin(); it != latest_prices.end(); ++it) { 
            update_price(it -> first, it -> second);
        }
    }

    void Portfolio::update_position(const std::string &symbol, std::int64_t quantity) {
        double price = instrument_price_.at(symbol);
        
        if (cash_ - price * static_cast<double>(quantity) < 0) {
            throw std::runtime_error("Not enough cash to perform transaction");
        }

        cash_ -= price * static_cast<double>(quantity);
        holding_size_.at(symbol) += quantity;
    }

    void Portfolio::update_cash(double delta) {
        cash_ += delta;
    }

}