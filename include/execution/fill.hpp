#pragma once
#include <cstdint>

namespace bt{

    class Fill {
        public:
            Fill(
                std::int64_t quantity,
                double value
            );

        private:
            std::int64_t quantity_;
            double value_;
    };

}