#include <gtest/gtest.h>

#include "domain/portfolio.hpp" 

using namespace bt;


TEST(PortfolioCtor, PortfolioConstructsSuccessfuly){
    std::unordered_map<std::string, double> initial_prices;
    initial_prices["AAPL"] = 150.0;
    initial_prices["NVDA"] = 125.0;
    double initial_value = 1000.0;
    EXPECT_NO_THROW(Portfolio(initial_value, initial_prices));
}


TEST(PortfolioCtor, InitialValueNegativeThrows) {
    std::unordered_map<std::string, double> initial_prices;
    initial_prices["AAPL"] = 150.0;
    initial_prices["NVDA"] = 125.0;
    double initial_value = -1000.0;
    EXPECT_THROW(Portfolio(initial_value, initial_prices), 
        std::invalid_argument
    );
}

TEST(PortfolioGetter, InitialValueGetter) {
    std::unordered_map<std::string, double> initial_prices;
    initial_prices["AAPL"] = 150.0;
    initial_prices["NVDA"] = 125.0;
    double initial_value = 1000.0;
    Portfolio portfolio(initial_value, initial_prices);
    EXPECT_EQ(portfolio.initial_value(), initial_value);
}

