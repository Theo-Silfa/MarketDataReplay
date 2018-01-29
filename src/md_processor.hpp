//
//  md_processor.hpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 16.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#ifndef md_processor_hpp
#define md_processor_hpp

//System includes
#include <unordered_map>
#include <string>
#include <vector>

//Local includes
#include "defines.h"

namespace md
{
namespace processors
{

using namespace std;

/**
 * Market data processor class. Is used to process the tokens in to the
 * commands and execute them
 */
class MdProcessor
{
public:

    /** Default constructor */
    MdProcessor();

    /** Default destructor */
    ~MdProcessor() = default;

    /**
     * Is used to set the symbol for print filtering
     * @param val symbol to show in output
     */
    void setFilter(const string &val);

    /**
     * Is used to get the symbol for print filtering
     * @return symbol to show in output
     */
    const string & getFilter() const;

    /**
     * Main routine for processing the tokens in to commands
     * @param tokens what to be processed
     */
    void process(const vector<string> &tokens);

protected:
    /** Token handler definition */
    using MdHandler = function<void(const vector<string> &, const string &)>;

    /** Handler map definition */
    using MdHandlerMap = unordered_map<string, MdHandler>;

    /** Holds the handlers */
    const MdHandlerMap handlers_;

    /** Holds the symbol to show in the output */
    string symbol_;

private:
    PREVENT_COPY(MdProcessor);
    PREVENT_MOVE(MdProcessor);
};

} // namespace processors
} // namespace md

#endif /* md_processor_hpp */
