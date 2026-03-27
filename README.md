# Flower Exchange Project

Small C++ command-line app that reads orders from a CSV file and writes execution reports to a CSV file.

## Compile

Run this from the project root:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic \
  main.cpp Exchange.cpp ExecutionReport.cpp InputValidator.cpp Order.cpp OrderBook.cpp \
  -o exchange_app
```

## Run

```bash
./exchange_app [input_csv] [output_csv]
```

Arguments:
- `input_csv` (optional): Path to the input orders file.
- `output_csv` (optional): Path for the generated execution report.

Defaults when omitted:
- Input: `order.csv`
- Output: `execution_rep.csv`

## Example

```bash
./exchange_app example_orders/order2.csv execution_rep.csv
```

## Input CSV Format

Header and columns expected:

```csv
Cl. Ord.ID,Instrument,Side,Quantity,Price
```

Notes:
- `Instrument`: one of `Rose`, `Lavender`, `Lotus`, `Tulip`, `Orchid`
- `Side`: `1` = buy, `2` = sell
