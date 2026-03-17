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
    if (fields.size() != 5) {
        throw runtime_error("Invalid number of fields at line " + to_string(lineNumber));
    }

    static const unordered_set<string> allowed = {"Rose", "Lavender", "Tulip", "Lotus", "Orchid"};
    if (!allowed.count(fields[1])) {
        throw runtime_error("Invalid instrument at line " + to_string(lineNumber));
    }

    int sideValue = stoi(fields[2]);
    if (sideValue != 1 && sideValue != 2) {
        throw runtime_error("Invalid side at line " + to_string(lineNumber));
    }

    int quantity = stoi(fields[3]);
    double price = stod(fields[4]);
    if (quantity <= 0) {
        throw runtime_error("Quantity must be positive at line " + to_string(lineNumber));
    }

    if (price <= 0) {
        throw runtime_error("Price must be positive at line " + to_string(lineNumber));
    }

    return Order{fields[0], fields[1], sideValue == 1 ? Side::BUY : Side::SELL, quantity, price};
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
    