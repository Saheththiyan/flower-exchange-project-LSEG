#include  "../headers/instrumentOrderBook.hpp"
using namespace std;

vector<string> InstrumentOrderBook::getInstruments() const {
    vector<string> instruments;
    instruments.reserve(books.size());

    for (const auto& [instrument, book] : books) {
        instruments.push_back(instrument);
    }

    sort(instruments.begin(), instruments.end());
    return instruments;
}

void InstrumentOrderBook::printAvailableOrderBooks() const {
    auto instruments = getInstruments();

    cout << "\n=== Instrument Order Books ===\n";
    for (const auto& instrument : instruments) {
        const auto& book = books.at(instrument);
        auto buy = book.getBuyOrders();   // already desc by price
        auto sell = book.getSellOrders(); // already asc by price

        cout << "\nInstrument: " << instrument << "\n";

        cout << "BUY BOOK (Price desc, FIFO)\n";
        cout << left << setw(12) << "OrderID" << setw(10) << "Qty" << setw(10) << "Price" << "\n";
        if (buy.empty()) cout << "(empty)\n";
        for (const auto& o : buy) {
            cout << left << setw(12) << o.orderID
                 << setw(10) << o.quantity
                 << fixed << setprecision(2) << o.price << "\n";
        }

        cout << "SELL BOOK (Price asc, FIFO)\n";
        cout << left << setw(12) << "OrderID" << setw(10) << "Qty" << setw(10) << "Price" << "\n";
        if (sell.empty()) cout << "(empty)\n";
        for (const auto& o : sell) {
            cout << left << setw(12) << o.orderID
                 << setw(10) << o.quantity
                 << fixed << setprecision(2) << o.price << "\n";
        }
    }
}