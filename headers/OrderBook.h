#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <list>
#include <string>
#include <vector>
#include "Order.h"
#include "Constants.h"
#include "ExecutionReport.h"

class OrderBook {
private:
    std::list<Order> buySide;   
    std::list<Order> sellSide;  
    Instrument instrument;

    void insertBuy(const Order& order);
    void insertSell(const Order& order);

public:
    OrderBook(Instrument inst);

    std::vector<ExecutionReport> processOrder(const Order& order);
    void cancelOrder(const std::string& clientOrderID);

    const std::list<Order>& getBuySide() const;
    const std::list<Order>& getSellSide() const;
};

#endif