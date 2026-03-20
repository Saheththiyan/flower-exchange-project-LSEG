#include "headers/csvOrderReader.hpp"
#include "headers/executionReportWriter.hpp"
#include "headers/instrumentOrderBook.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    try {
        CSVOrderReader reader("order.csv");
        InstrumentOrderBook instrumentOrderBook;
        ExecutionReportWriter reportWriter("execution_rep.csv");

        const auto orders = reader.readOrders();
        reportWriter.writeHeader();

        vector<ExecutionReport> rows;
        int id = 1;

        for (auto order : orders) {
            order.orderID = "ord" + to_string(id++);

            if (!order.reason.empty()) {
                rows.push_back(ExecutionReport{
                    order.orderID,
                    order.clientOrderID,
                    order.instrument,
                    ExecStatus::Reject,
                    order.side,
                    order.quantity,
                    order.price,
                    order.reason
                });
                continue;
            }

            Order working = order;
            auto fills = instrumentOrderBook.getOrderBook(order.instrument).execute(working);

            if (fills.empty()) {
                instrumentOrderBook.getOrderBook(order.instrument).addOrder(order);
                rows.push_back(ExecutionReport{
                    order.orderID,
                    order.clientOrderID,
                    order.instrument,
                    ExecStatus::New,
                    order.side,
                    order.quantity,
                    order.price
                });
                continue;
            }

            for (const auto& fill : fills) {
                ExecStatus status = (working.quantity == 0) ? ExecStatus::Fill : ExecStatus::PFill;

                rows.push_back(ExecutionReport({
                    order.orderID,
                    order.clientOrderID,
                    order.instrument,
                    status,
                    order.side,
                    fill.quantity,
                    fill.price
                }));

                rows.push_back(ExecutionReport({
                    fill.restingOrder.orderID,
                    fill.restingOrder.clientOrderID,
                    fill.restingOrder.instrument,
                    ExecStatus::Fill,
                    fill.restingOrder.side,
                    fill.quantity,
                    fill.price
                }));
            }

            if (working.quantity > 0) {
                Order remaining = order;
                remaining.quantity = working.quantity;
                instrumentOrderBook.getOrderBook(order.instrument).addOrder(remaining);
            }
        }

        reportWriter.writeRows(rows);

        instrumentOrderBook.printAvailableOrderBooks();
    } catch (const exception& ex) {
        cerr << "Error: " << ex.what() << endl;
        return 1;
    }

    return 0;
}