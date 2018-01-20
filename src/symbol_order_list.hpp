//
//  symbol_order_list.hpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 17.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//


#ifndef _SYMBOLORDERLIST_H
#define _SYMBOLORDERLIST_H

//System includes
#include <set>
#include <string>

//Local includes
#include "defines.h"
#include "container_definitions.hpp"

using namespace std;

//Forward declarations
struct OrderRequest;
struct OrderVwap;
class OrderBbo;
class OrderIterator;

/**
 * Order List class. Only orders for specific symbol are held.
 * Automatically updates the BBO information upon the order
 * receival, modification or cancel.
 */
class SymbolOrderList
{
public:
    /**
     * Constructor.
     * @param symbol which is associated with this object
     */
    SymbolOrderList(string symbol);

    /** Default destructor */
    ~SymbolOrderList() = default;

    /**
     * Is used to get the total quantity of shares for all orders
     * @return number of shares
     */
    uint64_t totalQuantity();

    /**
     * Is used to get the symbol associated with this object
     * @return symbol associated with this object
     */
    const string & symbol();

    /**
     * Is used to add a valid order to this object. BBO will be
     * recalculated in this case
     * @param order_id order unique identificator
     * @param side "Buy" or "Sell"
     * @param quantity number of shares
     * @param price for one share
     */
    void add(uint64_t order_id, string side, uint64_t quantity, double price);

    /**
     * Is used to modify the existing order in this object.
     * BBO will be recalculated in this case
     * @param order_id order unique identificator
     * @param quantity number of shares
     * @param price for one share
     */
    void modify(uint64_t order_id, uint64_t quantity, double price);

    /**
     * Is used to cancel the existing order in this object.
     * BBO will be recalculated in this case
     * @param order_id order unique identificator
     */
    void cancel(uint64_t order_id);

    /**
     * Is used to get the current Best Bid Offer (BBO)
     * @return object with current BBO
     */
    OrderBbo bbo();

    /**
     * Is used to get the current Volume Weighted Average Price (VWAP)
     * @param quantity number of shares to use for calculation
     * @return object with current VWAP
     */
    OrderVwap vwap(uint64_t quantity);

    /**
     * Is used to get the iterator to the Order list
     * @return iterator
     */
    OrderIterator getIterator();

protected:
    /** Holds the buy offers sorted by the price top to down */
    OrderSetGreaterPtr orders_buy_;

    /** Holds the sell offers sorted by the price down to top */
    OrderSetLessPtr orders_sell_;

    /** Holds the total amount of shares in this object */
    uint64_t total_quantity_;

private:
    /** Holds the symbol associated with this object */
    const string symbol_;

    /** Deleted default constructor */
    SymbolOrderList() = delete;

    PREVENT_COPY(SymbolOrderList);
    PREVENT_MOVE(SymbolOrderList);
};

#endif //_SYMBOLORDERLIST_H
