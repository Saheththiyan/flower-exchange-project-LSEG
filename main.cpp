#include "headers/Exchange.h"
#include <iostream>
#include <string>

//to bring together all the components

int main(int argc, char* argv[]) {
    std::string inputFile = "order.csv";
    std::string outputFile = "execution_rep.csv";

 //for user to specify input in the terminal (if not specified it'll search for default names) 
    if (argc >= 2) {
        inputFile = argv[1];       // ./exchange_app is argv[0]
    }
    if (argc >= 3) {
        outputFile = argv[2];
    }

    Exchange exchange;
    exchange.processOrders(inputFile, outputFile);

    std::cout << "Processed orders from '" << inputFile
              << "' into '" << outputFile << "'." << '\n';

    return 0;
}

