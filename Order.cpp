#include "headers/Order.h"

Order::Order(std::string clientOrderID, std::string orderID, Instrument instrument, int side, int quantity, double price)
{
    this->clientOrderID = clientOrderID;
    this->orderID = orderID;
    this->instrument = instrument;
    this->side = side;
    this->quantity = quantity;
    this->price = price;
}

//getters and setters for order
std::string Order::getClientOrderID() const { 
    return clientOrderID; 
}

std::string Order::getOrderID() const {
    return orderID;
}

Instrument Order::getInstrument() const { 
    return instrument; 
}

int Order::getSide() const { 
    return side; 
}

int Order::getQuantity() const { 
    return quantity; 
}

double Order::getPrice() const { 
    return price; 
}

void Order::setQuantity(int newQty) { 
    this->quantity = newQty;                
}