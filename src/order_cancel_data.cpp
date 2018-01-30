//
//  order_cancel_data.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 30.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#include "order_cancel_data.hpp"

//System includes
#include <iostream>

//Local includes

using namespace std;
using namespace md::tokenizers;


OrderCancelData::OrderCancelData() :
    order_id_(0)
{
}


OrderCancelData::OrderCancelData(OrderCancelData & obj) :
    Parent(obj),
    order_id_(obj.order_id_)
{
}

OrderCancelData & OrderCancelData::operator=(const OrderCancelData &obj)
{
    Parent::operator=(obj);
    order_id_ = obj.order_id_;
    return *this;
}

void OrderCancelData::processTokens(const vector<string> &tokens)
{
    try
    {
        if (tokens.size() != OrderCancelIndex::SIZE)
        {
            Parent::setProcessed(false);
            Parent::setErrorMessage("Bad number of tokens to process");
            return;
        }

        if (tokens.at(Parent::COMMAND_NAME) != "ORDER CANCEL")
        {
            Parent::setProcessed(false);
            Parent::setErrorMessage("Bad order type");
            return;
        }

        order_id_ = stoull(tokens.at(OrderCancelIndex::ORDER_ID));

        Parent::setProcessed(true);
        Parent::setErrorMessage("Success");
        return;
    }
    catch (out_of_range &e)
    {
        cerr << "OrderCancelData::processTokens(): Out of range exception: ["
            << string(e.what()) << "]" << '\n';
    }
    catch (invalid_argument &e)
    {
        cerr << "OrderCancelData::processTokens(): Numeric conversion failure: ["
            << string(e.what()) << "]" << '\n';
    }

    Parent::setProcessed(false);
    Parent::setErrorMessage("Critical failure");
    return;
}

uint64_t OrderCancelData::getOrderId()
{
    return order_id_;
}
