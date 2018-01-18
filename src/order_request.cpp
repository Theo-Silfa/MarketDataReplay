//
//  order_request.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 17.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#include "order_request.hpp"

//System includes
#include <iomanip>

//Local includes


bool operator<(const OrderRequest &lhs, const OrderRequest &rhs)
{
    return (lhs.price < rhs.price ? true : false);
}

bool operator>(const OrderRequest &lhs, const OrderRequest &rhs)
{
    return (lhs.price > rhs.price ? true : false);
}

bool operator==(const OrderRequest &lhs, const OrderRequest &rhs)
{
    return (lhs.price == rhs.price ? true : false);
}

ostream & operator<<(ostream &out, const OrderVwap &obj)
{
    return out << '<' << fixed << setprecision(2) << obj.buy_price << ','
        << fixed << setprecision(2) << obj.sell_price << '>';
}
