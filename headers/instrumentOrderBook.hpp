#pragma once

#include <bits/stdc++.h>
#include "orderBook.hpp"
using namespace std;

class InstrumentOrderBook {
private:
    unordered_map<string, OrderBook> books;

public:
    OrderBook& getOrderBook(const string& instrument) {
        return books[instrument];
    }

    vector<string> getInstruments() const;
    void printAvailableOrderBooks() const;
};