#pragma once
#include <chrono>


namespace bt {

    using Timestamp = std::chrono::time_point<
        std::chrono::system_clock,
        std::chrono::seconds
    >;

    class Bar {
        public:
            Bar(
                Timestamp timestamp, 
                double open,
                double high,
                double low,
                double close,
                std::int64_t volume
            );

            Timestamp timestamp() const;
            double open() const;
            double high() const;
            double low() const;
            double close() const;
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