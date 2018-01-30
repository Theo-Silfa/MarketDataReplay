//
//  order_cancel_data.hpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 30.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#ifndef order_cancel_data_hpp
#define order_cancel_data_hpp

//System includes

//Local includes
#include "md_command_data.hpp"

namespace md
{
namespace tokenizers
{

using namespace std;

/**
 * Order Cancel command data class. Is used to process tokens from OC string and hold
 * the data.
 */
class OrderCancelData : public MdCommandData
{
public:
    /** Convinience type definition */
    using Parent = MdCommandData;

    /** Index for the Order Cancel command */
    enum OrderCancelIndex
    {
        ORDER_ID = 1,
        SIZE
    };

    /** Default constructor */
    OrderCancelData();

    /** Copy constructor */
    OrderCancelData(OrderCancelData &);

    /** Copy assignment operator */
    OrderCancelData & operator=(const OrderCancelData &);

    /** Default destructor */
    virtual ~OrderCancelData() = default;

    /**
     * Implements the operation to process all of the tokens for Order Cancel command
     * @param tokens vector of the tokens for OC command
     */
    virtual void processTokens(const vector<string> &tokens) override;

    /** Returns current order id */
    uint64_t getOrderId();

protected:
    /** Holds order id. No constraints */
    uint64_t order_id_;

private:
};

} // namespace tokenizers
} // namespace md

#endif /* order_cancel_data_hpp */