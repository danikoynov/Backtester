/*
Bar
Purpose:
    Represents a OHLC (open, high, low, close) bar of a tradable instrument.

Invariants:
    - low <= open <= high
    - low <= close <= high
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
        
            /*
            Bar::Bar
            Purpose:
                Construct a bar given a timestamp, OHLC and volume fields.

            Errors:
                Throws std::invalid_argument if any invariant is violated.
            */
            Bar(
                Timestamp timestamp, 
                double open,
                double high,
                double low,
                double close,
                std::int64_t volume
            );

            /*
            timestamp
            Purpose:
                Returns the bar's timestamp.
            
            Invariants:
             - Returned timestamp corresponds to the open time of the bar.
            
             Errors:
                Does not throw.
            */
            Timestamp timestamp() const;

            /*
            open
            Purpose:
                Returns bar's open price.
            
            Errors:
                Does not throw.
            */
            double open() const;

            /*
            high
            Purpose:
                Returns bar's high price.
            
            Errors:
                Does not throw.
            */
            double high() const;

            /*
            low
            Purpose:
                Returns bar's low price.
            
            Errors:
                Does not throw.
            */
            double low() const;

            /*
            close
            Purpose:
                Returns bar's close price.
            
            Errors:
                Does not throw.
            */
            double close() const;

            /*
            volume
            Purpose:
                Returns bar's volume.
            
            Errors:
                Does not throw.
            */
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