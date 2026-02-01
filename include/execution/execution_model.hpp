#pragma once
#include "data/bar.hpp"
#include "domain/order.hpp"
#include "execution/fill.hpp"
#include <optional>

namespace bt {

    /*
    try_fill_market

    Purpose:
        Attempts to fill a Market order using a single OHLC bar.

    Invariants:
        - Assumes no price impact and no partial fills.
        - If called, the order is assumed to be of type Market.
        - The fill price is bar.open().

    Errors:
        - Propagates errors from Order and Fill
    */
    std::optional<Fill> try_fill_market(
        const Order& order,
        const Bar& bar,
        const Ticker& ticker);


    /*
    try_fill_limit

    Purpose:
        Attempts to fill a Limit order using a single OHLC bar.

    Invariants:
        - Assumes no price impact and no partial fills.
        - If called, the order is assumed to be of type Limit.
        - Buy limit fills if bar.low() <= order.limit_price().
        - Sell limit fills if bar.high() >= order.limit_price().
        - Fill price is exactly order.limit_price().

    Errors:
        - Propagates errors from Order and Fill
    */
    std::optional<Fill> try_fill_limit(
        const Order& order,
        const Bar& bar,
        const Ticker& ticker);


    /*
    try_activate_stop

    Purpose:
        Attempts to activate a Stop order into a Market order using a single OHLC bar.

    Invariants:
        - If called, the order is assumed to be of type Stop.
        - Buy stop triggers iff bar.high() >= order.stop_price().
        - Sell stop triggers iff bar.low() <= order.stop_price().

    Errors:
        - Propagates errors from Order
    */
    std::optional<Order> try_activate_stop(
        const Order& order,
        const Bar& bar,
        const Ticker& ticker);


    /*
    try_activate_stop_limit

    Purpose:
        Attempts to activate a Stop-Limit order into a Limit order using a single OHLC bar.

    Invariants:
        - If called, the order is assumed to be of type StopLimit.
        - Buy stop-limit triggers iff bar.high() >= order.stop_price().
        - Sell stop-limit triggers iff bar.low() <= order.stop_price().

    Errors:
        - Propagates errors from Order
    */
    std::optional<Order> try_activate_stop_limit(
        const Order& order,
        const Bar& bar,
        const Ticker& ticker);

}
