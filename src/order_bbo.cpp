//
//  order_bbo.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 17.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#include "order_bbo.hpp"

//System includes

//Local includes
#include "order_request.hpp"

/*************************** Helper Functions *************************/

ostream & operator<<(ostream & out, const OrderBbo & obj)
{
    return out;
}

/*************************** OrderBbo *********************************/

OrderBbo::OrderBbo(OrderRangeIterators bid, OrderRangeIterators ask)
{
}

void OrderBbo::calculate()
{
    return;
}
