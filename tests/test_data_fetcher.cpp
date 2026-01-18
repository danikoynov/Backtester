#include <gtest/gtest.h>
#include <iostream>
#include <cstdlib>
#include "data/data_fetcher.hpp"

using namespace bt;

TEST(DataFetch, DataFetchesSuccessfully) {
    std::vector < Ticker > tickers = {
        Ticker("AAPL"),
        Ticker("NVDA")
    };

    DataFetcher df = DataFetcher(tickers, Timeframe::Minute5);

    EXPECT_NO_THROW(df.populate_tickers());
}