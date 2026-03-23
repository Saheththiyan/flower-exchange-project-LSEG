#include "InputValidator.h"

Instrument InputValidator::stringToInstrument(const std::string& flower) {
    if (flower == "Rose") return Instrument::ROSE; 
    if (flower == "Lavender") return Instrument::LAVENDER; 
    if (flower == "Lotus") return Instrument::LOTUS; 
    if (flower == "Tulip") return Instrument::TULIP; 
    if (flower == "Orchid") return Instrument::ORCHID; 
    return Instrument::INVALID;
}

ValidatedInput InputValidator::validate(const std::vector<std::string>& row) {
    ValidatedInput res;
    res.isValid = true;

    if (row.size() < 5) {
        res.isValid = false;
        res.rejectReason = "Required fields not available"; 
        return res;
    }

    for (const auto& field : row) {
        if (field.empty()) {
            res.isValid = false;
            res.rejectReason = "Required fields not available"; 
        }
    }

    if (row[0].length() > 8) { 
        res.isValid = false;
        res.rejectReason = "Invalid client order ID";
        return res;
    }
    res.clientOrderID = row[0];

    res.instrument = stringToInstrument(row[1]);
    if (res.instrument == Instrument::INVALID) { 
        res.isValid = false;
        res.rejectReason = "Invalid instrument"; 
        return res;
    }

    int side = std::stoi(row[2]);
    if (side != 1 && side != 2) { 
        res.isValid = false;
        res.rejectReason = "Invalid side"; 
        return res;
    }
    res.side = side;

    int quantity = std::stoi(row[3]);
    if (quantity % 10 != 0 || quantity < 10 || quantity > 1000) { 
        res.isValid = false;
        res.rejectReason = "Invalid quantity"; 
        return res;
    }
    res.quantity = quantity;

    int price = std::stod(row[4]);
    if (price <= 0.0) { 
        res.isValid = false;
        res.rejectReason = "Invalid price"; 
        return res;
    }
    res.price = price;

    return res;
}

