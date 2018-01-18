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
#include <string>

//Local includes
#include "defines.h"
#include "symbol_order_list.hpp"

using namespace std;

/** Convinience definitions */
using OrdersActiveMap = unordered_map<uint64_t, string>;

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

private:
    /** Default constructor */
    OrderRegistry() = default;

    /** Current active orders */
    OrdersActiveMap orders_active_;

    PREVENT_COPY(OrderRegistry);
    PREVENT_MOVE(OrderRegistry);
};

#endif /* order_registry_hpp */
