#include <data/bar.hpp>

namespace bt {

    using Timestamp = std::chrono::time_point<
        std::chrono::system_clock,
        std::chrono::seconds
    >;

    Bar::Bar(
        Timestamp timestamp,
        double open,
        double high,
        double low,
        double close,
        std::int64_t volume)
        : timestamp_(timestamp),
          open_(open),
          high_(high),
          low_(low),
          close_(close),
          volume_(volume) {
        
        
        if (low_ > high_) {
            throw std::invalid_argument(
                "Low price must be less than or equal to the high price");
        }

        if (low_ < 0) {
            throw std::invalid_argument("Low price must be non-negative");
        }

        if (open_ < low_ || open_ > high_) {
            throw std::invalid_argument("Open price must be in the range [low, high]");
        }

        if (close_ < low_ || close_ > high_) {
            throw std::invalid_argument("Close price must be in the range [low, high]");
        }
        
        if (volume_ < 0) {
            throw std::invalid_argument("Volume must be non-negative");
        }
    }

    Timestamp Bar::timestamp() const {
        return timestamp_;
    }

    double Bar::open() const {
        return open_;
    }

    double Bar::high() const {
        return high_;
    }

    double Bar::low() const {
        return low_;
    }

    double Bar::close() const {
        return close_;
    }

    std::int64_t Bar::volume() const {
        return volume_;
    }



}