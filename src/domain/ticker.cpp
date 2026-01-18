#include <string>

#include "domain/ticker.hpp"

namespace bt{

    Ticker::Ticker(std::string ticker)
        : ticker_(std::move(ticker)) {
        /// Potentailly add validation
    }

    const std::string& Ticker::str() const noexcept {
        return ticker_;
    } 
}