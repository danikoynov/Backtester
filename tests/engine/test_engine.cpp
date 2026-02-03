#include <gtest/gtest.h>

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "engine/engine.hpp"
#include "strategy/strategy.hpp"
#include "domain/ticker.hpp"
#include "domain/timeframe.hpp"
#include "domain/order.hpp"
#include "execution/order_book.hpp"
#include "data/bar.hpp"
#include "domain/portfolio.hpp"
#include "engine/broker.hpp"

namespace fs = std::filesystem;

using namespace bt;

static void cleanup_data_dir_for(const std::string& ticker) {

    fs::path dir = fs::path("data") / ticker;
    if (fs::exists(dir)) {
        std::error_code ec;
        fs::remove_all(dir, ec);
    }
}

class EngineIntegrationTest : public ::testing::Test {
protected:
    void TearDown() override {
        cleanup_data_dir_for("AAPL");
        cleanup_data_dir_for("MSFT");
    }
};


TEST_F(EngineIntegrationTest, BacktestRuns_NoStrategyActions) {
    std::vector<Ticker> tickers{Ticker("AAPL")};
    Timeframe tf = Timeframe::Minute1;

    Strategy s(
        tickers,
        tf,
        [](const std::unordered_map<std::string, Bar>&,
           const Portfolio&,
           const std::unordered_map<std::string, OrderBook>&,
           Broker&) {

        }
    );

    Engine engine(s, 1000.0);

    EXPECT_NO_THROW(engine.backtest());
}

TEST_F(EngineIntegrationTest, MarketOrderEventuallyExecutes) {
    std::vector<Ticker> tickers{Ticker("AAPL")};
    Timeframe tf = Timeframe::Minute1;

    bool submitted = false;
    bool saw_executed = false;

    Strategy s(
        tickers,
        tf,
        [&](const std::unordered_map<std::string, Bar>&,
            const Portfolio&,
            const std::unordered_map<std::string, OrderBook>& books,
            Broker& br) {

            if (!submitted) {
                submitted = true;
                br.submit_order(Order::Market(Side::Buy, 1), Ticker("AAPL"));
            }

            auto it = books.find("AAPL");
            if (it != books.end()) {
                const auto executed = it->second.get_executed_orders();
                if (!executed.empty()) {
                    saw_executed = true;
                }
            }
        }
    );

    Engine engine(s, 1000.0);
    engine.backtest();

    EXPECT_TRUE(submitted) << "Strategy never got called / never submitted an order.";
    EXPECT_TRUE(saw_executed) << "Market order was never marked executed in the order book.";
}

TEST_F(EngineIntegrationTest, TwoTickers_SubmitOrders_NoCrash) {
    std::vector<Ticker> tickers{Ticker("AAPL"), Ticker("MSFT")};
    Timeframe tf = Timeframe::Minute1;

    bool submitted = false;

    Strategy s(
        tickers,
        tf,
        [&](const std::unordered_map<std::string, Bar>&,
            const Portfolio&,
            const std::unordered_map<std::string, OrderBook>&,
            Broker& br) {

            if (submitted) return;
            submitted = true;

            br.submit_order(Order::Market(Side::Buy, 1), Ticker("AAPL"));
            br.submit_order(Order::Market(Side::Buy, 1), Ticker("MSFT"));
        }
    );

    Engine engine(s, 1000.0);

    EXPECT_NO_THROW(engine.backtest());
    EXPECT_TRUE(submitted);
}

