#include "data/data_fetcher.hpp"
#include <stdexcept>
#include <vector>
#include <cassert>
#include <filesystem>

namespace fs = std::filesystem;
namespace bt{

    DataFetcher::DataFetcher(
        const std::vector<Ticker>& instruments,
        Timeframe timeframe
    )
        : instruments_(instruments), timeframe_(timeframe) {
        
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