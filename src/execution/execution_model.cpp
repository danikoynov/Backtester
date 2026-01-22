#include "execution/fill.hpp"
#include "data/bar.hpp"
#include "domain/order.hpp"

/*
Assumptions of the model:
    1. The user does not have enough volume to influence the 
market
    2. Partial fills do not occur (again because user's volume
is negligable w.r.t. the market) 
    3. Market orders are filled at the open price of the bar

*/
namespace bt {

    std::optional<Fill> try_fill_market(
        const Order &order,
        const Bar &bar) {

        std::int64_t quantity = order.quantity();
        double value = static_cast<double>(quantity) * bar.open();
        
        return Fill(quantity, value);
    }

    std::optional<Fill> try_fill_limit(
        const Order &order,
        const Bar &bar) {

        if (order.side() == Side::Buy) {
            if (bar.low() <= order.limit_price()) {
                std::int64_t quantity = order.quantity();
                double value = static_cast<double>(quantity) * order.limit_price(); 
                return Fill(quantity, quantity);
            }
        }
        else {
            if (bar.high() >= order.limit_price()) {
                std::int64_t quantity = order.quantity();
                double value = static_cast<double>(quantity) * order.limit_price(); 
                return Fill(quantity, quantity);
            }
        }

        return std::nullopt;
    }

    std::optional<Order> try_activate_stop(
        const Order &order,
        const Bar &bar) {
        
        if (order.side() == Side::Buy) {
            if (bar.high() >= order.stop_price()) {
                return Order::Market(Side::Buy, order.quantity());
            }
        }
        else {
            if (bar.low() <= order.stop_price()) {
                return Order::Market(Side::Sell, order.quantity());
            }
        }

        return std::nullopt;
    }


    std::optional<Order> try_activate_stop_limit(
        const Order &order,
        const Bar &bar) {
        
        if (order.side() == Side::Buy) {
            if (bar.high() >= order.stop_price()) {
                return Order::Limit(Side::Buy, order.quantity(), order.limit_price());
            }
        }
        else {
            if (bar.low() <= order.stop_price()) {
                return Order::Limit(Side::Sell, order.quantity(), order.limit_price());
            }
        }

        return std::nullopt;
    }
}