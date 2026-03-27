#include "headers/OrderBook.h"
#include <algorithm>  //to get min function

OrderBook::OrderBook(Instrument inst) : instrument(inst) {}

std::vector<ExecutionReport> OrderBook::matchOrders(Order in, const std::string& eventTimestamp) {
    std::vector<ExecutionReport> reports;
    reports.reserve(4);
    bool hadMatch = false;

    std::deque<Order>& opp = (in.getSide() == 1) ? sellSide : buySide;  //if we in buy order we have to check sell side of book

    while (!opp.empty() && in.getQuantity() > 0) {
        Order& top = opp.front();

        bool crossed = false;

        if (in.getSide() == 1) {  // BUY
            if (in.getPrice() >= top.getPrice()) {
                crossed = true;
            }
        } else {                  // SELL
            if (in.getPrice() <= top.getPrice()) {
                crossed = true;
            }
        }  //buying price should be greter than selling price for a trade

        if (!crossed) break;

        hadMatch = true;

        const int fillQty = std::min(in.getQuantity(), top.getQuantity());
        const double executionPrice = top.getPrice();

//two emplaces for both buy and sell side reports

        reports.emplace_back(           //use emplace_back instead of push_back to avaoid unnecessary copying
            in.getClientOrderID(),
            in.getOrderID(),
            in.getInstrument(),
            in.getSide(),
            fillQty,
            executionPrice,
            eventTimestamp
        );
        reports.back().setStatus((fillQty == in.getQuantity()) ? Status::FILL : Status::PARTIAL_FILL);  //if input qty same as exeuted qty as here its fill else pfill

        reports.emplace_back(
            top.getClientOrderID(),
            top.getOrderID(),
            top.getInstrument(),
            top.getSide(),
            fillQty,
            executionPrice,
            eventTimestamp
        );
        reports.back().setStatus((fillQty == top.getQuantity()) ? Status::FILL : Status::PARTIAL_FILL);

        //update new qty
        in.setQuantity(in.getQuantity() - fillQty);
        top.setQuantity(top.getQuantity() - fillQty);

        if (top.getQuantity() == 0) { //remove if fully filled
            opp.pop_front();
        }
    }

    if (in.getQuantity() > 0) {
        if (in.getSide() == 1) {  //if not fill,pfill then its a New order
            insertBuy(in);
        } else {
            insertSell(in);
        }
        if (!hadMatch) {
            reports.emplace_back(
                in.getClientOrderID(),
                in.getOrderID(),
                in.getInstrument(),
                in.getSide(),
                in.getQuantity(),
                in.getPrice(),
                eventTimestamp
            );
            reports.back().setStatus(Status::NEW);
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


const std::deque<Order>& OrderBook::getBuySide() const {
    return buySide;
}

const std::deque<Order>& OrderBook::getSellSide() const {
    return sellSide;
}