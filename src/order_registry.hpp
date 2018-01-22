//
//  order_registry.hpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 17.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#ifndef order_registry_hpp
#define order_registry_hpp

//System includes
#include <unordered_map>
#include <map>
#include <string>

//Local includes
#include "defines.h"
#include "symbol_order_list.hpp"

using namespace std;

/** Convinience definitions */
using OrdersActiveMap = unordered_map<uint64_t, string>;
using SymbolToOrdersMap = unordered_map<string, unique_ptr<SymbolOrderList>>;
using BboSubscribersMap = unordered_map<string, uint32_t>;
using VwapSubscribersMap = unordered_map<string, map<uint64_t, uint32_t>>;

/**
 * Order registry class. Implemented as singleton. Is used to hold
 * the order related data in one place. This is not ment to be used
 * in multiple threads at the same time
 */
class OrderRegistry final
{
public:
    /** Default destructor */
    ~OrderRegistry() = default;

    /**
     * Is used to get the single objects of this class
     * @return the single object of this class
     */
    static OrderRegistry& get()
    {
        static OrderRegistry instance;
        return instance;
    }

    /**
     * Is used to get the map of current active orders
     * @return current map of the active orders
     */
    OrdersActiveMap & getOrdersActive();

    /**
     * Is used to get the map of current active symbol to order connections
     * @return current map of the symbol to order connections
     */
    SymbolToOrdersMap & getSymbolToOrdersBind();

    /**
    * Is used to get the map of current bbo subscribers
    * @return current map of the bbo subscribers
    */
    BboSubscribersMap & getBboSubscribers();

    /**
    * Is used to get the map of current vwap subscribers
    * @return current map of the vwap subscribers
    */
    VwapSubscribersMap & getVwapSubscribers();

private:
    /** Default constructor */
    OrderRegistry() = default;

    /** Current active orders. Key is an order id, value is a symbol */
    OrdersActiveMap orders_active_;

    /** Current bindings of the symbols to the existing orders */
    SymbolToOrdersMap symbol_to_orders_bind_;

    /** Current bbo subscribers map. Key is a symbol, value is a subscriber counter */
    BboSubscribersMap bbo_subscribers_;

    /**
    * Current vwap subscribers map. Key is a symbol, value is a
    * map with quantity as a key and subscriber counter as a value
    */
    VwapSubscribersMap vwap_subscribers_;

    PREVENT_COPY(OrderRegistry);
    PREVENT_MOVE(OrderRegistry);
};

#endif /* order_registry_hpp */
