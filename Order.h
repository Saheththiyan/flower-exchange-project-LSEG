#ifndef ORDER_H
#define ORDER_H

#include <string>
#include "Constants.h"

class Order {
private:
    std::string clientOrderID;
    Instrument instrument;
    int side;
    int quantity;
    double price;

public:
    Order(std::string clID, Instrument instruent, int side, int quantity, double price);

    std::string getClientOrderID() const;
    Instrument getInstrument() const;
    int getSide() const;
    int getQuantity() const;
    double getPrice() const;

    void setQuantity(int newQty);
};

#endif