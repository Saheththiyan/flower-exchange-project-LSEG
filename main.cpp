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

            auto matchedOrder = instrumentOrderBook.getOrderBook(order.instrument).tryMatch(order);

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
            } else {    

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
        }

        reportWriter.writeRows(rows);

        instrumentOrderBook.printAvailableOrderBooks();
    } catch (const exception& ex) {
        cerr << "Error: " << ex.what() << endl;
        return 1;
    }

    return 0;
}