//
//  order_modify_data.hpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 30.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#ifndef order_modify_data_hpp
#define order_modify_data_hpp

//System includes

//Local includes
#include "md_command_data.hpp"

namespace md
{
namespace tokenizers
{

using namespace std;

/**
 * Order Modify command data class. Is used to process tokens from OM string and hold
 * the data.
 */
class OrderModifyData : public MdCommandData
{
public:
    /** Convinience type definition */
    using Parent = MdCommandData;

    /** Index for the Order Modify command */
    enum OrderModifyIndex
    {
        ORDER_ID = 1,
        QUANTITY,
        PRICE,
        SIZE
    };

    /** Default constructor */
    OrderModifyData();

    /** Copy constructor */
    OrderModifyData(OrderModifyData &);

    /** Copy assignment operator */
    OrderModifyData & operator=(const OrderModifyData &);

    /** Default destructor */
    virtual ~OrderModifyData() = default;

    /**
     * Implements the operation to process all of the tokens for Order Modify command
     * @param tokens vector of the tokens for OM command
     */
    virtual void processTokens(const vector<string> &tokens) override;

    /** Returns current order id */
    uint64_t getOrderId();

    /** Returns current stock quantity */
    uint64_t getQuantity();

    /** Returns current stock price */
    double getPrice();

protected:

    /** Holds order id. No constraints */
    uint64_t order_id_;

    /** Holds the quantity of the deal */
    uint64_t quantity_;

    /** Holds the price for one stock */
    double price_;

private:
};

} // namespace tokenizers
} // namespace md

#endif /* order_modify_data_hpp */