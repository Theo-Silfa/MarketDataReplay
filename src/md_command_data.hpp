//
//  md_command_data.hpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 29.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#ifndef md_command_data_hpp
#define md_command_data_hpp

//System includes
#include <string>
#include <vector>

//Local includes

namespace md
{
namespace tokenizers
{

using namespace std;

/**
 * Md command data base clase. Is used as a base interface class for the family
 * of the different data holders.
 */
class MdCommandData
{
public:
    /** Defines the index for this object */
    enum BaseIndex
    {
        COMMAND_NAME = 0
    };

    /** Default constructor */
    MdCommandData();

    /** Copy constructor */
    MdCommandData(MdCommandData &);

    /** Copy assignment operator */
    MdCommandData & operator=(const MdCommandData &);

    /** Default destructor */
    virtual ~MdCommandData() = default;

    /**
     * Defines the operation to process all of the tokens. Must be implemented in
     * the child classes.
     * @param tokens vector of the string command tokens
     */
    virtual void processTokens(const vector<string> &tokens) = 0;

    /**
     * Is used to check whether or not the processTokens operation went successful
     * and the data can be safely retrieved from the obj
     * @return true - success, false - failure
     */
    virtual bool isProcessed() final;

    /**
     * Is used to get the failure error message from the object.
     * @return Error message
     */
    virtual const string & errorMessage() final;

protected:

    /**
     * Is used to set the proccessed state
     * @param state of the processing of tokens
     */
    virtual void setProcessed(bool state) final;

    /**
     * Is used to set the error message
     * @param message of the processing failure
     */
    virtual void setErrorMessage(const string &message) final;

private:

    /** State of the processing of tokens. True is ok&done, false if failed */
    bool process_state_;

    /** Will hold the error string of  */
    string error_message_;
};

} // namespace tokenizers
} // namespace md

#endif /* md_command_data_hpp */