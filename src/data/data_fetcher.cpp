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


#include "data/data_fetcher.hpp"
#include "data/detail.hpp"
#include "data/bar.hpp"
#include <stdexcept>
#include <vector>
#include <cassert>
#include <fstream>
#include <charconv>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

namespace bt{
   
    DataFetcher::DataFetcher(
        const std::vector<Ticker>& instruments,
        Timeframe timeframe
    )
        : instruments_(instruments), timeframe_(timeframe) {
        std::cout<<"CONSTRUCTOR"<<std::endl;
        populate_tickers();
        load_data_from_csvs();
    }   

    void DataFetcher::populate_ticker(Ticker ticker) const{
        std::cout<<"Populate ticker: " << ticker.str()<<std::endl;
        if (fs::exists(script_path) == false) {
            throw std::runtime_error("Fetch script is not found.");
        }
        
        std::string ticker_symbol = ticker.str();
        std::string tf = convert_timeframe_to_yf();

        std::string cmd = 
            "python " + 
            script_path +
            " --ticker " +
            ticker_symbol +
            " --timeframe " +
            tf;
        std::cout << "CWD: " << fs::current_path().string() << "\n";
        std::cout<<"System path: " << cmd.c_str() << std::endl;
        const int rc = std::system(
            cmd.c_str()
        );

        if (rc != 0) {
            throw std::runtime_error("Return code from fetch script is non-zero.");
        }
    }

    void DataFetcher::populate_tickers() const {
        for (auto ticker: instruments_) {
            populate_ticker(ticker);
        }
    }

    void DataFetcher::read_bars_from_csv(const std::string& ticker) {

        std::string file_path = data_path + "/" + ticker + "/" + convert_timeframe_to_yf();
        std::ifstream file(file_path);

        if (!file.is_open())
            throw std::runtime_error("Failed to open CSV file: " + file_path);

        std::string line;
        std::getline(file, line); /// Header line
        
        while(std::getline(file, line)) {
            std::vector<std::string> res = split_csv_line(line);

            Timestamp ts = parse_timestamp_seconds(res[0]);
            double open = parse_double(res[1]);
            double high = parse_double(res[2]);
            double low = parse_double(res[3]);
            double close = parse_double(res[4]);
            std::int64_t volume = parse_int64_t(res[5]);
            
            Bar bar(ts, open, high, low, close, volume);
            data_[ticker].push_back(bar);
        }   
    }

    void DataFetcher::load_data_from_csvs() {
        bool loaded_ticker = false;
        for (Ticker ticker: instruments_) {
            read_bars_from_csv(ticker.str());

            if (loaded_ticker && data_[ticker.str()].size() != number_of_bars_) {
                throw std::runtime_error("Instruments have different amount of bars"); 
            }

            number_of_bars_ = static_cast<std::uint32_t>(data_[ticker.str()].size());
            loaded_ticker = true;
        }
    }

    const Bar& DataFetcher::next_bar(const std::string &ticker) {
        if (global_cursor_ >= number_of_bars_) [[unlikely]] {
            throw std::runtime_error("Global cursor is out of range.");
        }
        return data_.at(ticker)[global_cursor_];
    }
    
    std::unordered_map<std::string, Bar> DataFetcher::next_bars() {
        std::unordered_map<std::string, Bar> bars;
        for (const Ticker& ticker : instruments_) { 
            bars.try_emplace(ticker.str(), next_bar(ticker.str()));
        }
        global_cursor_ ++;
        return bars;
    }

    std::uint32_t DataFetcher::global_cursor() const {
        return global_cursor_;
    }

    std::uint32_t DataFetcher::number_of_bars() const {
        return number_of_bars_;
    }
    
    std::string DataFetcher::convert_timeframe_to_yf() const{
        switch (timeframe_) {
            case Timeframe::Minute1:
                return "1m";

            case Timeframe::Minute2:
                return "2m";

            case Timeframe::Minute5:
                return "5m";

            case Timeframe::Minute15:
                return "15m";

            case Timeframe::Minute30:
                return "30m";

            case Timeframe::Minute60:
                return "60m";

            case Timeframe::Hour1:
                return "1h";

            case Timeframe::Minute90:
                return "90m";

            case Timeframe::Day1:
                return "1d";

            case Timeframe::Day5:
                return "5d";

            case Timeframe::Week1:
                return "1wk";

            case Timeframe::Month1:
                return "1mo";

            case Timeframe::Month3:
                return "3mo";
        }
        throw std::runtime_error("Conversion to yfinance timeframe unsuccessful");
    }

}