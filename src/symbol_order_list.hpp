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
 * Order process exception definition.
 * Is thrown whenever where is a problem with add, modify or cancel of order.
 */
class OrderProcessException: public exception
{
public:
    /**
     * Constructor (C strings).
     * @param message C-style string error message.
     *                The string contents are copied upon construction.
     *                Hence, responsibility for deleting the char* lies
     *                with the caller.
     */
    explicit OrderProcessException(const char* message):
        msg_(message)
        {}

    /**
     * Constructor (C++ STL strings).
     * @param message The error message.
     */
    explicit OrderProcessException(const string& message):
        msg_(message)
        {}

    /**
     * Destructor.
     * Virtual to allow for subclassing.
     */
    virtual ~OrderProcessException() = default;

    /**
     * Returns a pointer to the (constant) error description.
     * @return A pointer to a const char*. The underlying memory
     *         is in posession of the Exception object. Callers must
     *         not attempt to free the memory.
     */
    virtual const char* what() const noexcept
    {
       return msg_.c_str();
    }

protected:
    /** Error message */
    string msg_;
};

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

    /** Holds the existing orders in this object */
    OrderIdMap existing_orders_;

private:
    /** Holds the symbol associated with this object */
    const string symbol_;

    /** Deleted default constructor */
    SymbolOrderList() = delete;

    PREVENT_COPY(SymbolOrderList);
    PREVENT_MOVE(SymbolOrderList);
};

#endif //_SYMBOLORDERLIST_H
