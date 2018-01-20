//
//  container_definitions.hpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 20.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#ifndef _CONTAINERDEFINITIONS_H
#define _CONTAINERDEFINITIONS_H

using namespace std;

//Forward declarations
struct OrderRequest;

//Definitions

/** Multiset (greater) pointer definition for the convinience */
using OrderSetGreaterPtr = shared_ptr<multiset<OrderRequest, greater<OrderRequest>>>;

/** Multiset (less) pointer definition for the convinience */
using OrderSetLessPtr = shared_ptr<multiset<OrderRequest, less<OrderRequest>>>;

#endif //_CONTAINERDEFINITIONS_H