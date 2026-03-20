#include "../headers/csvOrderReader.hpp"
#include <bits/stdc++.h>
using namespace std;

CSVOrderReader::CSVOrderReader(const string& filePath) : filePath(filePath) {}

vector<string> CSVOrderReader::splitCsvLine(const string &line) {
    vector<string> fields;
    stringstream ss(line);
    string field;

    while (getline(ss, field, ',')) {
        fields.push_back(field);
    }
    return fields;
}

bool CSVOrderReader::isHeader(const vector<string>& fields) {
    return !fields.empty() && fields[0] == "Cl. Ord.ID";
}

Order CSVOrderReader::parseOrder(const vector<string>& fields, size_t lineNumber) {
    // This function validates the fields and constructs an Order object
    // If any validation fails, it sets the reason field of the Order to indicate the error
    string reason;
    if (fields.size() != 5 && reason.empty()) {
        reason = "Invalid number of fields";
    }

    static const unordered_set<string> allowed = {"Rose", "Lavender", "Tulip", "Lotus", "Orchid"};
    if (!allowed.count(fields[1]) && reason.empty()) {
        reason = "Invalid instrument";
    }

    int sideValue = stoi(fields[2]);
    if (sideValue != 1 && sideValue != 2 && reason.empty()) {
        reason = "Invalid side";
    }

    int quantity = stoi(fields[3]);
    double price = stod(fields[4]);
    if (quantity < 10 || quantity > 1000 && reason.empty()) {
        reason = "Invalid size";
    }

    if (price <= 0 && reason.empty()) {
        reason = "Invalid price";
    }

    return Order{"",fields[0], fields[1], sideValue == 1 ? Side::BUY : Side::SELL, quantity, price, reason};
}

vector<Order> CSVOrderReader::readOrders() {
    vector<Order> orders;
    ifstream file(filePath);
    string line;
    size_t lineNumber = 0;

    if (!file.is_open()) {
        throw runtime_error("Could not open file: " + filePath);
    }

    while (getline(file, line)) {
        lineNumber++;

        if (line.empty()) {
            continue;
        }
        auto fields = splitCsvLine(line);
        if (isHeader(fields)) {
            continue;
        }

        orders.push_back(parseOrder(fields, lineNumber));
    }

    return orders;
}
    