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
            Order(OrderType type,
                  Side side,
                  std::int64_t quantity,
                  std::optional<double> limit_price,
                  std::optional<double> stop_price);
    
        private:
            OrderType type_;
            Side side_;
            std::int64_t quantity_;
            std::optional<double> limit_price_;
            std::optional<double> stop_price_;
    };

}