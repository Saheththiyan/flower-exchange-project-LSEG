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

        constexpr int orderIdWidth = 12;
        constexpr int qtyWidth = 10;
        constexpr int priceWidth = 10;
        constexpr int sideWidth = orderIdWidth + qtyWidth + priceWidth;

        cout << left << setw(sideWidth) << "BUY BOOK (Price desc, FIFO)"
             << " | "
             << "SELL BOOK (Price asc, FIFO)" << "\n";

        cout << left << setw(orderIdWidth) << "OrderID"
             << setw(qtyWidth) << "Qty"
             << setw(priceWidth) << "Price"
             << " | "
             << setw(orderIdWidth) << "OrderID"
             << setw(qtyWidth) << "Qty"
             << setw(priceWidth) << "Price" << "\n";

        size_t rowCount = max(buy.size(), sell.size());
        if (rowCount == 0) {
            cout << left << setw(sideWidth) << "(empty)"
                 << " | "
                 << "(empty)" << "\n";
            continue;
        }

        for (size_t i = 0; i < rowCount; ++i) {
            if (i < buy.size()) {
                cout << left << setw(orderIdWidth) << buy[i].orderID
                     << setw(qtyWidth) << buy[i].quantity
                     << setw(priceWidth) << fixed << setprecision(2) << buy[i].price;
            } else {
                cout << left << setw(orderIdWidth) << ""
                     << setw(qtyWidth) << ""
                     << setw(priceWidth) << "";
            }

            cout << " | ";

            if (i < sell.size()) {
                cout << left << setw(orderIdWidth) << sell[i].orderID
                     << setw(qtyWidth) << sell[i].quantity
                     << setw(priceWidth) << fixed << setprecision(2) << sell[i].price;
            } else {
                cout << left << setw(orderIdWidth) << ""
                     << setw(qtyWidth) << ""
                     << setw(priceWidth) << "";
            }

            cout << "\n";
        }
    }
}