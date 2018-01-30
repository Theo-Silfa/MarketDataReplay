//
//  order_modify_data.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 30.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#include "order_modify_data.hpp"

//System includes
#include <iostream>

//Local includes

using namespace std;
using namespace md::tokenizers;


OrderModifyData::OrderModifyData() :
    order_id_(0),
    quantity_(0),
    price_(0.0)
{
}


OrderModifyData::OrderModifyData(OrderModifyData & obj) :
    Parent(obj),
    order_id_(obj.order_id_),
    quantity_(obj.quantity_),
    price_(obj.price_)
{
}

OrderModifyData & OrderModifyData::operator=(const OrderModifyData &obj)
{
    order_id_ = obj.order_id_;
    quantity_ = obj.quantity_;
    price_ = obj.price_;
    return *this;
}

void OrderModifyData::processTokens(const vector<string> &tokens)
{
    try
    {
        if (tokens.size() != OrderModifyIndex::SIZE)
        {
            Parent::setProcessed(false);
            Parent::setErrorMessage("Bad number of tokens to process");
            return;
        }

        if (tokens.at(Parent::COMMAND_NAME) != "ORDER MODIFY")
        {
            Parent::setProcessed(false);
            Parent::setErrorMessage("Bad order type");
            return;
        }

        order_id_ = stoull(tokens.at(OrderModifyIndex::ORDER_ID));
        quantity_ = stoull(tokens.at(OrderModifyIndex::QUANTITY));
        price_ = stod(tokens.at(OrderModifyIndex::PRICE));

        Parent::setProcessed(true);
        Parent::setErrorMessage("Success");
        return;
    }
    catch (out_of_range &e)
    {
        cerr << "OrderModifyData::processTokens(): Out of range exception: ["
            << string(e.what()) << "]" << '\n';
    }
    catch (invalid_argument &e)
    {
        cerr << "OrderModifyData::processTokens(): Numeric conversion failure: ["
            << string(e.what()) << "]" << '\n';
    }

    Parent::setProcessed(false);
    Parent::setErrorMessage("Critical failure");
    return;
}

uint64_t OrderModifyData::getOrderId()
{
    return order_id_;
}

uint64_t OrderModifyData::getQuantity()
{
    return quantity_;
}

double OrderModifyData::getPrice()
{
    return price_;
}
