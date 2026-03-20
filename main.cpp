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

            auto matchedOrder = instrumentOrderBook.getOrderBook(order.instrument).match(order);

            if (matchedOrder) {
                rows.push_back(ExecutionReport{
                    order.orderID,
                    order.clientOrderID,
                    order.instrument,
                    ExecStatus::Fill,
                    order.side,
                    order.quantity,
                    order.price
                });

                rows.push_back(ExecutionReport{
                    matchedOrder->orderID,
                    matchedOrder->clientOrderID,
                    matchedOrder->instrument,
                    ExecStatus::Fill,
                    matchedOrder->side,
                    matchedOrder->quantity,
                    matchedOrder->price
                });
                continue;
            }

            auto partialMatchedOrder = instrumentOrderBook.getOrderBook(order.instrument).partialMatch(order);

            if (partialMatchedOrder) {
                rows.push_back(ExecutionReport{
                    order.orderID,
                    order.clientOrderID,
                    order.instrument,
                    ExecStatus::PFill,
                    order.side,
                    partialMatchedOrder->quantity,
                    partialMatchedOrder->price
                });

                rows.push_back(ExecutionReport{
                    partialMatchedOrder->orderID,
                    partialMatchedOrder->clientOrderID,
                    partialMatchedOrder->instrument,
                    ExecStatus::Fill,
                    partialMatchedOrder->side,
                    partialMatchedOrder->quantity,
                    partialMatchedOrder->price
                });

                instrumentOrderBook.getOrderBook(order.instrument).addOrder(Order{
                    order.orderID,
                    order.clientOrderID,
                    order.instrument,
                    order.side,
                    order.quantity - partialMatchedOrder->quantity,
                    order.price
                });
                continue;
            }

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

        }

        reportWriter.writeRows(rows);

        instrumentOrderBook.printAvailableOrderBooks();
    } catch (const exception& ex) {
        cerr << "Error: " << ex.what() << endl;
        return 1;
    }

    return 0;
}