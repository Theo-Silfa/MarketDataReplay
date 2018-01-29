//
//  order_add_data.hpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 29.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#ifndef order_add_data_hpp
#define order_add_data_hpp

//System includes

//Local includes
#include "md_command_data.hpp"
#include "container_definitions.hpp"

namespace md
{
namespace tokenizers
{

using namespace std;

/**
 * Order Add command data class. Is used to process tokens from OA string and hold
 * the data.
 */
class OrderAddData : public MdCommandData
{
public:
    /** Convinience type definition */
    using Parent = MdCommandData;

    /** Index for the Order Add command */
    enum OrderAddIndex
    {
        ORDER_ID = 1,
        SYMBOL,
        SIDE,
        QUANTITY,
        PRICE,
        SIZE
    };

    /** Default constructor */
    OrderAddData();

    /** Copy constructor */
    OrderAddData(OrderAddData &);

    /** Copy assignment operator */
    OrderAddData & operator=(const OrderAddData &);

    /** Default destructor */
    virtual ~OrderAddData() = default;

    /**
     * Implements the operation to process all of the tokens for Order Add command
     * @param tokens vector of the tokens for OA command
     */
    virtual void processTokens(const vector<string> &tokens) override;

    /** Returns current order id */
    uint64_t getOrderId();

    /** Returns current symbol */
    const string & getSymbol();

    /** Returns current order side */
    OrderSide getSide();

    /** Returns current stock quantity */
    uint64_t getQuantity();

    /** Returns current stock price */
    double getPrice();

protected:

    /** Holds order id. No constraints */
    uint64_t order_id_;

    /** Holds stock symbol. Can't be empty string */
    string symbol_;

    /** Holds side of the operation. Either buy or sell. Converted from the string */
    OrderSide side_;

    /** Holds the quantity of the deal */
    uint64_t quantity_;

    /** Holds the price for one stock */
    double price_;

private:
};

} // namespace tokenizers
} // namespace md

#endif /* order_add_data_hpp */