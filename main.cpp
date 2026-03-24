#include "headers/Exchange.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string inputFile = "order.csv";
    std::string outputFile = "execution_rep.csv";

    if (argc >= 2) {
        inputFile = argv[1];
    }
    if (argc >= 3) {
        outputFile = argv[2];
    }

    Exchange exchange;
    exchange.processOrders(inputFile, outputFile);

    std::cout << "Processed orders from '" << inputFile
              << "' into '" << outputFile << "'." << std::endl;

    return 0;
}
