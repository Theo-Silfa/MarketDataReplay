//
//  container_definitions.hpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 20.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#ifndef _CONTAINERDEFINITIONS_H
#define _CONTAINERDEFINITIONS_H

//System includes
#include <set>
#include <unordered_map>
#include <memory>
#include <functional>

//Local includes
#include "order_request.hpp"

using namespace std;

//Forward declarations

//Definitions

/**
 * Enumeration to distinguish between Buy and Sell orders.
 * Is used to make the connection between OrderIdMap and OrderSet*Ptr's
 */
enum OrderSide
{
    BUY,
    SELL
};

/** Multiset (greater) pointer definition for the convinience */
using OrderSetGreaterPtr = shared_ptr<multiset<OrderRequest, greater<OrderRequest>>>;

/** Multiset (less) pointer definition for the convinience */
using OrderSetLessPtr = shared_ptr<multiset<OrderRequest, less<OrderRequest>>>;

/** Set definition to take track on the existing order ids */
using OrderIdMap = unordered_map<uint64_t, pair<OrderSide, multiset<OrderRequest>::iterator>>;

#endif //_CONTAINERDEFINITIONS_H