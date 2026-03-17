#include "../headers/executionReportWriter.hpp"
#include <bits/stdc++.h>
using namespace std;

ExecutionReportWriter::ExecutionReportWriter(const string& filePath) : filePath(filePath) {}

string ExecutionReportWriter::statustoString(ExecStatus status) {
    switch (status) {
        case ExecStatus::New: return "New";
        case ExecStatus::PFill: return "PFill";
        case ExecStatus::Fill: return "Fill";
        case ExecStatus::Reject: return "Reject";
    } 

    return "Reject";
}

int ExecutionReportWriter::sidetoInt(Side side) {
    switch (side) {
        case Side::BUY: return 1;
        case Side::SELL: return 2;
    }

    return 0;
}

void ExecutionReportWriter::writeHeader() {
    ofstream file(filePath);
    if (!file.is_open()) {
        throw runtime_error("Could not open file: " + filePath);
    }

    file << "Order ID, Cl. Ord.ID,Instrument,Side,Exec_Status,Quantity,Price\n";
}

void ExecutionReportWriter::writeRows(const vector<ExecutionReport> &rows) {
    ofstream out(filePath, ios::app);
    if (!out.is_open()) throw runtime_error("Could not open file: " + filePath);

    for (const auto& report : rows) {
        out << report.orderID << ","
            << report.clientOrderID << ","
            << report.instrument << ","
            << sidetoInt(report.side) << ","
            << statustoString(report.execStatus) << ","
            << report.quantity << ","
            << fixed << setprecision(2) << report.price << "\n";
    }
}