#include "headers/Exchange.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

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
}

// Constructor: Initialize the order ID counter and the 5 specific order books 
Exchange::Exchange() {
    currentOrderIdCounter = 1;
    
    // Initialize the 5 required instruments [cite: 64, 68]
    orderBooks.emplace("Rose", OrderBook(Instrument::ROSE));
    orderBooks.emplace("Lavender", OrderBook(Instrument::LAVENDER));
    orderBooks.emplace("Lotus", OrderBook(Instrument::LOTUS));
    orderBooks.emplace("Tulip", OrderBook(Instrument::TULIP));
    orderBooks.emplace("Orchid", OrderBook(Instrument::ORCHID));
}

// Helper to generate IDs like "ord1", "ord2", etc. [cite: 53]
std::string Exchange::generateOrderID() {
    return "ord" + std::to_string(currentOrderIdCounter++);
}

// Helper to convert Status enum to string for the CSV output
std::string statusToString(Status status) {
    switch (status) {
        case Status::NEW: return "New";
        case Status::REJECTED: return "Rejected";
        case Status::FILL: return "Fill";
        case Status::PARTIAL_FILL: return "Pfill";
        case Status::CANCELLED: return "Cancelled";
        default: return "Unknown";
    }
}

// The main processing pipeline
void Exchange::processOrders(const std::string& inputFileName, const std::string& outputFileName) {
    std::ifstream inputFile(inputFileName);
    std::ofstream outputFile(outputFileName);

    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open input file: " << inputFileName << std::endl;
        return;
    }
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open output file: " << outputFileName << std::endl;
        return;
    }

    // Write the header for the execution report [cite: 68]
    outputFile << "Order ID,Client Order ID,Instrument,Side,Exec Status,Quantity,Price,Reason,Transaction Time\n";

    std::string line;
    bool isHeader = true;

    // Read the input CSV line by line
    while (std::getline(inputFile, line)) {
        // Skip the header row of the input file
        if (isHeader) {
            isHeader = false;
            continue;
        }

        // Handle empty lines gracefully
        if (line.empty() || line.find_first_not_of(" \r\n\t") == std::string::npos) {
            continue;
        }

        std::stringstream ss(line);
        std::string clOrdId, instrumentStr, sideStr, qtyStr, priceStr;

        // Parse the CSV fields [cite: 29, 30]
        std::getline(ss, clOrdId, ',');
        std::getline(ss, instrumentStr, ',');
        std::getline(ss, sideStr, ',');
        std::getline(ss, qtyStr, ',');
        std::getline(ss, priceStr, ',');

        // 1. Validation Phase [cite: 247-253]
        std::vector<std::string> row{clOrdId, instrumentStr, sideStr, qtyStr, priceStr};
        ValidatedInput validated;
        try {
            validated = InputValidator::validate(row);
        } catch (const std::exception&) {
            validated.isValid = false;
            validated.rejectReason = "Invalid numeric format";
            validated.clientOrderID = clOrdId;
            validated.instrument = Instrument::INVALID;
            validated.side = 0;
            validated.quantity = 0;
            validated.price = 0.0;
        }

        if (!validated.isValid) {
            // Generate a Rejected report [cite: 254]
            std::string newOrdId = generateOrderID();
            ExecutionReport rejectReport(
                clOrdId,
                newOrdId,
                validated.instrument,
                validated.side,
                validated.quantity,
                validated.price
            );
            rejectReport.setStatus(Status::REJECTED);
            rejectReport.setRejectReason(validated.rejectReason);

            // Write rejection immediately
            outputFile << rejectReport.getOrderID() << ","
                       << rejectReport.getClientOrderID() << ","
                       << instrumentToString(rejectReport.getInstrument()) << ","
                       << rejectReport.getSide() << ","
                       << statusToString(rejectReport.getStatus()) << ","
                       << rejectReport.getQuantity() << ","
                       << std::fixed << std::setprecision(2) << rejectReport.getPrice() << ","
                       << rejectReport.getRejectReason() << ","
                       << rejectReport.getTimestamp() << "\n";
            continue; 
        }

        // 2. Order Creation & Routing
        std::string newOrdId = generateOrderID();
        Order newOrder(
            validated.clientOrderID,
            newOrdId,
            validated.instrument,
            validated.side,
            validated.quantity,
            validated.price
        );

        // Pass the order to the correct OrderBook.
        auto orderBookIt = orderBooks.find(instrumentStr);
        if (orderBookIt != orderBooks.end()) {
            std::vector<ExecutionReport> reports = orderBookIt->second.processOrder(newOrder);

            // 3. Output Phase
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
    }

    inputFile.close();
    outputFile.close();
}