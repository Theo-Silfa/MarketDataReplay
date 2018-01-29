//
//  md_command_data.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 29.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#include "md_command_data.hpp"

//System includes

//Local includes

using namespace std;
using namespace md::tokenizers;


MdCommandData::MdCommandData() :
    process_state_(false),
    error_message_("Object is empty")
{
}


MdCommandData::MdCommandData(MdCommandData & obj) :
    process_state_(obj.process_state_),
    error_message_(obj.error_message_)
{
}

MdCommandData & MdCommandData::operator=(const MdCommandData &obj)
{
    process_state_ = obj.process_state_;
    error_message_ = obj.error_message_;
    return *this;
}

bool MdCommandData::isProcessed()
{
    return process_state_;
}

const string & MdCommandData::errorMessage()
{
    return error_message_;
}

void MdCommandData::setProcessed(bool state)
{
    process_state_ = state;
}

void MdCommandData::setErrorMessage(const string &message)
{
    error_message_ = message;
}