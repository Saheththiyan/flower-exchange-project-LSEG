#include "headers/csvOrderReader.hpp"
#include "headers/executionReportWriter.hpp"
#include "headers/orderBook.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    try {
        CSVOrderReader reader("order.csv");
        OrderBook orderBook;
        ExecutionReportWriter reportWriter("execution_rep.csv");

        const auto orders = reader.readOrders();
        reportWriter.writeHeader();

        vector<ExecutionReport> rows;
        int id = 1;

        for (const auto& order : orders) {
            orderBook.addOrder(order);
            rows.push_back(ExecutionReport{
                "ord" + to_string(id++),
                order.clientOrderID,
                order.instrument,
                ExecStatus::New,
                order.side,
                order.quantity,
                order.price
            });
        }

        reportWriter.writeRows(rows);
    } catch (const exception& ex) {
        cerr << "Error: " << ex.what() << endl;
        return 1;
    }

    return 0;
}