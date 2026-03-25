#include "headers/InputValidator.h"

Instrument InputValidator::stringToInstrument(const std::string& flower) {
    if (flower== "Rose") return Instrument::ROSE; 
    if (flower== "Lavender") return Instrument::LAVENDER; 
    if (flower== "Lotus") return Instrument::LOTUS; 
    if (flower== "Tulip") return Instrument::TULIP; 
    if (flower== "Orchid") return Instrument::ORCHID; 
    return Instrument::INVALID;
}

//funtion to validate input and give relevant rejection msg
ValidatedInput InputValidator::validate(const std::vector<std::string>& row) {
    ValidatedInput res;
    res.isValid = true;

    if (row[0].empty() || row[0].length() >= 8) {
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

    if (row[2].empty()) {
        res.isValid = false;
        res.rejectReason = "Invalid side";
        return res;
    }
    try {
        int side = std::stoi(row[2]);
        if (side != 1 && side != 2) {
            res.isValid = false;
            res.rejectReason = "Invalid side";
            return res;
        }
        res.side = side;
    } catch (const std::exception&) {
        res.isValid = false;
        res.rejectReason = "Invalid side";
        return res;
    }

    if (row[3].empty()) {
        res.isValid = false;
        res.rejectReason = "Invalid size";
        return res;
    }
    try {
        int quantity = std::stoi(row[3]);  //string to int conversion
        if (quantity % 10 != 0 || quantity < 10 || quantity >= 1000) {
            res.isValid = false;
            res.rejectReason = "Invalid size";
            return res;
        }
        res.quantity = quantity;
    } catch (const std::exception&) {
        res.isValid = false;
        res.rejectReason = "Invalid size";
        return res;
    }

    if (row[4].empty()) {
        res.isValid = false;
        res.rejectReason = "Invalid price";
        return res;
    }
    try {
        double price = std::stod(row[4]);  //string to double conversion
        if (price <= 0.0) {
            res.isValid = false;
            res.rejectReason = "Invalid price";
            return res;
        }
        res.price = price;
    } catch (const std::exception&) {
        res.isValid = false;
        res.rejectReason = "Invalid price";
        return res;
    }

    return res;
}
