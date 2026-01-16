#include "order.hpp"
#include <stdexcept>
#include <cstdint>

namespace bt {

    Order::Order(OrderType type,
                 Side side,
                 std::int64_t quantity,
                 std::optional<double> limit_price,
                 std::optional<double> stop_price)
            : type_(type),
              side_(side),
              quantity_(quantity),
              limit_price_(limit_price),
              stop_price_(stop_price)
    {
        if (quantity <= 0) {
            throw std::invalid_argument("Order quantity should be positive");
        }

        switch(type_) {
            case OrderType::Market:
                if (limit_price_ || stop_price_) {
                    throw std::invalid_argument(
                        "Market order must not have limit or stop price"
                    );
                }
                break;
            
            case OrderType::Limit:
                if (!limit_price_ || stop_price_) {
                    throw std::invalid_argument(
                        "Limit order requires limit price only"
                    );
                }
                break;
            
            case OrderType::Stop:
                if (limit_price_ || !stop_price_) {
                    throw std::invalid_argument(
                        "Stop order requires stop price only"
                    );
                }
                break;
            
            case OrderType::StopLimit:
                if (!limit_price_ || !stop_price_) {
                    throw std::invalid_argument(
                        "StopLimit order requires both limit and stop price"
                    );
                }
                break;
        }

        if (limit_price_ && *limit_price <= 0.0) {
            throw std::invalid_argument("Limit price must be positive");
        }

        if (stop_price_ && *stop_price <= 0) {
            throw std::invalid_argument("Stop price must be positive");
        }
    }

}