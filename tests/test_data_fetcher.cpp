#include <gtest/gtest.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include "data/data_fetcher.hpp"

using namespace bt;

TEST(LineSplit, LineSplitsByCommaSuccessfully) {
    std::string line = "2025-11-19 17:25:00+00:00,270.3999938964844,270.5199890136719,269.94000244140625,270.0799865722656,163138";
    std::vector<std::string> result = DataFetcher::split_csv_line(line);
    EXPECT_EQ("2025-11-19 17:25:00+00:00", result[0]);
    EXPECT_EQ("270.3999938964844", result[1]);
    EXPECT_EQ("270.5199890136719", result[2]);
    EXPECT_EQ("269.94000244140625", result[3]);
    EXPECT_EQ("270.0799865722656", result[4]);
    EXPECT_EQ("163138", result[5]);
}


/**
TEST(DataFetch, DataFetchesSuccessfully) {
    std::vector < Ticker > tickers = {
        Ticker("AAPL"),
        Ticker("NVDA")
    };

    DataFetcher df = DataFetcher(tickers, Timeframe::Minute5);

    EXPECT_NO_THROW(df.populate_tickers());
}
*/