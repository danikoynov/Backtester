#include <gtest/gtest.h>
#include "domain/ticker.hpp"

using bt::Ticker;

TEST(TickerTest, ConstructsWithValidString) {
    Ticker t("AAPL");
    EXPECT_EQ(t.str(), "AAPL");
}

TEST(TickerTest, StoresExactStringIncludingCase) {
    Ticker t("aApL");
    EXPECT_EQ(t.str(), "aApL");
}

TEST(TickerTest, AllowsEmptyStringForNow) {
    Ticker t("");
    EXPECT_TRUE(t.str().empty());
}

TEST(TickerTest, StrReturnsSameReference) {
    Ticker t("MSFT");
    const std::string& ref1 = t.str();
    const std::string& ref2 = t.str();

    EXPECT_EQ(&ref1, &ref2);
}
