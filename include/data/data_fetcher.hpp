#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "data/bar.hpp"
#include "domain/timeframe.hpp"
#include "domain/ticker.hpp"

namespace bt {

    static const std::string script_path = "scripts/fetch_yf.py";
    static const std::string data_path = "data";

    class DataFetcher { 
        public:
            DataFetcher(
                const std::vector<Ticker> &instruments,
                Timeframe timeframe  
            );
            void populate_ticker(Ticker ticker) const;
            void populate_tickers() const;
            void read_bars_from_csv(const std::string& ticker); 
            void load_data_from_csvs();
            const Bar &get_latest_bar(const std::string &ticker);

        private:
            std::string convert_timeframe_to_yf() const;


            const std::vector<Ticker> &instruments_;
            Timeframe timeframe_;
            std::unordered_map<std::string, std::vector<Bar>>data_;
            std::unordered_map<std::string, int>cursor_;

    };
}