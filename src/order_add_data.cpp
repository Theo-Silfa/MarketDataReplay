//
//  order_add_data.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 29.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#include "order_add_data.hpp"

//System includes
#include <iostream>

//Local includes

using namespace std;
using namespace md::tokenizers;


OrderAddData::OrderAddData() :
    order_id_(0),
    symbol_(""),
    side_(OrderSide::UNKNOWN),
    quantity_(0),
    price_(0.0)
{
}


OrderAddData::OrderAddData(OrderAddData & obj) :
    Parent(obj),
    order_id_(obj.order_id_),
    symbol_(obj.symbol_),
    side_(obj.side_),
    quantity_(obj.quantity_),
    price_(obj.price_)
{
}

OrderAddData & OrderAddData::operator=(const OrderAddData &obj)
{
    Parent::operator=(obj);
    order_id_ = obj.order_id_;
    symbol_ = obj.symbol_;
    side_ = obj.side_;
    quantity_ = obj.quantity_;
    price_ = obj.price_;
    return *this;
}

void OrderAddData::processTokens(const vector<string> &tokens)
{
    try
    {
        if (tokens.size() != OrderAddIndex::SIZE)
        {
            Parent::setProcessed(false);
            Parent::setErrorMessage("Bad number of tokens to process");
            return;
        }

        if (tokens.at(Parent::COMMAND_NAME) != "ORDER ADD")
        {
            Parent::setProcessed(false);
            Parent::setErrorMessage("Bad order type");
            return;
        }

        order_id_ = stoull(tokens.at(OrderAddIndex::ORDER_ID));

        symbol_ = tokens.at(OrderAddIndex::SYMBOL);

        const auto & raw_side = tokens.at(OrderAddIndex::SIDE);

        if (raw_side == "Buy")
        {
            side_ = OrderSide::BUY;
        }
        else if (raw_side == "Sell")
        {
            side_ = OrderSide::SELL;
        }
        else
        {
            Parent::setProcessed(false);
            Parent::setErrorMessage("Bad side");
            return;
        }

        quantity_ = stoull(tokens.at(OrderAddIndex::QUANTITY));
        price_ = stod(tokens.at(OrderAddIndex::PRICE));

        Parent::setProcessed(true);
        Parent::setErrorMessage("Success");
        return;
    }
    catch (out_of_range &e)
    {
        cerr << "OrderAddData::processTokens(): Out of range exception: ["
            << string(e.what()) << "]" << '\n';
    }
    catch (invalid_argument &e)
    {
        cerr << "OrderAddData::processTokens(): Numeric conversion failure: ["
            << string(e.what()) << "]" << '\n';
    }

    Parent::setProcessed(false);
    Parent::setErrorMessage("Critical failure");
    return;
}

uint64_t OrderAddData::getOrderId()
{
    return order_id_;
}

const string & OrderAddData::getSymbol()
{
    return symbol_;
}

OrderSide OrderAddData::getSide()
{
    return side_;
}

uint64_t OrderAddData::getQuantity()
{
    return quantity_;
}

double OrderAddData::getPrice()
{
    return price_;
}
