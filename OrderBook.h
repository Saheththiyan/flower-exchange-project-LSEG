#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <list>
#include <string>
#include "Order.h"
#include "Constants.h"

class OrderBook {
private:
    std::list<Order> buySide;   
    std::list<Order> sellSide;  
    Instrument instrument;

    void insertBuy(const Order& order);
    void insertSell(const Order& order);
    void matchOrders();

public:
    OrderBook(Instrument inst);

    void processOrder(const Order& order);
    void cancelOrder(const std::string& clientOrderID);

    const std::list<Order>& getBuySide() const;
    const std::list<Order>& getSellSide() const;
};

#endif