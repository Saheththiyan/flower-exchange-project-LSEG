#include "ExecutionReport.h"
#include <chrono>
#include <iomanip>
#include <sstream>

std::string generateTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    auto timer = std::chrono::system_clock::to_time_t(now);
    
    std::tm bt = *std::localtime(&timer);
    std::ostringstream oss;
    
    oss << std::put_time(&bt, "%Y%m%d-%H%M%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    
    return oss.str();
}

ExecutionReport::ExecutionReport(std::string clID, std::string ordID, Instrument instrument, int side, int quantity, double price)
    : clientOrderID(clID), orderID(ordID), instrument(instrument), side(side), quantity(quantity), price(price) 
{
    this->status = Status::NEW;
    this->rejectReason = "";
    this->timestamp = generateTimestamp();
}

std::string ExecutionReport::getClientOrderID() const { return clientOrderID; }
std::string ExecutionReport::getOrderID() const { return orderID; }
Instrument ExecutionReport::getInstrument() const { return instrument; }
int ExecutionReport::getSide() const { return side; }
int ExecutionReport::getQuantity() const { return quantity; }
double ExecutionReport::getPrice() const { return price; }
Status ExecutionReport::getStatus() const { return status; }

void ExecutionReport::setQuantity(int newQty) { 
    this->quantity = newQty; 
}

void ExecutionReport::setStatus(Status newStatus) { 
    this->status = newStatus; 
}