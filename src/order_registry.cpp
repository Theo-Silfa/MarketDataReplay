//
//  order_registry.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 17.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#include "order_registry.hpp"

//System includes

//Local includes


/************************* OrderRegistry ******************************/

OrdersActiveMap & OrderRegistry::getOrdersActive()
{
    return orders_active_;
}

SymbolToOrdersMap & OrderRegistry::getSymbolToOrdersBind()
{
    return symbol_to_orders_bind_;
}

BboSubscribersMap & OrderRegistry::getBboSubscribers()
{
    return bbo_subscribers_;
}
