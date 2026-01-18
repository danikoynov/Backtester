#pragma once
#include <string>
#include <utility>

namespace bt {

    class Ticker {
        public:
            explicit Ticker(std::string ticker);
            const std::string& str() const noexcept;

        private:
            std::string ticker_;    
    };

}