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
    return lhs.price < rhs.price;
}

bool operator>(const OrderRequest &lhs, const OrderRequest &rhs)
{
    return lhs.price > rhs.price;
}

bool operator==(const OrderRequest &lhs, const OrderRequest &rhs)
{
    return lhs.price == rhs.price;
}

ostream & operator<<(ostream &out, const OrderVwap &obj)
{
    if (obj.buy_price != 0.0)
    {
        out << '<' << fixed << setprecision(2) << obj.buy_price;
    }
    else
    {
        out << '<' << fixed << setprecision(2) << "NIL";
    }

    out << ',';

    if (obj.sell_price != 0.0)
    {
        out << fixed << setprecision(2) << obj.sell_price << '>';
    }
    else
    {
        out << fixed << setprecision(2) << "NIL" << '>';
    }

    return out;
}
