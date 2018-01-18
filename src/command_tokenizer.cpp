//
//  command_tokenizer.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 17.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#include "command_tokenizer.hpp"

//System includes
#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>
#include <string>

//Local includes
#include "defines.h"

using namespace md::tokenizers;

/*************************** Helper Functions *************************/

namespace
{
/** This string indicates the failure in the tokenizer */
static const string NIL = "NIL";

/**
 * Implements the split by delimiters alghorithm
 * @param s target string
 * @param delim delimiter to be used
 * @param result where to store the tokens
 */
template<typename Out>
void split(const string &s, char delim, Out result)
{
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim))
    {
        *(result++) = item;
    }
}

/**
 * Wraper around the split function. Vector specialized
 * @param s target string
 * @param delim delimiter to be used
 * @return vector of tokens
 */
vector<string> split(const string &s, char delim)
{
    vector<string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}

} // namespace

/*************************** CommandTokenizer *************************/

CommandTokenizer::CommandTokenizer(const CommandTokenizer &obj)
{
    this->tokens_ = obj.tokens_;
}

CommandTokenizer::CommandTokenizer(md::tokenizers::CommandTokenizer &&obj)
{
    this->tokens_ = move(obj.tokens_);
}

CommandTokenizer & CommandTokenizer::operator=(const CommandTokenizer &obj)
{
    this->tokens_ = obj.tokens_;
    return *this;
}

CommandTokenizer & CommandTokenizer::operator=(CommandTokenizer &&obj)
{
    this->tokens_ = move(obj.tokens_);
    return obj;
}

void CommandTokenizer::tokenize(const string &raw)
{
    tokens_ = split(raw, ',');
}

const string & CommandTokenizer::operator[](uint8_t index) const
{
    try
    {
        return tokens_.at(index);
    }
    catch(out_of_range &e)
    {
        cerr << "CommandTokenizer::operator[]: bad index: [" << to_string(index) <<
        "]. Exception: [" + string(e.what()) + "]" << '\n';

        //Return NIL string to indicate that something went wrong
        return NIL;
    }
}
