#pragma once
#include <chrono>
#include <vector>
#include <string>

namespace bt {
    using Timestamp = std::chrono::time_point<
        std::chrono::system_clock,
        std::chrono::seconds
    >;
    

    std::vector<std::string> split_csv_line(const std::string &line);
    double parse_double(std::string_view sv);
    std::int64_t parse_int64_t(std::string_view sv);
    int parse_int(std::string_view sv);
    Timestamp parse_timestamp_seconds(std::string_view sv);
}