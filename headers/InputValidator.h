#ifndef INPUT_VALIDATOR_H
#define INPUT_VALIDATOR_H

#include <string>
#include <vector>
#include "Constants.h"

struct ValidatedInput {
    bool isValid;
    std::string rejectReason;
    std::string clientOrderID;
    Instrument instrument;
    int side;
    int quantity;
    double price;
};

class InputValidator {
public:
    static ValidatedInput validate(const std::vector<std::string>& row);   //can get validated input and status together
private:
    static Instrument stringToInstrument(const std::string& str);  // convert string input to enum
};

#endif