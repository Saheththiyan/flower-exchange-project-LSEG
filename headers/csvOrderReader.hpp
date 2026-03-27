#pragma once

#include <bits/stdc++.h>
#include "order.hpp"
using namespace std;

class OrderReader {
public:
    virtual ~OrderReader() = default;
    virtual vector<Order> readOrders() = 0;
};

class CSVOrderReader : public OrderReader {
private:
    string filePath;

    static vector<string> splitCsvLine(const string &line);
    static Order parseOrder(const vector<string>& fields, size_t lineNumber);
    static bool isHeader(const vector<string>& fields);

public:
    explicit CSVOrderReader(const string& filePath);
    vector<Order> readOrders() override;
};