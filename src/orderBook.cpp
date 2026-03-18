#include "../headers/orderBook.hpp"
using namespace std;

void OrderBook::addOrder(const Order& order) {
    if (order.side == Side::BUY) {
        buyOrders.push_back(order);
    } else {
        sellOrders.push_back(order);
    }
}

const vector<Order>& OrderBook::getBuyOrders() const {
    return buyOrders;
}

const vector<Order>& OrderBook::getSellOrders() const {
    return sellOrders;
}

optional<Order> OrderBook::tryMatch(const Order& incomingOrder) {

    if (incomingOrder.side == Side::BUY) {
        for (size_t i = 0; i < sellOrders.size(); ++i) {
            const Order& resting = sellOrders[i];
            if (resting.instrument == incomingOrder.instrument && resting.price == incomingOrder.price && resting.quantity == incomingOrder.quantity) {
                Order matched = resting;
                sellOrders.erase(sellOrders.begin() + i);
                return matched;
            }
        }

    } else {
        for (size_t i = 0; i < buyOrders.size(); ++i) {
            const Order& resting = buyOrders[i];
            if (resting.instrument == incomingOrder.instrument && resting.price == incomingOrder.price && resting.quantity == incomingOrder.quantity) {
                Order matched = resting;
                buyOrders.erase(buyOrders.begin() + i);
                return matched;
            }
        }
    }

    return nullopt;
}