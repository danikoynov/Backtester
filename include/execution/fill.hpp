#pragma once
#include "domain/ticker.hpp"
#include <cstdint>

namespace bt{

    class Fill {
        public:
            Fill(
                std::int64_t quantity,
                double value,
                Ticker ticker
            );
            std::int64_t quantity() const;
            double value() const;
            const Ticker &ticker() const;

        private:
            std::int64_t quantity_;
            double value_;
            Ticker ticker_;
    };

}