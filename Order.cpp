#include "Order.h"

Order::Order(std::string clientOrderID, Instrument instrument, int side, int quantity, double price)
{
    this->clientOrderID = clientOrderID;
    this->instrument = instrument;
    this->side = side;
    this->quantity = quantity;
    this->price = price;
}

std::string Order::getClientOrderID() const { 
    return clientOrderID; 
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