#include "headers/OrderBook.h"

OrderBook::OrderBook(Instrument inst) : instrument(inst) {}

std::vector<ExecutionReport> OrderBook::processOrder(const Order& order) {
    std::vector<ExecutionReport> reports;
    Order incoming = order;
    bool hadMatch = false;

    std::list<Order>& oppositeSide = (incoming.getSide() == 1) ? sellSide : buySide;

    while (!oppositeSide.empty() && incoming.getQuantity() > 0) {
        Order& resting = oppositeSide.front();

        const bool crossed = (incoming.getSide() == 1)
            ? (incoming.getPrice() >= resting.getPrice())
            : (incoming.getPrice() <= resting.getPrice());

        if (!crossed) {
            break;
        }

        hadMatch = true;

        const int fillQty = std::min(incoming.getQuantity(), resting.getQuantity());
        const double executionPrice = resting.getPrice();

        ExecutionReport aggressiveReport(
            incoming.getClientOrderID(),
            incoming.getOrderID(),
            incoming.getInstrument(),
            incoming.getSide(),
            fillQty,
            executionPrice
        );
        aggressiveReport.setStatus((fillQty == incoming.getQuantity()) ? Status::FILL : Status::PARTIAL_FILL);
        reports.push_back(aggressiveReport);

        ExecutionReport passiveReport(
            resting.getClientOrderID(),
            resting.getOrderID(),
            resting.getInstrument(),
            resting.getSide(),
            fillQty,
            executionPrice
        );
        passiveReport.setStatus((fillQty == resting.getQuantity()) ? Status::FILL : Status::PARTIAL_FILL);
        reports.push_back(passiveReport);

        incoming.setQuantity(incoming.getQuantity() - fillQty);
        resting.setQuantity(resting.getQuantity() - fillQty);

        if (resting.getQuantity() == 0) {
            oppositeSide.pop_front();
        }
    }

    if (incoming.getQuantity() > 0) {
        if (incoming.getSide() == 1) {
            insertBuy(incoming);
        } else {
            insertSell(incoming);
        }
        if (!hadMatch) {
            ExecutionReport newReport(
                incoming.getClientOrderID(),
                incoming.getOrderID(),
                incoming.getInstrument(),
                incoming.getSide(),
                incoming.getQuantity(),
                incoming.getPrice()
            );
            newReport.setStatus(Status::NEW);
            reports.push_back(newReport);
        }
    }

    return reports;
}

void OrderBook::insertBuy(const Order& order) {
    auto i = buySide.begin();
    while (i != buySide.end() && i->getPrice() >= order.getPrice())
        ++i;
    buySide.insert(i, order);
}

void OrderBook::insertSell(const Order& order) {
    auto i = sellSide.begin();
    while (i != sellSide.end() && i->getPrice() <= order.getPrice())
        ++i;
    sellSide.insert(i, order);
}
void OrderBook::cancelOrder(const std::string& clientOrderID) {
    for (auto it = buySide.begin(); it != buySide.end(); ++it) {
        if (it->getClientOrderID() == clientOrderID) {
            buySide.erase(it);
            return;
        }
    }

    for (auto it = sellSide.begin(); it != sellSide.end(); ++it) {
        if (it->getClientOrderID() == clientOrderID) {
            sellSide.erase(it);
            return;
        }
    }
}

const std::list<Order>& OrderBook::getBuySide() const {
    return buySide;
}

const std::list<Order>& OrderBook::getSellSide() const {
    return sellSide;
}