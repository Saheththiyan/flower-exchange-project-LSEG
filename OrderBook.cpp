#include "OrderBook.h"

void OrderBook::processOrder(const Order& order) {
    if (order.getSide() == 1)
        insertBuy(order);
    else
        insertSell(order);

    matchOrders();
}

void OrderBook::insertBuy(const Order& order) {
    auto i = buySide.begin();
    while (i != buySide.end() && i->getPrice() > order.getPrice())
        ++i;
    buySide.insert(i, order);
}

void OrderBook::insertSell(const Order& order) {
    auto i = sellSide.begin();
    while (i != sellSide.end() && i->getPrice() < order.getPrice())
        ++i;
    sellSide.insert(i, order);
}


void OrderBook::matchOrders() {
    while (!buySide.empty() && !sellSide.empty()) {
        Order& bestBuy  = buySide.front();  
        Order& bestSell = sellSide.front();  

        if (bestBuy.getPrice() < bestSell.getPrice())
            break; 

        int fillQty = std::min(bestBuy.getQuantity(), bestSell.getQuantity());
        bestBuy.setQuantity(bestBuy.getQuantity()   - fillQty);
        bestSell.setQuantity(bestSell.getQuantity() - fillQty);

        if (bestBuy.getQuantity()  == 0) buySide.pop_front();
        if (bestSell.getQuantity() == 0) sellSide.pop_front();
    }
}