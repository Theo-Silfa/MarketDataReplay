//
//  symbol_order_list.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 17.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#include "symbol_order_list.hpp"

//System includes

//Local includes
#include "order_iterator.hpp"
#include "order_bbo.hpp"
#include "order_request.hpp"

/*************************** Helper Functions *************************/

/*************************** SymbolOrderList **************************/

SymbolOrderList::SymbolOrderList(string symbol) : symbol_(symbol)
{
}

uint64_t SymbolOrderList::totalQuantity()
{
    return 0;
}

const string & SymbolOrderList::symbol()
{
    return symbol_;
}

void SymbolOrderList::add(uint64_t order_id, string side, uint64_t quantity, double price) {
    return;
}

void SymbolOrderList::modify(uint64_t order_id, uint64_t quantity, double price) {
    return;
}

void SymbolOrderList::cancel(uint64_t order_id) {
    return;
}

OrderBbo SymbolOrderList::bbo()
{
    //TODO: use std::multiset::equal_range
    return OrderBbo(0, 0.0, 0, 0, 0.0, 0);
}

OrderVwap SymbolOrderList::vwap(uint64_t quantity)
{
    return {0.0, 0.0};
}

OrderIterator SymbolOrderList::getIterator()
{
    return OrderIterator(orders_buy_, orders_sell_);
}
