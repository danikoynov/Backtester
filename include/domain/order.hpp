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

    /*
    Order
    Purpose:
        Represents a market, limit, stop or stop limit order.

    Invariants:
        - limit_price has a value only if type is Limit or StopLimit,
        otherwise it is nullopt.
        - stop_price has a value only if type is Stop or StopLimit,
        otherswise it is nullopt. 
        - 0 < quantity
        - 0 < limit_price
        - 0 < stop_price

    Errors:
        Throws std::invalid_argument if any of the invariants is violated.
        Throws std::runtime_error if limit_price is accessed but does not exist.
        Throws std::runtime_error if stop_price is accessed but does not exist.
    */
    class Order {
        public:

            /*
            Market
            Purpose:
                Constructs a market order.

            Invariants:
                - type == OrderType::Market
                - limit_price is nullopt
                - stop_price is nullopt
                - 0 < quantity

            Errors:
                Throws std::invalid_argument if quantity <= 0.
            */
            static Order Market(Side side, std::int64_t quantity) {
                return Order(OrderType::Market, side, quantity, std::nullopt, std::nullopt);
            }

            /*
            Limit
            Purpose:
                Constructs a limit order.

            Invariants:
                - type == OrderType::Limit
                - stop_price is nullopt
                - 0 < quantity
                - 0 < limit_price

            Errors:
                Throws std::invalid_argument if quantity <= 0.
                Throws std::invalid_argument if limit_price <= 0.
            */
            static Order Limit(Side side, std::int64_t quantity, double limit_price) {
                return Order(OrderType::Limit, side, quantity, limit_price, std::nullopt);
            }
            
            /*
            Limit
            Purpose:
                Constructs a stop order.

            Invariants:
                - type == OrderType::Stop
                - limit_price is nullopt
                - 0 < quantity
                - 0 < stop_price

            Errors:
                Throws std::invalid_argument if quantity <= 0.
                Throws std::invalid_argument if stop_price <= 0.
            */ 
            static Order Stop(Side side, std::int64_t quantity, double stop_price) {
                return Order(OrderType::Stop, side, quantity, std::nullopt, stop_price);
            }
           
            /*
            StopLimit
            Purpose:
                Constructs a stop order.

            Invariants:
                - type == OrderType::StopLimit
                - 0 < quantity
                - 0 < limit_price
                - 0 < stop_price

            Errors:
                Throws std::invalid_argument if quantity <= 0.
                Throws std::invalid_argument if stop_price <= 0.
                Throws std::invalid_argument if stop_limit <= 0.
            */ 
            static Order StopLimit(Side side, std::int64_t quantity, double limit_price, double stop_price) {
                return Order(OrderType::StopLimit, side, quantity, limit_price, stop_price);
            }

            /*
            type
            Purpose:
                Returns the type of the order.
            
            Errors:
                Does not throw.
            */
            OrderType type() const;

            /*
            side
            Purpose:
                Returns the side of the order.
            
            Errors:
                Does not throw.
            */ 
            Side side() const;

            /*
            quantity
            Purpose:
                Returns the quantity of the order.
            
            Errors:
                Does not throw.
            */
            std::int64_t quantity() const;

            /*
            limit_price
            Purpose:
                Returns the limit_price of the order.
            
            Errors:
                Throws std::runtime_error if limit_price is accessed but does not exist. 
            */
            double limit_price() const;

            /*
            stop_price
            Purpose:
                Returns the stop_price of the order.
            
            Errors:
                Throws std::runtime_error if stop_price is accessed but does not exist. 
            */
            double stop_price() const;
    
        private:

            /*
            Order::Order
            Purpose:
                Construct an Order with explicit type, side, quantity and optional prices.

            Invariants:
                - 0 < quantity
                - If type is Market:
                    - limit_price is nullopt
                    - stop_price  is nullopt
                - If type is Limit:
                    - limit_price has a value and 0 < limit_price
                    - stop_price  is nullopt
                - If type is Stop:
                    - limit_price is nullopt
                    - stop_price  has a value and 0 < stop_price
                - If type is StopLimit:
                    - limit_price has a value and 0 < limit_price
                    - stop_price  has a value and 0 < stop_price

            Errors:
                Throws std::invalid_argument if any invariant is violated.
            */
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