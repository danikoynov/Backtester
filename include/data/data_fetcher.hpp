#pragma once
#include <vector>
#include <string>
#include "domain/timeframe.hpp"
#include "domain/ticker.hpp"

namespace bt {
    static const std::string script_path = "scripts/fetch_yf.py";

    class DataFetcher { 
        public:
            DataFetcher(
                const std::vector<Ticker> &instruments,
                Timeframe timeframe  
            );
            void populate_ticker(Ticker ticker) const;
            void populate_tickers() const;
            static std::vector<std::string> split_csv_line(const std::string &line);
        
        private:
            std::string convert_timeframe_to_yf() const;


            const std::vector<Ticker> &instruments_;
            Timeframe timeframe_;

    };
}