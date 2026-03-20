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

vector<TradeFill> OrderBook::execute(Order& incomingOrder) {
    vector<TradeFill> fills;
    const bool aggressive = (incomingOrder.price <= 5.0);

    if (incomingOrder.side == Side::BUY) {
        while (incomingOrder.quantity > 0 && !sellOrders.empty()) {
            auto levelIt = sellOrders.begin();
            double bestPrice = levelIt->first;

            if (!aggressive && bestPrice != incomingOrder.price) {
                break;
            }

            auto& queue = levelIt->second;
            while (incomingOrder.quantity > 0 && !queue.empty()) {
                Order& restingOrder = queue.front();
                int trade = min(incomingOrder.quantity, restingOrder.quantity);

                fills.push_back(TradeFill{restingOrder, trade, bestPrice});
                incomingOrder.quantity -= trade;
                restingOrder.quantity -= trade;

                if (restingOrder.quantity == 0) {
                    queue.pop_front();
                }
            }

            if (queue.empty()) {
                sellOrders.erase(levelIt);
            }
        }
    } else {
        while (incomingOrder.quantity > 0 && !buyOrders.empty()) {
            auto levelIt = buyOrders.begin();
            double bestPrice = levelIt->first;

            if (!aggressive && bestPrice != incomingOrder.price) {
                break;
            }

            auto& queue = levelIt->second;
            while (incomingOrder.quantity > 0 && !queue.empty()) {
                Order& restingOrder = queue.front();
                int trade = min(incomingOrder.quantity, restingOrder.quantity);

                fills.push_back(TradeFill{restingOrder, trade, bestPrice});
                incomingOrder.quantity -= trade;
                restingOrder.quantity -= trade;

                if (restingOrder.quantity == 0) {
                    queue.pop_front();
                }
            }

            if (queue.empty()) {
                buyOrders.erase(levelIt);
            }
        }
    }
    return fills;
}