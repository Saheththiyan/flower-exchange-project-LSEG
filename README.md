# Stock Exchange Order Matching Engine

A C++ implementation of a basic stock exchange order matching system, built as part of the LSEG certification program. The core goal was to correctly implement price-time priority matching across multiple instruments using standard C++ data structures.

## What it does

Reads buy/sell orders from a CSV file, matches them against an in-memory order book using price-time priority, and writes execution reports to another CSV file.

Supported execution statuses: `New`, `PFill`, `Fill`, `Reject`.

## Project Structure
```
flower-exchange-project-LSEG/
├── main.cpp
├── order.csv                  # Input
├── execution_rep.csv          # Output
├── headers/
│   ├── order.hpp              # Order, ExecutionReport, TradeFill structs
│   ├── orderBook.hpp
│   ├── instrumentOrderBook.hpp
│   ├── csvOrderReader.hpp
│   └── executionReportWriter.hpp
├── src/
│   ├── orderBook.cpp          # Matching logic lives here
│   ├── instrumentOrderBook.cpp
│   ├── csvOrderReader.cpp
│   └── executionReporterWriter.cpp
├── Makefile
└── build/                     # Auto-created, git-ignored
```

## Core Data Structures

The order book uses `map<double, deque<Order>>` separately for buy and sell sides:

- **Buy side**: `map` in descending order → highest bid at top
- **Sell side**: `map` in ascending order → lowest ask at top
- Within each price level, orders are matched FIFO via `deque`

This gives O(log n) price-level lookup and O(1) front-of-queue access.

## Matching Logic

When an order comes in:

1. Validate it (non-negative quantity, valid price, known instrument, etc.) — reject if invalid
2. Check the opposite side for a crossable price:
   - Incoming BUY: match against the lowest available SELL price, if `sell_price <= buy_price`
   - Incoming SELL: match against the highest available BUY price, if `buy_price >= sell_price`
3. Fill as much quantity as possible, generating fill reports for each matched order
4. If quantity remains after matching, add the remainder to the book with status `New`

Partial fills on both sides are handled. The resting order gets a `PFill` report and stays in the book with reduced quantity.

## Building and Running

**Requirements:** GCC/G++ with C++17, Make, Linux/Unix or WSL
```bash
make build    # compiles to build/exchange_app
make run      # build + run (Default file name is order.csv)
make run FILE=[FILE_NAME]    # overrides the input file
make clean    # removes build/
make rebuild  # clean + build
```

**Input format (`order.csv`):**
```
ClientOrderID,Instrument,Side,Quantity,Price
```

**Output format (`execution_rep.csv`):**
```
OrderID,ClientOrderID,Instrument,ExecStatus,Side,Quantity,Price,Reason
```

## Limitations

- Single-threaded — no concurrency support
- No persistence; order book state is lost when the process exits
- No real-time feed; processes a static CSV file in batch
- No market order support — limit orders only
- Minimal error handling; malformed CSV rows beyond basic validation may cause undefined behavior
- Not benchmarked; performance characteristics under load are unknown