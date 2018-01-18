//
//  command_tokenizer.hpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 17.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#ifndef command_tokenizer_hpp
#define command_tokenizer_hpp

//System includes
#include <vector>
#include <string>

//Local includes

namespace md
{
namespace tokenizers
{

using namespace std;

/**
 *  Tokenizer class. Is used to process argument of the program.
 */
class CommandTokenizer
{
public:

    /** Index for the Order Add command */
    enum OrderAddIndex
    {
        COMMAND_NAME = 0,
        OA_ORDER_ID,
        OA_SYMBOL,
        OA_SIDE,
        OA_QUANTITY,
        OA_PRICE,
    };

    /** Index for the Order Modify command */
    enum OrderModifyIndex
    {
        OM_ORDER_ID = 1,
        OM_QUANTITY,
        OM_PRICE,
    };

    /** Index for the Order Cancel command */
    enum OrderCancelIndex
    {
        OC_ORDER_ID = 1,
    };

    /** Index for the BBO related commands */
    enum BboIndex
    {
        BBO_SYMBOL = 1,
    };

    /** Index for the VWAP related commands */
    enum VwapIndex
    {
        VWAP_SYMBOL = 1,
        VWAP_QUANTITY,
    };

    /** Index for the Print related commands */
    enum PrintIndex
    {
        PRINT_SYMBOL = 1,
    };

    /** Default constructor */
    CommandTokenizer() = default;

    /** Default destructor */
    ~CommandTokenizer() = default;

    /** Copy constructor */
    CommandTokenizer(const CommandTokenizer &);

    /** Move constructor */
    CommandTokenizer(CommandTokenizer &&);

    /** Copy assignment operator */
    CommandTokenizer & operator=(const CommandTokenizer &);

    /** Move assignment operator */
    CommandTokenizer & operator=(CommandTokenizer &&);

    /**
     * This method is used to break down the string in to tokens
     * @param raw a string to be tokenized
     */
    void tokenize(const string &raw);

    /**
     * Is used for the access to the tokens
     * @param index enums defined in this class should be used
     */
    const string & operator[](uint8_t index) const;

protected:

    /** Container which holds tokens */
    vector<string> tokens_;
};

} // namespace tokenizers
} // namespace md

#endif /* command_tokenizer_hpp */
