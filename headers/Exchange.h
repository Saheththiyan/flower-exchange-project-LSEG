#ifndef EXCHANGE_H
#define EXCHANGE_H

#include <string>
#include <map>
#include <vector>
#include "OrderBook.h"
#include "ExecutionReport.h"
#include "InputValidator.h"

class Exchange {
private:
    std::map<std::string, OrderBook> orderBooks;
    int currentOrderIdCounter;
    std::string generateOrderID();

public:
    Exchange();

    void processOrders(const std::string& inputFileName, const std::string& outputFileName);
};

#endif