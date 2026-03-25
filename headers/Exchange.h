#ifndef EXCHANGE_H
#define EXCHANGE_H

#include <array>
#include <string>
#include <vector>
#include "OrderBook.h"
#include "ExecutionReport.h"
#include "InputValidator.h"

class Exchange {
private:
    std::array<OrderBook, 5> orderBooks;
    int orderCounter;
    std::string generateOrderID();
    static std::size_t instrumentToIndex(Instrument instrument);

public:
    Exchange();

    void processOrders(const std::string& inputFileName, const std::string& outputFileName);
};

#endif