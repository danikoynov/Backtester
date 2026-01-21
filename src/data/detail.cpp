#include "data/detail.hpp"
#include <charconv>
#include <string>
#include <vector>
#include <stdexcept>
namespace bt {

     
    std::vector<std::string> split_csv_line(const std::string &line) {
        std::vector<std::string> out;
        int last_comma = -1, line_size = line.size();
        for (int i = 0; i < line_size; i ++) {
            if (line[i] == ',')
            {
                out.push_back(line.substr(last_comma + 1, i - last_comma - 1));
                last_comma = i;
            }
        }
        out.push_back(line.substr(last_comma + 1, line_size - last_comma - 1));
        return out;
    }

    double parse_double(std::string_view sv) {
        if (sv.empty())
            throw std::invalid_argument("Empty string");

        double value{};
        auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);

        if (ec != std::errc{} || ptr != sv.data() + sv.size())
            throw std::invalid_argument("Invalid double");

        return value;
    }

    
    std::int64_t parse_int64_t(std::string_view sv) {
        if (sv.empty())
            throw std::invalid_argument("Empty string");

        std::int64_t value{};
        auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);

        if (ec != std::errc{} || ptr != sv.data() + sv.size())
            throw std::invalid_argument("Invalid double");

        return value;
    }

    int parse_int(std::string_view sv) {
        if (sv.empty())
            throw std::invalid_argument("Empty string");

        int value{};
        auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);

        if (ec != std::errc{} || ptr != sv.data() + sv.size())
            throw std::invalid_argument("Invalid double");

        return value;
    }


    Timestamp parse_timestamp_seconds(std::string_view sv) {
        if (sv[4]  != '-' || sv[7]  != '-' ||
            sv[10] != ' ' ||
            sv[13] != ':' || sv[16] != ':' ||
            (sv[19] != '+' && sv[19] != '-') ||
            sv[22] != ':') {
            throw std::invalid_argument("Invalid timestamp format");
        }
        
        const int year = parse_int(sv.substr(0, 4));
        const int month = parse_int(sv.substr(5, 2));
        const int day = parse_int(sv.substr(8, 2));
        const int hour = parse_int(sv.substr(11, 2));
        const int minute = parse_int(sv.substr(14, 2));
        const int second = parse_int(sv.substr(17, 2));
    
        if (month < 1 || month > 12)  throw std::invalid_argument("Month out of range");
        if (day   < 1 || day   > 31)  throw std::invalid_argument("Day out of range");
        if (hour  < 0 || hour  > 23)  throw std::invalid_argument("Hour out of range");
        if (minute< 0 || minute> 59)  throw std::invalid_argument("Minute out of range");
        if (second< 0 || second> 60)  throw std::invalid_argument("Second out of range");
        
        const std::chrono::year_month_day ymd {
            std::chrono::year{year},
            std::chrono::month{static_cast<unsigned>(month)},
            std::chrono::day{static_cast<unsigned>(day)}
        };

        Timestamp ts = std::chrono::sys_days{ymd} +
            std::chrono::hours{hour} +
            std::chrono::minutes{minute} +
            std::chrono::seconds{second};

        return ts;
    }

}