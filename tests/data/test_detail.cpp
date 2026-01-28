#include <gtest/gtest.h>

#include "data/detail.hpp"

#include <chrono>
#include <limits>
#include <string>
#include <vector>

namespace bt::test {

static bt::Timestamp make_ts(int y, int m, int d, int hh, int mm, int ss) {
    const std::chrono::year_month_day ymd{
        std::chrono::year{y},
        std::chrono::month{static_cast<unsigned>(m)},
        std::chrono::day{static_cast<unsigned>(d)}
    };

    return bt::Timestamp{std::chrono::sys_days{ymd}}
        + std::chrono::hours{hh}
        + std::chrono::minutes{mm}
        + std::chrono::seconds{ss};
}

// -------------------- split_csv_line --------------------

TEST(SplitCsvLine, NoComma) {
    const std::string line = "abc";
    const auto out = bt::split_csv_line(line);
    ASSERT_EQ(out.size(), 1u);
    EXPECT_EQ(out[0], "abc");
}

TEST(SplitCsvLine, BasicThreeFields) {
    const std::string line = "a,b,c";
    const auto out = bt::split_csv_line(line);
    ASSERT_EQ(out.size(), 3u);
    EXPECT_EQ(out[0], "a");
    EXPECT_EQ(out[1], "b");
    EXPECT_EQ(out[2], "c");
}

TEST(SplitCsvLine, EmptyLineGivesSingleEmptyField) {
    const std::string line = "";
    const auto out = bt::split_csv_line(line);
    ASSERT_EQ(out.size(), 1u);
    EXPECT_EQ(out[0], "");
}

TEST(SplitCsvLine, LeadingComma) {
    const std::string line = ",a,b";
    const auto out = bt::split_csv_line(line);
    ASSERT_EQ(out.size(), 3u);
    EXPECT_EQ(out[0], "");
    EXPECT_EQ(out[1], "a");
    EXPECT_EQ(out[2], "b");
}

TEST(SplitCsvLine, TrailingComma) {
    const std::string line = "a,b,";
    const auto out = bt::split_csv_line(line);
    ASSERT_EQ(out.size(), 3u);
    EXPECT_EQ(out[0], "a");
    EXPECT_EQ(out[1], "b");
    EXPECT_EQ(out[2], "");
}

TEST(SplitCsvLine, ConsecutiveCommas) {
    const std::string line = "a,,b";
    const auto out = bt::split_csv_line(line);
    ASSERT_EQ(out.size(), 3u);
    EXPECT_EQ(out[0], "a");
    EXPECT_EQ(out[1], "");
    EXPECT_EQ(out[2], "b");
}

// -------------------- parse_double --------------------

TEST(ParseDouble, ValidNumbers) {
    EXPECT_DOUBLE_EQ(bt::parse_double("0"), 0.0);
    EXPECT_DOUBLE_EQ(bt::parse_double("-12.5"), -12.5);
    EXPECT_NEAR(bt::parse_double("3.14159"), 3.14159, 1e-12);
}

TEST(ParseDouble, RejectsEmpty) {
    EXPECT_THROW(bt::parse_double(""), std::invalid_argument);
    EXPECT_THROW(bt::parse_double(std::string_view{}), std::invalid_argument);
}

TEST(ParseDouble, RejectsTrailingOrLeadingGarbage) {
    EXPECT_THROW(bt::parse_double("1.0 "), std::invalid_argument);
    EXPECT_THROW(bt::parse_double(" 1.0"), std::invalid_argument);
    EXPECT_THROW(bt::parse_double("1.0x"), std::invalid_argument);
    EXPECT_THROW(bt::parse_double("x1.0"), std::invalid_argument);
    EXPECT_THROW(bt::parse_double("1,0"), std::invalid_argument);
}

// -------------------- parse_int64_t --------------------

TEST(ParseInt64, ValidNumbers) {
    EXPECT_EQ(bt::parse_int64_t("0"), 0);
    EXPECT_EQ(bt::parse_int64_t("-42"), -42);
    EXPECT_EQ(bt::parse_int64_t("9223372036854775807"),
              std::numeric_limits<std::int64_t>::max());
}

TEST(ParseInt64, RejectsEmptyAndGarbage) {
    EXPECT_THROW(bt::parse_int64_t(""), std::invalid_argument);
    EXPECT_THROW(bt::parse_int64_t("12x"), std::invalid_argument);
    EXPECT_THROW(bt::parse_int64_t(" 12"), std::invalid_argument);
    EXPECT_THROW(bt::parse_int64_t("12 "), std::invalid_argument);
}

TEST(ParseInt64, RejectsOutOfRange) {
    EXPECT_THROW(bt::parse_int64_t("9223372036854775808"), std::invalid_argument);
    EXPECT_THROW(bt::parse_int64_t("-9223372036854775809"), std::invalid_argument);
}

// -------------------- parse_int --------------------

TEST(ParseInt, ValidNumbers) {
    EXPECT_EQ(bt::parse_int("0"), 0);
    EXPECT_EQ(bt::parse_int("-42"), -42);

    const auto imax = std::numeric_limits<int>::max();
    const auto imin = std::numeric_limits<int>::min();
    EXPECT_EQ(bt::parse_int(std::to_string(imax)), imax);
    EXPECT_EQ(bt::parse_int(std::to_string(imin)), imin);
}

TEST(ParseInt, RejectsEmptyAndGarbage) {
    EXPECT_THROW(bt::parse_int(""), std::invalid_argument);
    EXPECT_THROW(bt::parse_int("7x"), std::invalid_argument);
    EXPECT_THROW(bt::parse_int(" 7"), std::invalid_argument);
    EXPECT_THROW(bt::parse_int("7 "), std::invalid_argument);
}

TEST(ParseInt, RejectsOutOfRange) {
    EXPECT_THROW(bt::parse_int("2147483648"), std::invalid_argument);
    EXPECT_THROW(bt::parse_int("-2147483649"), std::invalid_argument);
}

// -------------------- parse_timestamp_seconds --------------------

TEST(ParseTimestampSeconds, ValidParsesAndIgnoresOffset) {
    const auto ts1 = bt::parse_timestamp_seconds("2024-01-02 03:04:05+00:00");
    EXPECT_EQ(ts1, make_ts(2024, 1, 2, 3, 4, 5));

    const auto ts2 = bt::parse_timestamp_seconds("2024-01-02 03:04:05-05:00");
    EXPECT_EQ(ts2, make_ts(2024, 1, 2, 3, 4, 5));
}

TEST(ParseTimestampSeconds, RejectsBadSeparators) {
    // Keep length >= 23 to avoid UB in current implementation.
    EXPECT_THROW(bt::parse_timestamp_seconds("2024/01/02 03:04:05+00:00"),
                 std::invalid_argument);
    EXPECT_THROW(bt::parse_timestamp_seconds("2024-01-02T03:04:05+00:00"),
                 std::invalid_argument);
    EXPECT_THROW(bt::parse_timestamp_seconds("2024-01-02 03-04-05+00:00"),
                 std::invalid_argument);
    EXPECT_THROW(bt::parse_timestamp_seconds("2024-01-02 03:04:05*00:00"),
                 std::invalid_argument);
}

TEST(ParseTimestampSeconds, RejectsOutOfRangeFields) {
    EXPECT_THROW(bt::parse_timestamp_seconds("2024-13-02 03:04:05+00:00"),
                 std::invalid_argument); // month
    EXPECT_THROW(bt::parse_timestamp_seconds("2024-01-00 03:04:05+00:00"),
                 std::invalid_argument); // day
    EXPECT_THROW(bt::parse_timestamp_seconds("2024-01-02 24:04:05+00:00"),
                 std::invalid_argument); // hour
    EXPECT_THROW(bt::parse_timestamp_seconds("2024-01-02 03:60:05+00:00"),
                 std::invalid_argument); // minute
    EXPECT_THROW(bt::parse_timestamp_seconds("2024-01-02 03:04:61+00:00"),
                 std::invalid_argument); // second (>60)
}

TEST(ParseTimestampSeconds, RejectsNonNumericFields) {
    EXPECT_THROW(bt::parse_timestamp_seconds("202x-01-02 03:04:05+00:00"),
                 std::invalid_argument);
    EXPECT_THROW(bt::parse_timestamp_seconds("2024-0x-02 03:04:05+00:00"),
                 std::invalid_argument);
    EXPECT_THROW(bt::parse_timestamp_seconds("2024-01-02 0x:04:05+00:00"),
                 std::invalid_argument);
}

} 
