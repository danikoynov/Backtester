#include "execution/fill.hpp"
#include <stdexcept>

namespace bt {

    Fill::Fill(
        std::int64_t quantity, 
        double value)
        : quantity_(quantity),
          value_(value) {
        
        if (quantity_ <= 0) {
            throw std::invalid_argument("Quantity should be a positive number");
        }

        if (value_ <= 0) {
            throw std::invalid_argument("Value should be a positive number");
        }
    }
}