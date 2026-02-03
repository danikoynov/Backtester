# Backtester (C++)

A modular, event-driven **backtesting engine written in modern C++**, built to explore **low-latency design**, **clean architecture**, and **idiomatic C++ practices** while upgrading a previous Python backtesting engine.

---

## Motivation

This project is a ground-up rewrite of my earlier **Python backtesting engine**, with three main goals:

1. **Learn modern, idiomatic C++**
   - RAII, value semantics, const-correctness
   - `std::optional`, `std::function`, exceptions for invariant violations
   - clean interfaces and clear ownership

2. **Structure a real C++ project**
   - modular layout (`domain`, `engine`, `execution`, `strategy`, `data`)
   - separation of interface (`include/`) and implementation (`src/`)
   - testable components with GoogleTest

3. **Minimize latency by design**
   - avoid unnecessary copies (pass by reference where appropriate)
   - use move semantics when owning data
   - explicit boundaries for mutation and state updates

## High-Level Architecture

The system follows an **event-driven architecture** similar to real trading systems:

- The **Engine** owns the event loop.
- The **Strategy** observes state and emits intent (orders/cancels) via a **Broker**.
- The **Broker** queues actions so the Strategy remains decoupled from execution.
- The **Engine** applies those actions to **OrderBook(s)** and the **Portfolio**.

### Control Flow

For each bar:

1. **Engine fetches data**
2. **Engine tries to execute existing orders**
3. **Engine sends data to the strategy**
4. **Strategy makes decisions** and pushes actions (orders, cancels, fills) into the broker
5. **Engine applies actions** (updates order books + portfolio)


---

## Core Modules

### `domain/`
Value objects and invariants:
- **Ticker**: wraps a symbol string
- **Order**: Market/Limit/Stop/StopLimit orders with strong invariants + factory constructors
- **Portfolio**: cash, positions, mark-to-market valuation

### `strategy/`
- **Strategy** holds the user-defined signal function:
  - observes bars + portfolio + order books
  - uses **Broker&** to submit orders/cancels/fills

### `engine/`
- **Broker** queues actions (`InsertOrder`, `CancelOrder`, `ExecuteOrder`, `ApplyFill`)
- **Engine** drives the backtest and orchestrates the pipeline

### `execution/`
- **OrderBook** tracks active orders per ticker
- **Execution model** decides when orders fill based on incoming bars (market/limit/stop logic)
- **Fill** represents executed quantity/value on a ticker

### `data/`
- **DataFetcher** loads price data (currently CSV-based) and provides bars to the engine

---

## Building

This project uses **CMake** and follows an out-of-source build setup.

### Configure

From the project root:

```bash
cmake -S . -B build
```

### Build

```bash
cmake --build build
```

### Testing

```bash
ctest --test-dir build
```