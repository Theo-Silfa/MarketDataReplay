//
//  print_data.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 30.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#include "print_data.hpp"

//System includes
#include <iostream>

//Local includes

using namespace std;
using namespace md::tokenizers;


PrintData::PrintData(const string &command) :
    symbol_(""),
    command_(command)
{
}


PrintData::PrintData(PrintData &obj) :
    Parent(obj),
    symbol_(obj.symbol_),
    command_(obj.command_)
{
}

PrintData & PrintData::operator=(const PrintData &obj)
{
    symbol_ = obj.symbol_;
    command_ = obj.command_;
    return *this;
}

void PrintData::processTokens(const vector<string> &tokens)
{
    try
    {
        if (tokens.size() != PrintIndex::SIZE)
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

        symbol_ = tokens.at(PrintIndex::SYMBOL);

        Parent::setProcessed(true);
        Parent::setErrorMessage("Success");
        return;
    }
    catch (out_of_range &e)
    {
        cerr << "PrintData::processTokens(): Out of range exception: ["
            << string(e.what()) << "]" << '\n';
    }

    Parent::setProcessed(false);
    Parent::setErrorMessage("Critical failure");
    return;
}

const string & PrintData::getSymbol()
{
    return symbol_;
}
