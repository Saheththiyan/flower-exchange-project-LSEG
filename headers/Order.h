#ifndef ORDER_H
#define ORDER_H

#include <string>
#include "Constants.h"

class Order {
private:
    std::string clientOrderID;
    std::string orderID;
    Instrument instrument;
    int side;
    int quantity;
    double price;

public:
    Order(std::string clID, std::string ordID, Instrument instruent, int side, int quantity, double price);

    std::string getClientOrderID() const;
    std::string getOrderID() const;
    Instrument getInstrument() const;
    int getSide() const;
    int getQuantity() const;
    double getPrice() const;

    void setQuantity(int newQty);
};

#endif