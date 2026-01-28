/*
split_csv_line
Purpose:
    Split a line on ',' into fields (no quoting/escaping support).

Invariants:
    - Returns at least one field (possibly empty).
*/

/*
parse_double
Purpose:
    Parse a base-10 double from a string_view (must consume entire input).

Errors:
    Throws std::invalid_argument on empty input or invalid/partial parse.
*/

/*
parse_int64_t
Purpose:
    Parse a base-10 int64_t from a string_view (must consume entire input).

Errors:
    Throws std::invalid_argument on empty input, invalid parse, or overflow.
*/

/*
parse_int
Purpose:
    Parse a base-10 int from a string_view (must consume entire input).

Errors:
    Throws std::invalid_argument on empty input, invalid parse, or overflow.
*/

/*
parse_timestamp_seconds
Purpose:
    Parse "YYYY-MM-DD HH:MM:SS±HH:MM" into Timestamp (seconds precision).
    Offset is format-checked but not applied.

Errors:
    Throws std::invalid_argument on format/parse/range errors.
*/

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