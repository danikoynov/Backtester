#pragma once
#include <optional>
#include <cstdint>

namespace bt {

    enum class OrderType {
        Market,
        Limit,
        Stop,
        StopLimit
    };

    enum class Side {
        Buy,
        Sell
    };

    class Order {
        public:
            static Order Market(Side side, std::int64_t quantity) {
                return Order(OrderType::Market, side, quantity, std::nullopt, std::nullopt);
            }
            static Order Limit(Side side, std::int64_t quantity, double limit_price) {
                return Order(OrderType::Limit, side, quantity, limit_price, std::nullopt);
            }
            static Order Stop(Side side, std::int64_t quantity, double stop_price) {
                return Order(OrderType::Stop, side, quantity, std::nullopt, stop_price);
            }
            static Order StopLimit(Side side, std::int64_t quantity, double limit_price, double stop_price) {
                return Order(OrderType::StopLimit, side, quantity, limit_price, stop_price);
            }

            OrderType type() const;
            Side side() const;
            std::int64_t quantity() const;
            double limit_price() const;
            double stop_price() const;
    
        private:
            Order(OrderType type,
                  Side side,
                  std::int64_t quantity,
                  std::optional<double> limit_price = std::nullopt,
                  std::optional<double> stop_price = std::nullopt);
            OrderType type_;
            Side side_;
            std::int64_t quantity_;
            std::optional<double> limit_price_;
            std::optional<double> stop_price_;
    };

}