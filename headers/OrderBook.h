#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <deque>  //double ended queue good for deletion and insertion at both ends than a list
#include <string>
#include <vector>
#include "Order.h"
#include "Constants.h"
#include "ExecutionReport.h"

class OrderBook {
private:
    std::deque<Order> buySide;
    std::deque<Order> sellSide;
    Instrument instrument;   //orderbook per flower type

    void insertBuy(const Order& order);
    void insertSell(const Order& order);

public:
    OrderBook(Instrument inst);

    std::vector<ExecutionReport> matchOrders(Order incoming, const std::string& eventTimestamp);

    const std::deque<Order>& getBuySide() const;
    const std::deque<Order>& getSellSide() const;
};

#endif