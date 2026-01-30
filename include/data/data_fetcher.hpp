/*
DataFetcher
Purpose:
    Fetch OHLCV data for a set of tickers using a Python yfinance script,
    load CSVs into memory, and serve bars sequentially via a global cursor.

Invariants:
    - instruments_ is stored by reference and must outlive the DataFetcher.
    - number_of_bars_ equals the bar count per ticker (all tickers must match).
    - global_cursor_ is the current index used for all tickers.

Errors:
    Throws std::runtime_error on script/file/consistency/range failures.
    Propagates std::invalid_argument from parsing helpers and Bar.
*/

/*
DataFetcher::DataFetcher
Purpose:
    Construct a fetcher for instruments at a given timeframe.
    Runs the fetch script(s) and loads CSV data into memory.

Errors:
    Propagates exceptions from populate_tickers() and load_data_from_csvs().
*/

/*
populate_ticker
Purpose:
    Run the Python fetch script for one ticker and timeframe.

Errors:
    Throws std::runtime_error if:
        - scripts/fetch_yf.py does not exist.
        - system() returns a non-zero return code.
*/

/*
populate_tickers
Purpose:
    Run the fetch script for all instruments.

Errors:
    Propagates exceptions from populate_ticker().
*/

/*
read_bars_from_csv
Purpose:
    Read one ticker's CSV file and append Bars into data_[ticker].

Invariants:
    - Each row must produce a valid Bar (Bar invariants apply).

Errors:
    Throws std::runtime_error if the CSV file cannot be opened.
    Propagates std::invalid_argument from parse_* helpers and Bar.
*/

/*
load_data_from_csvs
Purpose:
    Load CSV data for all instruments and set number_of_bars_.

Invariants:
    - All instruments must have the same bar count.

Errors:
    Throws std::runtime_error if instruments have different amount of bars.
    Propagates exceptions from read_bars_from_csv().
*/

/*
next_bar
Purpose:
    Return the bar at the current global_cursor_ for a given ticker.

Invariants:
    - global_cursor_ < number_of_bars_ for valid access.

Errors:
    Throws std::runtime_error if global_cursor_ is out of range.
    Throws std::out_of_range if ticker is not present in loaded data (data_.at).
*/

/*
next_bars
Purpose:
    Return a map (ticker -> bar at global_cursor_) for all instruments and
    advance global_cursor_ by one.

Invariants:
    - global_cursor_ increments by 1 per call.

Errors:
    Propagates exceptions from next_bar().
*/

/*
global_cursor
Purpose:
    Return the current global cursor position.

Errors:
    Does not throw.
*/

/*
number_of_bars
Purpose:
    Return the number of bars loaded per ticker.

Errors:
    Does not throw.
*/

/*
convert_timeframe_to_yf
Purpose:
    Convert Timeframe enum to yfinance interval string.

Errors:
    Throws std::runtime_error if the timeframe value is not handled.
*/


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
            const Bar& next_bar(const std::string &ticker);
            std::unordered_map<std::string, Bar> next_bars();
            std::uint32_t global_cursor() const;
            std::uint32_t number_of_bars() const;

        private:
            std::string convert_timeframe_to_yf() const;

            std::uint32_t number_of_bars_;
            const std::vector<Ticker> &instruments_;
            Timeframe timeframe_;
            std::unordered_map<std::string, std::vector<Bar>> data_;
            std::uint32_t global_cursor_ = 0;

    };
}