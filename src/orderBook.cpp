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
    // This function attempts to match the incoming order against the opposite side of the book
    // Returns empty vector if no match, otherwise returns the list of fills that occurred
    vector<TradeFill> fills;

    // Assumed prices less than or equal to 5.00 are aggressive orders
    const bool aggressive = (incomingOrder.price <= 5.00);

    if (incomingOrder.side == Side::BUY) {
        while (incomingOrder.quantity > 0 && !sellOrders.empty()) {
            auto levelIt = sellOrders.begin();
            double bestPrice = levelIt->first;
             
            // Breaks if the best price is not equal to incoming order price
            // But we could change to bestPrice < incomingOrder.price to have a better trade execution
            if (!aggressive && bestPrice != incomingOrder.price) {
                break;
            }

            auto& queue = levelIt->second;
            while (incomingOrder.quantity > 0 && !queue.empty()) {
                Order& restingOrder = queue.front();
                int trade = min(incomingOrder.quantity, restingOrder.quantity);

                fills.push_back(TradeFill{restingOrder, trade, bestPrice});

                // Remove the traded quantity from both orders
                incomingOrder.quantity -= trade;
                restingOrder.quantity -= trade;

                // If the resting order is fully filled, remove it from the queue
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

            // Same as above, but for sell orders
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