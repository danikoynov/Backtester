#pragma once
#include "domain/ticker.hpp"
#include <cstdint>

namespace bt {

    /*
    Fill

    Purpose:
        Represents the execution result of an Order.
        Encapsulates the executed quantity, total traded value, and the traded instrument.

    Invariants:
        - quantity > 0
        - value > 0
        - ticker identifies the traded instrument and is immutable after construction.

    Errors:
        - Throws std::invalid_argument if:
            * quantity <= 0
            * value <= 0
    */
    class Fill {
        public:

            /*
            Fill (constructor)

            Purpose:
                Constructs a Fill representing a fully executed order.

            Invariants:
                - quantity > 0
                - value > 0

            Errors:
                - Throws std::invalid_argument if:
                    * quantity <= 0
                    * value <= 0
            */
            Fill(std::int64_t quantity, double value, Ticker ticker);

            /*
            quantity

            Purpose:
                Returns the executed quantity.

            Errors:
                - Does not throw.
            */
            std::int64_t quantity() const;

            /*
            value

            Purpose:
                Returns the total traded value of the fill.

            Errors:
                - Does not throw.
            */
            double value() const;

            /*
            ticker

            Purpose:
                Returns the ticker of the traded instrument.

            Errors:
                - Does not throw.
            */
            const Ticker& ticker() const;

        private:
            std::int64_t quantity_;
            double value_;
            Ticker ticker_;
    };

}
