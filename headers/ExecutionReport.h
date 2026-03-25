#ifndef EXECUTION_REPORT_H
#define EXECUTION_REPORT_H

#include <string>
#include <ctime>
#include "Constants.h"

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
    ExecutionReport(              //report constructor
        std::string clientOrderID,
        std::string orderID,
        Instrument instrument,
        int side,
        int quantity,
        double price,               //reason not in constructor as it is not compulsory
        const std::string& reportTimestamp = ""
    );

    static std::string createTimestamp();        //can use without an instance/object

    std::string getClientOrderID() const;
    std::string getOrderID() const;
    Instrument getInstrument() const;
    int getSide() const;
    int getQuantity() const;
    double getPrice() const;
    Status getStatus() const;
    void setQuantity(int newQty);
    void setStatus(Status newStatus);
    void setTimestamp(const std::string& newTimestamp) { timestamp = newTimestamp; }
    std::string getRejectReason() const { return rejectReason; }
    std::string getTimestamp() const { return timestamp; }
    void setRejectReason(const std::string& reason) { rejectReason = reason; }   //using pass by reference to avoid copying
};

#endif