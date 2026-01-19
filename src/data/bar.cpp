#include <data/bar.hpp>
#include <chrono>



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