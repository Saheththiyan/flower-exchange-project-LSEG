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