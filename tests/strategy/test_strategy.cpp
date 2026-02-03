#include <gtest/gtest.h>

#include <unordered_map>
#include <string>
#include <vector>

#include "strategy/strategy.hpp"
#include "engine/broker.hpp"
#include "domain/portfolio.hpp"
#include "domain/ticker.hpp"
#include "domain/timeframe.hpp"
#include "execution/order_book.hpp"
#include "data/bar.hpp"
#include "domain/order.hpp"

using namespace bt;

TEST(StrategyTest, StoresTickersAndTimeframe) {
    std::vector<Ticker> tickers{Ticker("AAPL"), Ticker("MSFT")};
    Timeframe tf{};

    Strategy s(
        tickers,
        tf,
        [](const std::unordered_map<std::string, Bar>&,
           const Portfolio&,
           const std::unordered_map<std::string, OrderBook>&,
           Broker&) {}
    );

    ASSERT_EQ(s.tickers().size(), 2u);
    EXPECT_EQ(s.tickers()[0].str(), "AAPL");
    EXPECT_EQ(s.tickers()[1].str(), "MSFT");
    EXPECT_EQ(s.timeframe(), tf);
}

TEST(StrategyTest, OnDataInvokesSignalFunction) {
    std::vector<Ticker> tickers{Ticker("AAPL")};
    Timeframe tf{};

    bool called = false;

    Strategy s(
        tickers,
        tf,
        [&](const std::unordered_map<std::string, Bar>&,
            const Portfolio&,
            const std::unordered_map<std::string, OrderBook>&,
            Broker&) {
            called = true;
        }
    );

    std::unordered_map<std::string, Bar> bars;
    Portfolio portfolio(1000.0);
    std::unordered_map<std::string, OrderBook> order_books;
    Broker broker;

    s.on_data(bars, portfolio, order_books, broker);
    EXPECT_TRUE(called);
}

TEST(StrategyTest, OnDataForwardsReferencesExactly) {
    std::vector<Ticker> tickers{Ticker("AAPL")};
    Timeframe tf{};

    std::unordered_map<std::string, Bar> bars;
    Portfolio portfolio(1000.0);
    std::unordered_map<std::string, OrderBook> order_books;
    Broker broker;

    const std::unordered_map<std::string, Bar>* bars_ptr = nullptr;
    const Portfolio* portfolio_ptr = nullptr;
    const std::unordered_map<std::string, OrderBook>* books_ptr = nullptr;
    Broker* broker_ptr = nullptr;

    Strategy s(
        tickers,
        tf,
        [&](const std::unordered_map<std::string, Bar>& b,
            const Portfolio& p,
            const std::unordered_map<std::string, OrderBook>& ob,
            Broker& br) {
            bars_ptr = &b;
            portfolio_ptr = &p;
            books_ptr = &ob;
            broker_ptr = &br;
        }
    );

    s.on_data(bars, portfolio, order_books, broker);

    EXPECT_EQ(bars_ptr, &bars);
    EXPECT_EQ(portfolio_ptr, &portfolio);
    EXPECT_EQ(books_ptr, &order_books);
    EXPECT_EQ(broker_ptr, &broker);
}

TEST(StrategyTest, SignalFunctionCanQueueOrdersAndActionsOnBroker) {
    std::vector<Ticker> tickers{Ticker("AAPL")};
    Timeframe tf{};

    Strategy s(
        tickers,
        tf,
        [](const std::unordered_map<std::string, Bar>&,
           const Portfolio&,
           const std::unordered_map<std::string, OrderBook>&,
           Broker& br) {
            br.submit_order(Order::Market(Side::Buy, 10));
            br.submit_order(Order::Limit(Side::Sell, 5, 101.25));
            br.cancel_order(42);
            br.execute_order(7);
        }
    );

    std::unordered_map<std::string, Bar> bars;
    Portfolio portfolio(1000.0);
    std::unordered_map<std::string, OrderBook> order_books;
    Broker broker;

    EXPECT_EQ(broker.actions_count(), 0u);
    s.on_data(bars, portfolio, order_books, broker);
    EXPECT_EQ(broker.actions_count(), 4u);
}

TEST(StrategyTest, MultipleOnDataCallsInvokeSignalEachTime) {
    std::vector<Ticker> tickers{Ticker("AAPL")};
    Timeframe tf{};

    int calls = 0;

    Strategy s(
        tickers,
        tf,
        [&](const std::unordered_map<std::string, Bar>&,
            const Portfolio&,
            const std::unordered_map<std::string, OrderBook>&,
            Broker&) {
            ++calls;
        }
    );

    std::unordered_map<std::string, Bar> bars;
    Portfolio portfolio(1000.0);
    std::unordered_map<std::string, OrderBook> order_books;
    Broker broker;

    s.on_data(bars, portfolio, order_books, broker);
    s.on_data(bars, portfolio, order_books, broker);

    EXPECT_EQ(calls, 2);
}
