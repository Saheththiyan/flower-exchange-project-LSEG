#pragma once

#include <bits/stdc++.h>
#include "order.hpp"
using namespace std;

class OrderBook {
private:
    map<double, deque<Order>, greater<double>> buyOrders;
    map<double, deque<Order>> sellOrders;

public:
    void addOrder(const Order& order);

    vector<Order> getBuyOrders() const;
    vector<Order> getSellOrders() const;
    optional<Order> tryMatch(const Order& incomingOrder);
};