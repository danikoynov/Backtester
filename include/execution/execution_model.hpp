#pragma once
#include "data/bar.hpp"
#include "domain/order.hpp"
#include "execution/fill.hpp"
#include <optional>

namespace bt {

    std::optional<Fill> try_fill_market(
        const Order &order,
        const Bar &bar );

    std::optional<Fill> try_fill_limit(
        const Order &order,
        const Bar &bar );

    std::optional<Order> try_activate_stop(
        const Order &order,
        const Bar &bar );

    std::optional<Order> try_activate_stop_limit(
        const Order &order,
        const Bar &bar );

    

}