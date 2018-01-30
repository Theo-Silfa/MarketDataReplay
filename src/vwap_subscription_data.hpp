//
//  vwap_subscription_data.hpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 30.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#ifndef vwap_subscription_data_hpp
#define vwap_subscription_data_hpp

//System includes

//Local includes
#include "md_command_data.hpp"

namespace md
{
namespace tokenizers
{

using namespace std;

/**
 * Vwap Subscription command data class. Is used to process tokens from SV and UV string and hold
 * the data.
 */
class VwapSubscriptionData : public MdCommandData
{
public:
    /** Convinience type definition */
    using Parent = MdCommandData;

    /** Index for the Vwap Subscription command */
    enum VwapSubscriptionIndex
    {
        SYMBOL = 1,
        QUANTITY,
        SIZE
    };

    /**
     * Constructor
     * @param command to be used to check the first token
     */
    VwapSubscriptionData(const string &command);

    /** Copy constructor */
    VwapSubscriptionData(VwapSubscriptionData &);

    /** Copy assignment operator */
    VwapSubscriptionData & operator=(const VwapSubscriptionData &);

    /** Default destructor */
    virtual ~VwapSubscriptionData() = default;

    /**
     * Implements the operation to process all of the tokens for Vwap Subscription command
     * @param tokens vector of the tokens for SB and UB commands
     */
    virtual void processTokens(const vector<string> &tokens) override;

    /** Returns current symbol */
    const string & getSymbol();

    /** Returns current stock quantity */
    uint64_t getQuantity();

protected:
    /** Holds stock symbol. Can't be empty string */
    string symbol_;

    /** Holds the quantity of the deal */
    uint64_t quantity_;

    /** Holds the current command name */
    string command_;

private:
    /** Deleted default constructor */
    VwapSubscriptionData() = delete;
};

} // namespace tokenizers
} // namespace md

#endif /* vwap_subscription_data_hpp */