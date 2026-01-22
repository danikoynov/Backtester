## Features

    - Add a strategy interface/Class
    - Add an engine Class
    - Add start and end time for the data_fetcher

## Small fixes

    - Make the fetcher work with python3 too

## Control Flow

    1. Engine tries to execute existing orders
    2. Engine fetches data
    3. Engine sends data to strategy interface
    4. Strategy interface makes decisions (trade side, position, risk management...)
    5. Strategy interface return orders (potentially none) to the engine
    6. Engine adds to the existing stack

## Testing

    - Add additional tests on Portfolio class
    - Add tests on Data Fetcher
    - Add tests on detail
    - Add tests on Bar class

## Documentation

    - Add requirements for the python scripts
    - Add documentation on Portfolio Class
    - Add documentation on Order Class

## Nice to do

    - add a standard language formatting

## Done

    - Execution logic
    - Fill class
    - OHLC Class
    - Container for Bars inside Data Fetcher
    - Data Fetcher
    - Ticker Class
    - Timeframe Enum Class
    - Portfolio Class
    - Order Class
    - Set up Google test system
