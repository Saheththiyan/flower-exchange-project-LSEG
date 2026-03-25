#include "headers/Exchange.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <utility>

namespace {
std::string instrumentToString(Instrument instrument) {
    switch (instrument) {
        case Instrument::ROSE: return "Rose";
        case Instrument::LAVENDER: return "Lavender";
        case Instrument::LOTUS: return "Lotus";
        case Instrument::TULIP: return "Tulip";
        case Instrument::ORCHID: return "Orchid";
        default: return "Invalid";
    }
}

bool parser(const std::string& line, std::string fields[5]) {
    std::size_t start = 0;
    for (int i = 0; i < 4; ++i) {
        const std::size_t comma = line.find(',', start);
        if (comma == std::string::npos) {
            return false;
        }
        fields[i] = line.substr(start, comma - start);
        start = comma + 1;
    }
    fields[4] = line.substr(start);
    return true;
}
}


Exchange::Exchange()
    : orderBooks{               //orderbooks per instrument
        OrderBook(Instrument::ROSE),
        OrderBook(Instrument::LAVENDER),
        OrderBook(Instrument::LOTUS),
        OrderBook(Instrument::TULIP),
        OrderBook(Instrument::ORCHID)
      },
      orderCounter(1) {}

std::size_t Exchange::instrumentToIndex(Instrument instrument) {
    switch (instrument) {
        case Instrument::ROSE: return 0;
        case Instrument::LAVENDER: return 1;
        case Instrument::LOTUS: return 2;
        case Instrument::TULIP: return 3;
        case Instrument::ORCHID: return 4;
        default: return 5; // Invalid index
    }
}

//generate orderIDs
std::string Exchange::generateOrderID() {
    return "ord" + std::to_string(orderCounter++);
}

std::string statusToString(Status status) {
    switch (status) {
        case Status::NEW: return "New";
        case Status::REJECTED: return "Rejected";
        case Status::FILL: return "Fill";
        case Status::PARTIAL_FILL: return "Pfill";
        default: return "Unknown";
    }
}


void Exchange::processOrders(const std::string& inputFileName, const std::string& outputFileName) {
    std::ifstream inputFile(inputFileName);
    std::ofstream outputFile(outputFileName);

    //handle file opening errors
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open input file: " << inputFileName << '\n';
        return;
    }
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open output file: " << outputFileName << '\n';
        return;
    }

    outputFile << "Order ID,Client Order ID,Instrument,Side,Exec Status,Quantity,Price,Reason,Transaction Time\n";

    std::string line;
    bool isHeader = true;

    
    while (std::getline(inputFile, line)) {
        if (isHeader) {
            isHeader = false;
            continue;
        }

        std::string fields[5];
        if (!parser(line, fields)) {   //use parser to get 5 values of the fields in an order
            continue;
        }
        std::string& clOrdId = fields[0];
        std::string& instrumentStr = fields[1];
        std::string& sideStr = fields[2];
        std::string& qtyStr = fields[3];
        std::string& priceStr = fields[4];
        const std::string eventTimestamp = ExecutionReport::createTimestamp();

        //use innput validation
        std::vector<std::string> row{clOrdId, instrumentStr, sideStr, qtyStr, priceStr};
        ValidatedInput validated;
        try {
            validated = InputValidator::validate(row);
        } catch (const std::exception&) {
            validated.isValid = false;
        }

        if (!validated.isValid) {
            std::string newOrdId = generateOrderID();
            ExecutionReport rejectReport(
                clOrdId,
                newOrdId,
                Instrument::ROSE,
                1,
                0,
                0.0,   //filler values for rejected orders
                eventTimestamp
            );
            rejectReport.setStatus(Status::REJECTED);
            rejectReport.setRejectReason(validated.rejectReason);

            //writing the rejected row error to ouput file
            outputFile << rejectReport.getOrderID() << ","
                       << rejectReport.getClientOrderID() << ","
                       << instrumentStr << ","
                       << sideStr << ","
                       << statusToString(rejectReport.getStatus()) << ","
                       << qtyStr << ","
                       << priceStr<< ","
                       << rejectReport.getRejectReason() << ","
                       << rejectReport.getTimestamp() << "\n";
            continue; 
        }

        std::string newOrdId = generateOrderID();
        Order newOrder(
            validated.clientOrderID,
            newOrdId,
            validated.instrument,
            validated.side,
            validated.quantity,
            validated.price
        );

        std::vector<ExecutionReport> reports = orderBooks[instrumentToIndex(validated.instrument)]
            .matchOrders(std::move(newOrder), eventTimestamp);

            //write to putput file th valid roders
        for (const auto& report : reports) {
            outputFile << report.getOrderID() << ","
                       << report.getClientOrderID() << ","
                       << instrumentToString(report.getInstrument()) << ","
                       << report.getSide() << ","
                       << statusToString(report.getStatus()) << ","
                       << report.getQuantity() << ","
                       << std::fixed << std::setprecision(2) << report.getPrice() << ","
                       << report.getRejectReason() << ","
                       << report.getTimestamp() << "\n";
        }
    }
//close all opened files
    inputFile.close();
    outputFile.close();
}