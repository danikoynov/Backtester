## Features

    - Validation on broker and action!
    - Add a Visualizer
    - Add indicators

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

    Data:
    - ~~Add tests on bar.hpp~~
    - Add tests on data_fetcher.hpp
    - Add tests on detail.hpp

    Domain:
    - Add additional tests on portfolio.hpp
    - Add tests on ticker.hpp
    - Add tests on timeframe.hpp

    Execution:
    - Add tests on execution_model.hpp
    - Add tests on fill.hpp
    - Add additional tests on order_book.hpp

    Strategy:
    - Add tests on strategy.hpp

    Engine:
    - Add tests on action.hpp
    - Add tests on broker.hpp
    - Add tests on engine.hpp

## Documentation

    - Add requirements for the python scripts
    - Add documentation on components

## Nice to do

    - add a standard language formatting

## Done

    - Action Class
    - Broker Class
    - Engine Class
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
