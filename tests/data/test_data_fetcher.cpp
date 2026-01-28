// tests/test_data_fetcher_simple.cpp
#include <gtest/gtest.h>

#include "data/data_fetcher.hpp"
#include "data/detail.hpp"
#include "domain/timeframe.hpp"
#include "domain/ticker.hpp"

#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

#ifndef BT_PROJECT_ROOT
#define BT_PROJECT_ROOT "."
#endif

namespace bt::test {

static void expect_bar(const bt::Bar& b, const std::string& ts,
                       double o, double h, double l, double c, std::int64_t v) {
    EXPECT_EQ(b.timestamp(), bt::parse_timestamp_seconds(ts));
    EXPECT_DOUBLE_EQ(b.open(),  o);
    EXPECT_DOUBLE_EQ(b.high(),  h);
    EXPECT_DOUBLE_EQ(b.low(),   l);
    EXPECT_DOUBLE_EQ(b.close(), c);
    EXPECT_EQ(b.volume(), v);
}

static void write_text(const fs::path& p, const std::string& s) {
    fs::create_directories(p.parent_path());
    std::ofstream out(p, std::ios::trunc);
    ASSERT_TRUE(out.is_open()) << "Cannot write: " << p.string();
    out << s;
}

static void write_csv_no_ext(const std::string& ticker, const std::string& tf,
                             const std::string& body_lines) {
    fs::create_directories(fs::path("data") / ticker);

    // IMPORTANT: matches your reader: data/<ticker>/<tf>  (no ".csv")
    const fs::path p = fs::path("data") / ticker / tf;

    std::ofstream out(p, std::ios::trunc);
    ASSERT_TRUE(out.is_open()) << "Cannot write: " << p.string();

    out << "timestamp,open,high,low,close,volume\n";
    out << body_lines;
}

struct DataFetcherSimple : ::testing::Test {
    fs::path old_cwd;

    bool had_script = false;
    std::string old_script;

    const std::string t1 = "AAPL";
    const std::string t2 = "MSFT";
    const std::string tf = "1m"; // Timeframe::Minute1 maps to "1m"

    void SetUp() override {
        old_cwd = fs::current_path();
        fs::current_path(fs::path(BT_PROJECT_ROOT));

        // Backup existing fetch script (if any), then replace with no-op.
        const fs::path sp = fs::path("scripts") / "fetch_yf.py";
        had_script = fs::exists(sp);
        if (had_script) {
            std::ifstream in(sp, std::ios::binary);
            old_script.assign((std::istreambuf_iterator<char>(in)),
                              std::istreambuf_iterator<char>());
        }
        write_text(sp, "import sys\nsys.exit(0)\n");
    }

    void TearDown() override {
        // Restore fetch script
        const fs::path sp = fs::path("scripts") / "fetch_yf.py";
        if (had_script) {
            write_text(sp, old_script);
        } else {
            std::error_code ec;
            fs::remove(sp, ec);
        }

        // Remove data files we created: data/<ticker>/<tf> (no extension)
        std::error_code ec;
        fs::remove(fs::path("data") / t1 / tf, ec);
        fs::remove(fs::path("data") / t2 / tf, ec);

        fs::current_path(old_cwd);
    }
};

TEST_F(DataFetcherSimple, LoadsAndIterates) {
    write_csv_no_ext(t1, tf,
        "2024-01-02 03:04:05+00:00,1,2,0.5,1.5,100\n"
        "2024-01-02 03:04:06+00:00,2,3,1.5,2.5,200\n"
    );
    write_csv_no_ext(t2, tf,
        "2024-01-02 03:04:05+00:00,10,20,5,15,1000\n"
        "2024-01-02 03:04:06+00:00,20,30,15,25,2000\n"
    );

    std::vector<Ticker> inst{ Ticker{t1}, Ticker{t2} };
    bt::DataFetcher df(inst, Timeframe::Minute1);

    EXPECT_EQ(df.number_of_bars(), 2u);
    EXPECT_EQ(df.global_cursor(), 0u);

    auto bars0 = df.next_bars();
    expect_bar(bars0.at(t1), "2024-01-02 03:04:05+00:00", 1,2,0.5,1.5,100);
    expect_bar(bars0.at(t2), "2024-01-02 03:04:05+00:00", 10,20,5,15,1000);
    EXPECT_EQ(df.global_cursor(), 1u);

    auto bars1 = df.next_bars();
    expect_bar(bars1.at(t1), "2024-01-02 03:04:06+00:00", 2,3,1.5,2.5,200);
    expect_bar(bars1.at(t2), "2024-01-02 03:04:06+00:00", 20,30,15,25,2000);
    EXPECT_EQ(df.global_cursor(), 2u);

    EXPECT_THROW(df.next_bars(), std::runtime_error);
}

TEST_F(DataFetcherSimple, ThrowsOnInconsistentBarCounts) {
    write_csv_no_ext(t1, tf,
        "2024-01-02 03:04:05+00:00,1,2,0.5,1.5,100\n"
        "2024-01-02 03:04:06+00:00,2,3,1.5,2.5,200\n"
    );
    write_csv_no_ext(t2, tf,
        "2024-01-02 03:04:05+00:00,10,20,5,15,1000\n"
    );

    std::vector<Ticker> inst{ Ticker{t1}, Ticker{t2} };
    EXPECT_THROW((bt::DataFetcher{inst, Timeframe::Minute1}), std::runtime_error);
}

TEST_F(DataFetcherSimple, NextBarThrowsOnMissingTickerKey) {
    write_csv_no_ext(t1, tf,
        "2024-01-02 03:04:05+00:00,1,2,0.5,1.5,100\n"
    );
    write_csv_no_ext(t2, tf,
        "2024-01-02 03:04:05+00:00,10,20,5,15,1000\n"
    );

    std::vector<Ticker> inst{ Ticker{t1}, Ticker{t2} };
    bt::DataFetcher df(inst, Timeframe::Minute1);

    EXPECT_THROW(df.next_bar("NOT_A_TICKER"), std::out_of_range);
}

} // namespace bt::test
