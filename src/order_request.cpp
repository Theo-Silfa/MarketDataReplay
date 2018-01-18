//
//  order_request.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 17.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#include "order_request.hpp"

//System includes

//Local includes


bool operator <(const OrderRequest& a, const OrderRequest& b)
{
    return false;
}

bool operator >(const OrderRequest& a, const OrderRequest& b)
{
    return false;
}

ostream& operator<<(ostream& out, const OrderVwap& h)
{
    return out;
}
