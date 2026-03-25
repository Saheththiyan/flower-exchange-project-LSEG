#include "headers/ExecutionReport.h"
#include <chrono>   //for time tracking 
#include <iomanip>  //io manipulation for setfill('0') set precision 
#include <sstream>

std::string ExecutionReport::createTimestamp() {
    auto now = std::chrono::system_clock::now();

    auto m_sec = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;  //time_since_epoch jan 1st 1970 

    std::time_t t = std::chrono::system_clock::to_time_t(now);

    
    std::tm *tm = std::localtime(&t);
    std::stringstream ss;
    
    ss << std::put_time(tm, "%Y%m%d-%H%M%S");  //use stringstream to get the output format needed
    ss << '.' << std::setfill('0') << std::setw(3) << m_sec.count();
    
    return ss.str(); 
}

ExecutionReport::ExecutionReport(
    std::string clientOrderID,
    std::string orderID,
    Instrument instrument,
    int side,
    int quantity,
    double price,
    const std::string& reportTimestamp
) {
    this->clientOrderID = clientOrderID;
    this->orderID = orderID;
    this->instrument = instrument;
    this->side = side;
    this->quantity = quantity;
    this->price = price;
    this->status = Status::NEW;
    this->rejectReason = "";
    this->timestamp = reportTimestamp.empty() ? ExecutionReport::createTimestamp() : reportTimestamp; //if empty create timestamp using before function
}

//getters and setters for execution report
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