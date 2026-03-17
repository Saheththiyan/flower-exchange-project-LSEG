#pragma once

#include <bits/stdc++.h>
using namespace std;

enum class Side {
    BUY = 1,
    SELL = 2
};

enum class ExecStatus {
    New,
    PFill,
    Fill,
    Reject
};

struct Order {
    string clientOrderID;
    string instrument;
    Side side;
    int quantity;
    double price;
};

struct ExecutionReport {
    string orderID;
    string clientOrderID;
    string instrument;
    ExecStatus execStatus;
    Side side;
    int quantity;
    double price;
};
