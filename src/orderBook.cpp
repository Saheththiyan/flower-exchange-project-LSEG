#include "../headers/orderBook.hpp"
using namespace std;

void OrderBook::addOrder(const Order& order) {
    if (order.side == Side::BUY) {
        buyOrders[order.price].push_back(order);
    } else {
        sellOrders[order.price].push_back(order);
    }
}

vector<Order> OrderBook::getBuyOrders() const {
    vector<Order> out;
    for (const auto& [price, orders] : buyOrders) {
        out.insert(out.end(), orders.begin(), orders.end());
    }
    return out;

}

vector<Order> OrderBook::getSellOrders() const {
    vector<Order> out;
    for (const auto& [price, orders] : sellOrders) {
        out.insert(out.end(), orders.begin(), orders.end());
    }
    return out;
}

optional<Order> OrderBook::match(const Order& incomingOrder) {

    if (incomingOrder.side == Side::BUY) {
        auto levelIt = sellOrders.find(incomingOrder.price);
        if (levelIt == sellOrders.end())
            return nullopt;

        auto& queue = levelIt->second;
        for (auto it = queue.begin(); it != queue.end(); ++it) {
            if (it->quantity == incomingOrder.quantity) {
                Order matched = *it;
                queue.erase(it);
                if (queue.empty()) {
                    sellOrders.erase(levelIt);
                }
                return matched;
            }
        }
    } else {
        auto levelIt = buyOrders.find(incomingOrder.price);
        if (levelIt == buyOrders.end())
            return nullopt;

        auto& queue = levelIt->second;
        for (auto it = queue.begin(); it != queue.end(); ++it) {
            if (it->quantity == incomingOrder.quantity) {
                Order matched = *it;
                queue.erase(it);
                if (queue.empty()) {
                    buyOrders.erase(levelIt);
                }
                return matched;
            }
        }
    }

    return nullopt;
}

optional<Order> OrderBook::partialMatch(const Order& incomingOrder) {
    
    if (incomingOrder.side == Side::BUY) {
        auto levelIt = sellOrders.find(incomingOrder.price);
        if (levelIt == sellOrders.end())
            return nullopt;

        auto& queue = levelIt->second;
        for (auto it = queue.begin(); it != queue.end(); ++it) {
            if (it->quantity < incomingOrder.quantity) {
                Order matched = *it;
                queue.erase(it);
                if (queue.empty()) {
                    sellOrders.erase(levelIt);
                }
                return matched;
            }
        }
    } else {
        auto levelIt = buyOrders.find(incomingOrder.price);
        if (levelIt == buyOrders.end())
            return nullopt;

        auto& queue = levelIt->second;
        for (auto it = queue.begin(); it != queue.end(); ++it) {
            if (it->quantity < incomingOrder.quantity) {
                Order matched = *it;
                queue.erase(it);
                if (queue.empty()) {
                    buyOrders.erase(levelIt);
                }
                return matched;
            }
        }
    }
    return nullopt;
}