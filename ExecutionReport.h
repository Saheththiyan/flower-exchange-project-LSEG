#ifndef EXECUTION_REPORT_H
#define EXECUTION_REPORT_H

#include <string>
#include <ctime>
#include "Constants.h"

enum class Status {
    NEW, PARTIAL_FILL, FILL, CANCELLED, REJECTED
};

class ExecutionReport {
private:
    std::string clientOrderID;
    std::string orderID;
    Instrument instrument;
    int side;
    int quantity;
    double price;
    Status status;
    std::string rejectReason;
    std::string timestamp;


public:
    ExecutionReport(std::string clID, std::string ordID, Instrument instrument, int side, int quantity, double price);

    std::string getClientOrderID() const;
    std::string getOrderID() const;
    Instrument getInstrument() const;
    int getSide() const;
    int getQuantity() const;
    double getPrice() const;
    Status getStatus() const;
    void setQuantity(int newQty);
    void setStatus(Status newStatus);
    std::string getRejectReason() const { return rejectReason; }
    std::string getTimestamp() const { return timestamp; }
    void setRejectReason(const std::string& reason) { rejectReason = reason; }
};

#endif