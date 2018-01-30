//
//  bbo_subscription_data.hpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 30.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#ifndef bbo_subscription_data_hpp
#define bbo_subscription_data_hpp

//System includes

//Local includes
#include "md_command_data.hpp"

namespace md
{
namespace tokenizers
{

using namespace std;

/**
 * Bbo Subscription command data class. Is used to process tokens from SB and UB string and hold
 * the data.
 */
class BboSubscriptionData : public MdCommandData
{
public:
    /** Convinience type definition */
    using Parent = MdCommandData;

    /** Index for the Bbo Subscription command */
    enum BboSubscriptionIndex
    {
        SYMBOL = 1,
        SIZE
    };

    /**
     * Constructor
     * @param command to be used to check the first token
     */
    BboSubscriptionData(const string &command);

    /** Copy constructor */
    BboSubscriptionData(BboSubscriptionData &);

    /** Copy assignment operator */
    BboSubscriptionData & operator=(const BboSubscriptionData &);

    /** Default destructor */
    virtual ~BboSubscriptionData() = default;

    /**
     * Implements the operation to process all of the tokens for Bbo Subscription command
     * @param tokens vector of the tokens for SB and UB commands
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
    BboSubscriptionData() = delete;
};

} // namespace tokenizers
} // namespace md

#endif /* bbo_subscription_data_hpp */