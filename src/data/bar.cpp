#include <data/bar.hpp>
#include <stdexcept>

namespace bt {

    using Timestamp = std::chrono::time_point<
        std::chrono::system_clock,
        std::chrono::seconds
    >;

    namespace {
        constexpr double kEps = 1e-9;

        inline bool leq_eps(double a, double b) {
            return a <= b + kEps;
        }

        inline bool geq_eps(double a, double b) {
            return a + kEps >= b;
        }

        inline bool in_range_eps(double x, double lo, double hi) {
            return geq_eps(x, lo) && leq_eps(x, hi);
        }
    }

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

        if (!leq_eps(low_, high_)) {
            throw std::invalid_argument(
                "Low price must be less than or equal to the high price");
        }

        if (low_ < -kEps) {
            throw std::invalid_argument("Low price must be non-negative");
        }

        if (!in_range_eps(open_, low_, high_)) {
            throw std::invalid_argument("Open price must be in the range [low, high]");
        }

        if (!in_range_eps(close_, low_, high_)) {
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
