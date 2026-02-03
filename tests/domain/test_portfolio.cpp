#include <gtest/gtest.h>

#include <unordered_map>
#include <string>
#include <stdexcept>

#include "domain/portfolio.hpp"

using bt::Portfolio;

namespace {
constexpr double kEps = 1e-9;
}

TEST(PortfolioTest, TwoArgConstructorRejectsNegativeInitialValue) {
    std::unordered_map<std::string, double> prices{{"AAPL", 100.0}};
    EXPECT_THROW(Portfolio(-1.0, prices), std::invalid_argument);
}

TEST(PortfolioTest, OneArgConstructorShouldSetCashToInitialValue) {
    Portfolio p(1000.0);

    EXPECT_NEAR(p.initial_value(), 1000.0, kEps);

    EXPECT_NEAR(p.current_value(), 1000.0, kEps);
    EXPECT_NEAR(p.pnl(), 0.0, kEps);
}

TEST(PortfolioTest, InitialStateWithPricesHasZeroPnLAndEqualsInitialValue) {
    std::unordered_map<std::string, double> prices{
        {"AAPL", 100.0},
        {"MSFT", 200.0}
    };
    Portfolio p(1000.0, prices);

    EXPECT_NEAR(p.initial_value(), 1000.0, kEps);
    EXPECT_NEAR(p.current_value(), 1000.0, kEps);
    EXPECT_NEAR(p.pnl(), 0.0, kEps);
}

TEST(PortfolioTest, UpdatePriceRejectsNegativePrice) {
    std::unordered_map<std::string, double> prices{{"AAPL", 100.0}};
    Portfolio p(1000.0, prices);

    EXPECT_THROW(p.update_price("AAPL", -0.01), std::invalid_argument);
}

TEST(PortfolioTest, UpdatePriceThrowsIfSymbolUnknown) {
    std::unordered_map<std::string, double> prices{{"AAPL", 100.0}};
    Portfolio p(1000.0, prices);

    EXPECT_THROW(p.update_price("MSFT", 200.0), std::out_of_range);
}

TEST(PortfolioTest, UpdatePricesUpdatesAllProvidedSymbols) {
    std::unordered_map<std::string, double> prices{
        {"AAPL", 100.0},
        {"MSFT", 200.0}
    };
    Portfolio p(1000.0, prices);

    std::unordered_map<std::string, double> new_prices{
        {"AAPL", 110.0},
        {"MSFT", 190.0}
    }; 

    p.update_prices(new_prices);
    EXPECT_NEAR(p.current_value(), 1000.0, kEps);
    EXPECT_NEAR(p.pnl(), 0.0, kEps);
}

TEST(PortfolioTest, BuyPositionKeepsCurrentValueConstantAtExecutionPrice) {
    std::unordered_map<std::string, double> prices{{"AAPL", 100.0}};
    Portfolio p(1000.0, prices);

    p.update_position("AAPL", 3);

    EXPECT_NEAR(p.current_value(), 1000.0, kEps);
    EXPECT_NEAR(p.pnl(), 0.0, kEps);
}

TEST(PortfolioTest, PriceMoveChangesPnLAfterPosition) {
    std::unordered_map<std::string, double> prices{{"AAPL", 100.0}};
    Portfolio p(1000.0, prices);

    p.update_position("AAPL", 3); 
    p.update_price("AAPL", 120.0);   

    EXPECT_NEAR(p.current_value(), 1060.0, kEps);
    EXPECT_NEAR(p.pnl(), 60.0, kEps);
}

TEST(PortfolioTest, NotEnoughCashThrows) {
    std::unordered_map<std::string, double> prices{{"AAPL", 100.0}};
    Portfolio p(250.0, prices);

    EXPECT_THROW(p.update_position("AAPL", 3), std::runtime_error);
}

TEST(PortfolioTest, UpdatePositionThrowsIfSymbolUnknown) {
    std::unordered_map<std::string, double> prices{{"AAPL", 100.0}};
    Portfolio p(1000.0, prices);

    EXPECT_THROW(p.update_position("MSFT", 1), std::out_of_range);
}

TEST(PortfolioTest, UpdateCashChangesCurrentValueAndPnL) {
    std::unordered_map<std::string, double> prices{{"AAPL", 100.0}};
    Portfolio p(1000.0, prices);

    p.update_cash(50.0);
    EXPECT_NEAR(p.current_value(), 1050.0, kEps);
    EXPECT_NEAR(p.pnl(), 50.0, kEps);

    p.update_cash(-20.0);
    EXPECT_NEAR(p.current_value(), 1030.0, kEps);
    EXPECT_NEAR(p.pnl(), 30.0, kEps);
}

TEST(PortfolioTest, SellingIncreasesCashAndCanRealizePnLViaMarkToMarket) {
    std::unordered_map<std::string, double> prices{{"AAPL", 100.0}};
    Portfolio p(1000.0, prices);

    p.update_position("AAPL", 5);    
    p.update_price("AAPL", 120.0);    
    EXPECT_NEAR(p.pnl(), 100.0, kEps); 

    p.update_position("AAPL", -2);

    EXPECT_NEAR(p.current_value(), 1100.0, kEps);
    EXPECT_NEAR(p.pnl(), 100.0, kEps);
}

