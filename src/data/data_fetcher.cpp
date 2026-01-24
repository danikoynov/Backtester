#include "data/data_fetcher.hpp"
#include "data/detail.hpp"
#include "data/bar.hpp"
#include <stdexcept>
#include <vector>
#include <cassert>
#include <fstream>
#include <charconv>

#include <filesystem>

namespace fs = std::filesystem;

namespace bt{
   
    DataFetcher::DataFetcher(
        const std::vector<Ticker>& instruments,
        Timeframe timeframe
    )
        : instruments_(instruments), timeframe_(timeframe) {
        populate_tickers();
        load_data_from_csvs();
        for (auto instrument : instruments_) {
            cursor_[instrument.str()] = 0;
        }
    }   

    void DataFetcher::populate_ticker(Ticker ticker) const{
        assert(fs::exists(script_path));
        
        std::string ticker_symbol = ticker.str();
        std::string tf = convert_timeframe_to_yf();

        std::string cmd = 
            "python " + 
            script_path +
            " --ticker " +
            ticker_symbol +
            " --timeframe " +
            tf;

        const int rc = std::system(
            cmd.c_str()
        );

        assert(rc == 0);
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
        for (Ticker ticker: instruments_) {
            read_bars_from_csv(ticker.str());
        }
    }

    const Bar &DataFetcher::get_latest_bar(const std::string &ticker) {
        return data_[ticker][cursor_.at(ticker)];
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