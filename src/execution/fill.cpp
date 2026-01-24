#include "execution/fill.hpp"
#include <stdexcept>

namespace bt {

    Fill::Fill(
        std::int64_t quantity, 
        double value,
        Ticker ticker)
        : quantity_(quantity),
          value_(value),
          ticker_(ticker) {
        
        if (quantity_ <= 0) {
            throw std::invalid_argument("Quantity should be a positive number");
        }

        if (value_ <= 0) {
            throw std::invalid_argument("Value should be a positive number");
        }
    }

    std::int64_t Fill::quantity() const {
        return quantity_;
    }

    double Fill::value() const {
        return value_;
    }

    const Ticker &Fill::ticker() const {
        return ticker_;
    }
}