#pragma once

#include <bits/stdc++.h>
#include "order.hpp"
using namespace std;

class OrderBook {
private:
    vector<Order> buyOrders;
    vector<Order> sellOrders;

public:
    void addOrder(const Order& order);

    const vector<Order>& getBuyOrders() const;
    const vector<Order>& getSellOrders() const;
    optional<Order> tryMatch(const Order& incomingOrder);
};