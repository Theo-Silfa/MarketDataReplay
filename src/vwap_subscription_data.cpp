//
//  vwap_subscription_data.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 30.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#include "vwap_subscription_data.hpp"

//System includes
#include <iostream>

//Local includes

using namespace std;
using namespace md::tokenizers;


VwapSubscriptionData::VwapSubscriptionData(const string &command) :
    symbol_(""),
    quantity_(0),
    command_(command)
{
}


VwapSubscriptionData::VwapSubscriptionData(VwapSubscriptionData &obj) :
    symbol_(obj.symbol_),
    quantity_(obj.quantity_),
    command_(obj.command_)
{
}

VwapSubscriptionData & VwapSubscriptionData::operator=(const VwapSubscriptionData &obj)
{
    symbol_ = obj.symbol_;
    quantity_ = obj.quantity_;
    command_ = obj.command_;
    return *this;
}

void VwapSubscriptionData::processTokens(const vector<string> &tokens)
{
    try
    {
        if (tokens.size() != VwapSubscriptionIndex::SIZE)
        {
            Parent::setProcessed(false);
            Parent::setErrorMessage("Bad number of tokens to process");
            return;
        }

        if (tokens.at(Parent::COMMAND_NAME) != command_)
        {
            Parent::setProcessed(false);
            Parent::setErrorMessage("Bad order type");
            return;
        }

        symbol_ = tokens.at(VwapSubscriptionIndex::SYMBOL);

        quantity_ = stoull(tokens.at(VwapSubscriptionIndex::QUANTITY));

        Parent::setProcessed(true);
        Parent::setErrorMessage("Success");
        return;
    }
    catch (out_of_range &e)
    {
        cerr << "VwapSubscriptionData::processTokens(): Out of range exception: ["
            << string(e.what()) << "]" << '\n';
    }
    catch (invalid_argument &e)
    {
        cerr << "VwapSubscriptionData::processTokens(): Numeric conversion failure: ["
            << string(e.what()) << "]" << '\n';
    }

    Parent::setProcessed(false);
    Parent::setErrorMessage("Critical failure");
    return;
}

const string & VwapSubscriptionData::getSymbol()
{
    return symbol_;
}

uint64_t VwapSubscriptionData::getQuantity()
{
    return quantity_;
}