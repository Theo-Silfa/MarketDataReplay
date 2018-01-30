//
//  print_data.hpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 30.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#ifndef print_data_hpp
#define print_data_hpp

//System includes

//Local includes
#include "md_command_data.hpp"

namespace md
{
namespace tokenizers
{

using namespace std;

/**
 * Print command data class. Is used to process tokens from P and PF string and hold
 * the data.
 */
class PrintData : public MdCommandData
{
public:
    /** Convinience type definition */
    using Parent = MdCommandData;

    /** Index for the Print command */
    enum PrintIndex
    {
        SYMBOL = 1,
        SIZE
    };

    /**
     * Constructor
     * @param command to be used to check the first token
     */
    PrintData(const string &command);

    /** Copy constructor */
    PrintData(PrintData &);

    /** Copy assignment operator */
    PrintData & operator=(const PrintData &);

    /** Default destructor */
    virtual ~PrintData() = default;

    /**
     * Implements the operation to process all of the tokens for Print command
     * @param tokens vector of the tokens for P and PF commands
     */
    virtual void processTokens(const vector<string> &tokens) override;

    /** Returns current symbol */
    const string & getSymbol();

protected:
    /** Holds stock symbol. Can't be empty string */
    string symbol_;

    /** Holds the current command name */
    string command_;

private:
    /** Deleted default constructor */
    PrintData() = delete;
};

} // namespace tokenizers
} // namespace md

#endif /* print_data_hpp */