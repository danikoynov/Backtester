/*
Bar
Purpose:
    Represents a OHLC (open, high, low, close) bar of a tradable instrument.

Invariants:
    - low <= open <= high
    - low <= close <= high
    - low <= high
    - 0 <= low
    - 0 <= volume

Errors:
    Throws invalid argument if one of the invariants is violated.
*/

#pragma once
#include <chrono>
#include <cstdint>


namespace bt {

    using Timestamp = std::chrono::time_point<
        std::chrono::system_clock,
        std::chrono::seconds
    >;

    class Bar {
        public:
            Bar(
                Timestamp timestamp, 
                double open,
                double high,
                double low,
                double close,
                std::int64_t volume
            );

            Timestamp timestamp() const;
            double open() const;
            double high() const;
            double low() const;
            double close() const;
            std::int64_t volume() const;

        private:
            Timestamp timestamp_;
            double open_;
            double high_;
            double low_;
            double close_;
            std::int64_t volume_;

    };
}