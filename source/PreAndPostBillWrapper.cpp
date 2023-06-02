#include "PreAndPostBillWrapper.h"
#include "ABL_Date.h"
#include "ABL_DateTime.h"
#include "Utils.h"
#include <stdlib.h> 

/*********************************************************************
#   File        :   PreAndPostBillWrapper.cpp
#   Abstract    :   Implementation of pre and post bill Wrapper.
#
#   Date        :   31st May, 2013
#   Author(s)   :   Srinivasan V
#   Code Size   :   lines
#   Design      :
#   References  :
#   Module      :  
======================================================================
#   Revision History
======================================================================
#   SI       Edited by          Date            Area Edited
#    1       Srinivasan V       6/8/2013        Fixed the insertion error-175068    
#    2       Srinivasan V       9/26/2013       Fixed the intrim bill-182063        
#    3       Srinivasan V       11/14/2013      Fixed the pre bill check error-182063    
#    4       Srinivasan V       12/16/2013      Fixed the pre bill check error-191412    
#    5       Ralph              20/03/2014      Fixed the pre bill check error-207454
======================================================================
 */

//////////////////////////////////////////////////////////////////////
//
//      Default Destructor
//
//////////////////////////////////////////////////////////////////////
PreAndPostBillWrapper::PreAndPostBillWrapper()
{
    m_ABL_ServicePtr      = NULL;
    m_billDbConnectionPtr = NULL;
    m_orchMessengerPtr    = NULL;
    m_msgToOrchestrator   = NULL;
    m_msgFromOrchestrator = NULL;

    m_receiveMessageFormat = "05"; //Receive the confirm or rollback request from task allocator at confirm Q
    m_batchCountMessageFormat = "08"; //Send the status of the batch to the status updater
    m_statsUpdatorMessageFormat = "03"; //Send the metrics of the batch to stats updator
    m_taskAlMessageFormat = "02"; //Send the Task al
    m_pauseActionMessageFormat = "11"; //Send the PAUSE ACK 
}
//////////////////////////////////////////////////////////////////////
//
//      constructor
//
//////////////////////////////////////////////////////////////////////
//
//  Parameters  :   p_runMode    -> Running mode.
//              :   p_confFile   -> Configuration file path    
//
//////////////////////////////////////////////////////////////////////
PreAndPostBillWrapper::PreAndPostBillWrapper(ABL_String p_currentBillRunType,ABL_String p_currentBillRunMode, ABL_String p_confFile)
{
    m_ABL_ServicePtr      = NULL;
    m_billDbConnectionPtr = NULL;
    m_orchMessengerPtr    = NULL;
    m_msgToOrchestrator   = NULL;
    m_msgFromOrchestrator = NULL;

    m_currentBillRunType = p_currentBillRunType;
    m_currentBillRunMode = p_currentBillRunMode;
    m_confFile = p_confFile;

    m_receiveMessageFormat = "05"; //Receive the batch preparation request from task allocator at batch prep Q
    m_batchCountMessageFormat = "08"; //Send the status of the batch to dispatcher Q
    m_statsUpdatorMessageFormat = "03"; //Send the metrics of the batch to stats updator
    m_taskAlMessageFormat = "02"; //Send the Task al
    m_pauseActionMessageFormat = "11"; //Send the PAUSE ACK 
}
//////////////////////////////////////////////////////////////////////
//
//      Copy constructor
//
//////////////////////////////////////////////////////////////////////
//
//  Parameters  :   p_confirmWrapper    -> object to be copied from
//
//////////////////////////////////////////////////////////////////////
PreAndPostBillWrapper::PreAndPostBillWrapper(const PreAndPostBillWrapper& p_preAndPostBillWrapper)
{
    m_confFile = p_preAndPostBillWrapper.m_confFile;
    m_ABL_ServicePtr = p_preAndPostBillWrapper.m_ABL_ServicePtr;
    m_currentBillRunType = p_preAndPostBillWrapper.m_currentBillRunType;
    m_currentBillRunMode = p_preAndPostBillWrapper.m_currentBillRunMode;

    m_billDbID = p_preAndPostBillWrapper.m_billDbID;
    m_billDbConnectionPtr = p_preAndPostBillWrapper.m_billDbConnectionPtr;
    m_msgToOrchestrator = p_preAndPostBillWrapper.m_msgToOrchestrator;
    m_msgFromOrchestrator = p_preAndPostBillWrapper.m_msgFromOrchestrator;
    m_orchMessengerPtr = p_preAndPostBillWrapper.m_orchMessengerPtr;
    m_execName = p_preAndPostBillWrapper.m_execName;

    m_receiveMessageFormat  = "05"; //Receive the batch preparation request from task allocator at batch prep Q
    m_batchCountMessageFormat = "08"; //Send the status of the batch to dispatcher Q
    m_statsUpdatorMessageFormat = "03"; //Send the metrics of the batch to stats updator
    m_taskAlMessageFormat = "02"; //Send the Task al
    m_pauseActionMessageFormat = "11"; //Send the PAUSE ACK 
}

//////////////////////////////////////////////////////////////////////
//
//      Default Destructor
//
//////////////////////////////////////////////////////////////////////
PreAndPostBillWrapper::~PreAndPostBillWrapper()
{
    if (m_msgToOrchestrator != NULL)
        delete m_msgToOrchestrator;

    if (m_msgFromOrchestrator != NULL)
        delete m_msgFromOrchestrator;

    if (m_orchMessengerPtr != NULL)
        delete m_orchMessengerPtr;

    if (m_billDbConnectionPtr != NULL)
        delete m_billDbConnectionPtr;

    if (m_ABL_ServicePtr != NULL)
        delete m_ABL_ServicePtr;
}
//////////////////////////////////////////////////////////////////////
//
//      Initializing the  confirm wrapper
//
//////////////////////////////////////////////////////////////////////
//
//  Parameters  :   p_confirmWrapper    -> object to be copied from
//
//////////////////////////////////////////////////////////////////////
bool PreAndPostBillWrapper::mb_init()
{
    try
    {
        m_ABL_ServicePtr = new ABL_Service();
        m_ABL_ServicePtr->mb_initServices(m_confFile);
    }
    catch(ABL_Exception &e)
    {
        std::cerr << "Error in Initializing the ABL_Service" << e.mb_getMessage().c_str()<<std::endl;
        return false;
    }
    //Accessing orchestration related tables and getting info
    try
    {
        m_orchMessengerPtr = new OrchMessenger();

        if(m_currentBillRunMode == "P")
        {
            m_execName = "PREBILL";
            if(!m_orchMessengerPtr->mb_initMessenger(m_ABL_ServicePtr, PREBILL, READ_WRITE, m_currentBillRunType))
            {
                *m_ABL_ServicePtr << CRITICAL << "Error in Initializing the OrchMessenger" << Endl;
                return false;
            }
        }
        else
        {
            m_execName = "POSTBILL";
            if(!m_orchMessengerPtr->mb_initMessenger(m_ABL_ServicePtr, POSTBILL, READ_WRITE, m_currentBillRunType))
            {
                *m_ABL_ServicePtr << CRITICAL << "Error in Initializing the OrchMessenger" << Endl;
                return false;
            }
        }

    }
    catch(ABL_Exception &e)
    {
        *m_ABL_ServicePtr << CRITICAL << "Exception in getting data from Orchestrator Tables:" << e.mb_getMessage() << Endl;
        return false;
    }

    //Create a connection
    m_billDbID = m_ABL_ServicePtr->mb_getParameterValue("BILL_DB_ID");
    m_billDbConnectionPtr = new ABL_Connection();
    m_billDbConnectionPtr->mb_createConnection(*m_ABL_ServicePtr, true, true, m_billDbID);

    //Utils initialize
    Utils::mb_initilize(m_billDbConnectionPtr);

    //Create Message repository to send msg
    m_msgToOrchestrator = new MessageRepository;

    //Create Message repository to read msg
    m_msgFromOrchestrator = new MessageRepository;

    return true;
}

//////////////////////////////////////////////////////////////////////
//
//      Process the confirm the requests.
//
//////////////////////////////////////////////////////////////////////
//
//  Parameters  :   p_confirmWrapper    -> object to be copied from
//
//////////////////////////////////////////////////////////////////////
bool PreAndPostBillWrapper::mb_process()
{
    int l_sleeptime;

    ABL_String l_stopSignal = "";
    
    try
    {
        l_stopSignal = m_ABL_ServicePtr->mb_getParameterValue("PRE_POST_CHECK_STOP_SIGNAL");
    }
    catch (ABL_Exception &e)
    {
        if(e.mb_getErrorCode() == SER_NODATA)
        {
            *m_ABL_ServicePtr << CRITICAL << "PRE_POST_CHECK_STOP_SIGNAL:Not found in the .conf"<<Endl;
            return false;
        }
    }

    try
    {
        l_sleeptime = m_ABL_ServicePtr->mb_getInt("SLEEP_TIME");
    }
    catch (ABL_Exception &e)
    {
        if(e.mb_getErrorCode() == SER_NODATA)
        {
            *m_ABL_ServicePtr << CRITICAL << "SLEEP_TIME:Not found in the .conf"<<Endl;
            return false;
        }
    }

    ABL_String l_insertQuery = "Insert into CB_PRE_POST_BILL_CHECK_REQUEST (BATCH_ID_V,SUB_BATCH_ID_N,BILL_TYPE_V,STAGE_SEQ_N,COMMIT_FLAG_V,TYPE_OF_CHECK_V,REQUEST_STATUS_V,ACCOUNT_POSTED_V,BILL_CYCLE_FULL_CODE_N,BILL_CYCLE_START_DATE_D,BILL_CYCLE_END_DATE_D, MK_REQ_NUM_N)";
    l_insertQuery += " values (:1,:2,:3,:4,:5,:6,:7,:8,:9,:10,:11,:12)";
    ABL_Statement l_insertStatement = m_billDbConnectionPtr->mb_createStatement(l_insertQuery);    

    ABL_String l_selectQuery = "SELECT BATCH_ID_V,SUB_BATCH_ID_N,NO_OF_ACCOUNTS_PROCESSED_N,NO_OF_SUCCESS_ACCOUNTS_N,NO_OF_FAILED_ACCOUNTS_N,BILL_TYPE_V,STAGE_SEQ_N,";
    l_selectQuery += " TO_CHAR(BILL_CYCLE_START_DATE_D,'YYYYMMDD'),TO_CHAR(BILL_CYCLE_END_DATE_D,'YYYYMMDD'),BILL_CYCLE_FULL_CODE_N, MK_REQ_NUM_N, ROWID FROM CB_PRE_POST_BILL_CHECK_REQUEST ";

    l_selectQuery += " WHERE REQUEST_STATUS_V = 'C' AND ACCOUNT_POSTED_V = 'N' AND TYPE_OF_CHECK_V = '";
    l_selectQuery += m_currentBillRunMode;
    l_selectQuery += "' AND ";

    //Based on bill type pick up the requests
    if (m_currentBillRunType == "S")
    {
        l_selectQuery += " BILL_TYPE_V = 'S'";
    }
    else
        l_selectQuery += " BILL_TYPE_V != 'S'";

    l_selectQuery += " FOR UPDATE ";

    ABL_Statement l_selectStatement = m_billDbConnectionPtr->mb_createStatement(l_selectQuery);

    ABL_String l_updateQuery = "UPDATE CB_PRE_POST_BILL_CHECK_REQUEST SET ACCOUNT_POSTED_V = 'Y' ";
    //l_updateQuery += " WHERE BATCH_ID_V = :1 AND SUB_BATCH_ID_N = :2  AND TYPE_OF_CHECK_V = :3 ";
    l_updateQuery += " WHERE ROWID = :1 ";
    ABL_Statement l_updateStatement = m_billDbConnectionPtr->mb_createStatement(l_updateQuery);

    ABL_ResultSet l_resutSet;

    ABL_String l_batchId;
    ABL_String l_subBatchId;
    long l_noOfAccounts;
    long l_noOfSuccessAcc;
    long l_noOfFailedAcc;
    ABL_String l_billType;
    ABL_String l_startDate;
    ABL_String l_endDate;
    ABL_String l_seqNo;
    ABL_String l_billCycleCode;
    ABL_String l_mock_req_num;
    ABL_String l_rowId;

    while(true)
    {
        m_msgToOrchestrator->mb_reset();
        m_msgFromOrchestrator->mb_reset();
        try
        {
            //Check for existance of stop signal file
            if(!access(l_stopSignal.c_str(), F_OK))
            {
                *m_ABL_ServicePtr << CRITICAL << "Found the stop signal. stoping the engine." << Endl;
                m_billDbConnectionPtr->mb_terminateStatement(l_insertStatement);
                m_billDbConnectionPtr->mb_terminateStatement(l_selectStatement);
                m_billDbConnectionPtr->mb_terminateStatement(l_updateStatement);
            
                return true;
            }
            //Read message from batch prep Q
            *m_ABL_ServicePtr << INFO << "Reading mesg from Q" << Endl;
            if( m_orchMessengerPtr->mb_readMessageFromQueue(false))
            {
                *m_ABL_ServicePtr << INFO << "got the mesg from Q" << Endl;

                // std::cerr<<"Got the message"<<std::endl;
                //Get the pointer the object containing the message
                m_orchMessengerPtr->mb_getMessageIntoRepositoryObject(m_msgFromOrchestrator);
                *m_ABL_ServicePtr << INFO << "Processing request Batch Id:" << m_msgFromOrchestrator->mb_getMainBatch().c_str() << " Bill Run type:" << m_msgFromOrchestrator->mb_getBillType().c_str() << Endl;

                //Validate if the msg is to be processed by this engine
                /**if(!mb_validRequest(m_currentBillRunType, m_msgFromOrchestrator->mb_getBillType()))
                {
                    *m_ABL_ServicePtr << CRITICAL << "Configured Mode and Bill Mode received in msg not matching... discarding the msg" << Endl;
                    continue;
                }
                */
                *m_ABL_ServicePtr << INFO << "Validation success" << Endl;
                //Set the start time for batch preparation
                m_msgToOrchestrator->mb_setStartTime();

                //Check for Pause signal
                if(m_orchMessengerPtr->mb_isBatchPaused())
                {
                    //std::cerr<<"Batch is paused"<<std::endl;
                    *m_ABL_ServicePtr << INFO << "Batch is paused" << Endl;
                    //Send pause ACK to Orchestrator
                    m_msgToOrchestrator->mb_setBatchPauseState(true);
                    m_msgToOrchestrator->mb_setMainBatch(m_msgFromOrchestrator->mb_getMainBatch());
                    m_msgToOrchestrator->mb_setFullBillCycleCode(m_msgFromOrchestrator->mb_getFullBillCycleCode());
                    m_msgToOrchestrator->mb_setSubBatch(m_msgFromOrchestrator->mb_getSubBatch());
                    m_msgToOrchestrator->mb_setBillType(m_msgFromOrchestrator->mb_getBillType());
                    m_msgToOrchestrator->mb_setRefNumber(m_msgFromOrchestrator->mb_getRefNumber());
                    m_msgToOrchestrator->mb_setCycleStartDate(m_msgFromOrchestrator->mb_getCycleStartDate());
                    m_msgToOrchestrator->mb_setCycleEndDate(m_msgFromOrchestrator->mb_getCycleEndDate());
                    m_msgToOrchestrator->mb_setEndTime();
                    m_msgToOrchestrator->mb_setPauseAction("ACK");
                    m_msgToOrchestrator->mb_setTimeStamp();
                    m_msgToOrchestrator->mb_setEndTime();
                    m_msgToOrchestrator->mb_setAdditonalActionString(m_msgFromOrchestrator->mb_getAdditonalActionString());
                    // Forming the SubBatchCount
                    m_msgToOrchestrator->mb_setSubBatchCount(m_execName+ORCH_IMMEDIATE_PAUSE);
                    m_msgToOrchestrator->mb_setBatchUpdateStatus(PRC_PENDING_STATE);

                    *m_ABL_ServicePtr << CRITICAL << "Updating the batch status through procedure" << Endl;

                    if(!m_orchMessengerPtr->mb_updateBatchStatus(m_msgToOrchestrator))
                    {
                        *m_ABL_ServicePtr << CRITICAL << "Error in Updating the batch status to Pending:" << m_msgFromOrchestrator->mb_getMainBatch().c_str()<< ":"<<m_msgFromOrchestrator->mb_getSubBatch().c_str()<< Endl;
                        //return false;
                        continue;
                    }

                    m_msgToOrchestrator->mb_print(m_ABL_ServicePtr);
                    *m_ABL_ServicePtr << INFO << "Sending message type 11 to orch for paused message" << Endl;

                    //Sending the Pause ack to Orchestrator
                    m_orchMessengerPtr->mb_writeMessage(m_pauseActionMessageFormat, m_msgToOrchestrator);

                    //Sending batch count message for pause
                    
                    m_msgToOrchestrator->mb_reset();
                    m_msgToOrchestrator->mb_setStartTime();

                    m_msgToOrchestrator->mb_setMainBatch(m_msgFromOrchestrator->mb_getMainBatch());
                    m_msgToOrchestrator->mb_setFullBillCycleCode(m_msgFromOrchestrator->mb_getFullBillCycleCode());
                    m_msgToOrchestrator->mb_setSubBatch(m_msgFromOrchestrator->mb_getSubBatch());
                    m_msgToOrchestrator->mb_setBillType(m_msgFromOrchestrator->mb_getBillType());
                    m_msgToOrchestrator->mb_setRefNumber(m_msgFromOrchestrator->mb_getRefNumber());
                    m_msgToOrchestrator->mb_setCycleStartDate(m_msgFromOrchestrator->mb_getCycleStartDate());
                    m_msgToOrchestrator->mb_setCycleEndDate(m_msgFromOrchestrator->mb_getCycleEndDate());
                    m_msgToOrchestrator->mb_setSubBatchCount(m_execName+ORCH_IMMEDIATE_PAUSE);

                    // Set the timestamp
                    m_msgToOrchestrator->mb_setTimeStamp();
                    
                    m_msgToOrchestrator->mb_print(m_ABL_ServicePtr);

                    *m_ABL_ServicePtr << INFO << "Sending message to Status Updater for updating sub batch count due to pause batch"<< Endl;

                    m_orchMessengerPtr->mb_writeMessage(m_batchCountMessageFormat, m_msgToOrchestrator);

                    //Now continue to listen for new requests
                    continue;
                }

                //Sending batch count message for pause
                m_msgToOrchestrator->mb_reset();

                //Set the start time for batch preparation
                m_msgToOrchestrator->mb_setStartTime();

                m_msgToOrchestrator->mb_setMainBatch(m_msgFromOrchestrator->mb_getMainBatch());
                m_msgToOrchestrator->mb_setFullBillCycleCode(m_msgFromOrchestrator->mb_getFullBillCycleCode());
                m_msgToOrchestrator->mb_setSubBatch(m_msgFromOrchestrator->mb_getSubBatch());
                m_msgToOrchestrator->mb_setBillType(m_msgFromOrchestrator->mb_getBillType());
                m_msgToOrchestrator->mb_setRefNumber(m_msgFromOrchestrator->mb_getRefNumber());
                m_msgToOrchestrator->mb_setCycleStartDate(m_msgFromOrchestrator->mb_getCycleStartDate());
                m_msgToOrchestrator->mb_setCycleEndDate(m_msgFromOrchestrator->mb_getCycleEndDate());
                m_msgToOrchestrator->mb_setSequenceNumber(m_msgFromOrchestrator->mb_getSequenceNumber());

                m_msgToOrchestrator->mb_setBatchPauseState(false);
                // Set the timestamp
                m_msgToOrchestrator->mb_setTimeStamp();

                m_msgToOrchestrator->mb_setSubBatchCount(m_execName+ORCH_NO_PAUSE_IN_PROGRESS);

                // Update batch status to inprogress
                m_msgToOrchestrator->mb_setBatchUpdateStatus(PRC_IN_PROGRESS_STATE);

                //std::cerr<< "Updating the batch status through procedure" << std::endl;
                if(!m_orchMessengerPtr->mb_updateBatchStatus(m_msgToOrchestrator))
                {
                    *m_ABL_ServicePtr << CRITICAL << "Error in Updating the batch status to Pending:" << m_msgFromOrchestrator->mb_getMainBatch().c_str() << ":"<< m_msgFromOrchestrator->mb_getSubBatch().c_str() <<Endl;
                    //return false;
                    continue;
                }
                m_msgToOrchestrator->mb_print(m_ABL_ServicePtr);

                *m_ABL_ServicePtr << INFO << "Sending message count to orch for processing"<< Endl;

                //Sending the message
                m_orchMessengerPtr->mb_writeMessage(m_batchCountMessageFormat, m_msgToOrchestrator);

                
                /*
                 * inserting the request into table.
                 */

                //std::cerr<< "inserting the request into table." << std::endl;

                l_insertStatement.mb_setString(1,m_msgFromOrchestrator->mb_getMainBatch());
                l_insertStatement.mb_setInt64(2,atol(m_msgFromOrchestrator->mb_getSubBatch().c_str()));
                l_insertStatement.mb_setString(3,m_msgFromOrchestrator->mb_getBillType());
                l_insertStatement.mb_setInt64(4,atol(m_msgFromOrchestrator->mb_getSequenceNumber().c_str()));
                if(m_currentBillRunMode == "P")
                {
                    l_insertStatement.mb_setString(5,"");
                }
                else
                {
                    l_insertStatement.mb_setString(5,m_msgFromOrchestrator->mb_getAdditonalActionString());
                }
                l_insertStatement.mb_setString(6,m_currentBillRunMode);
                l_insertStatement.mb_setString(7,"Q");
                l_insertStatement.mb_setString(8,"N");
                l_insertStatement.mb_setString(9,m_msgFromOrchestrator->mb_getFullBillCycleCode());
                l_insertStatement.mb_setDate(10,ABL_Date(m_msgFromOrchestrator->mb_getCycleStartDate().c_str()));
                l_insertStatement.mb_setDate(11,ABL_Date(m_msgFromOrchestrator->mb_getCycleEndDate().c_str()));
                l_insertStatement.mb_setInt64(12,atol(m_msgFromOrchestrator->mb_getRefNumber().c_str()));
                l_insertStatement.mb_executeUpdate();
                m_billDbConnectionPtr->mb_commit();

            }
            else
            {
                sleep(l_sleeptime);
            }
        }
        catch(ABL_Exception &e)
        {
            std::cerr<<"Exception"<<e.mb_getMessage().c_str()<<std::endl;
            *m_ABL_ServicePtr << CRITICAL << "ERROR while processing the message:"<< e.mb_getMessage() << Endl;
        }

        /*
        * get the completed sub batches and update to the Orchestrator
        */

        try
        {
            int l_irowsFetched = 0;
            l_resutSet = l_selectStatement.mb_executeQuery();

             *m_ABL_ServicePtr << INFO << "fetching processed requests from table." << Endl;

            while(l_resutSet.mb_fetch())
            {
                l_irowsFetched = l_resutSet.mb_getNumArrayRows();

                l_batchId = l_resutSet.mb_getString(1);
                l_subBatchId= l_resutSet.mb_getString(2);
                l_noOfAccounts = l_resutSet.mb_getInt64(3);
                l_noOfSuccessAcc = l_resutSet.mb_getInt64(4);
                l_noOfFailedAcc = l_resutSet.mb_getInt64(5);
                l_billType = l_resutSet.mb_getString(6);
                l_seqNo = l_resutSet.mb_getString(7);
                l_startDate = l_resutSet.mb_getString(8);
                l_endDate = l_resutSet.mb_getString(9);
                l_billCycleCode = l_resutSet.mb_getString(10);
                l_mock_req_num = l_resutSet.mb_getString(11);
                l_rowId = l_resutSet.mb_getString(12);

                //Sending batch count message for pause
                m_msgToOrchestrator->mb_reset();

                //Set the start time 
                m_msgToOrchestrator->mb_setStartTime();

                m_msgToOrchestrator->mb_setMainBatch(l_batchId);
                m_msgToOrchestrator->mb_setSubBatch(l_subBatchId);
                m_msgToOrchestrator->mb_setBillType(l_billType);
                //std::cerr<"l_mock_req_num:"<<l_mock_req_num<<std::endl;
                m_msgToOrchestrator->mb_setRefNumber(l_mock_req_num);
                m_msgToOrchestrator->mb_setCycleStartDate(l_startDate);
                m_msgToOrchestrator->mb_setCycleEndDate(l_endDate);
                m_msgToOrchestrator->mb_setPassedAccounts(l_noOfSuccessAcc);
                m_msgToOrchestrator->mb_setFailedAccounts(l_noOfFailedAcc);
                m_msgToOrchestrator->mb_setStatistics();
                m_msgToOrchestrator->mb_setFullBillCycleCode(l_billCycleCode);
                m_msgToOrchestrator->mb_setSequenceNumber(l_seqNo);
                // Set the timestamp
                m_msgToOrchestrator->mb_setTimeStamp();

                if(l_noOfFailedAcc == 0 )
                {
                    //m_msgToOrchestrator->mb_setComplete("Y");
                    m_msgToOrchestrator->mb_setBatchUpdateStatus(PRC_CLEAN_COMPLETE_STATE);
                }
                else if(l_noOfSuccessAcc == 0)
                {
                        //m_msgToOrchestrator->mb_setComplete("N");
                        m_msgToOrchestrator->mb_setBatchUpdateStatus(PRC_FAILED_STATE);
                }
                else
                {
                    //m_msgToOrchestrator->mb_setComplete("N");
                    m_msgToOrchestrator->mb_setBatchUpdateStatus(PRC_ERRORED_COMPLETE_STATE);
                }
                
                m_msgToOrchestrator->mb_setComplete("Y");
                 m_msgToOrchestrator->mb_print(m_ABL_ServicePtr);

                *m_ABL_ServicePtr << INFO << "Updating the count in batch table" << Endl;

                //std::cerr<< "Updating the batch status through procedure" << std::endl;

                if(!m_orchMessengerPtr->mb_updateBatchStatus(m_msgToOrchestrator))
                {
                     *m_ABL_ServicePtr << CRITICAL << "Error in Updating the batch status to Pending:" <<l_batchId << Endl;
                     //return false; cant return ... just continue
                     continue;
                }
                //Sending the message
                m_orchMessengerPtr->mb_writeMessage(m_statsUpdatorMessageFormat, m_msgToOrchestrator);

                 //Sending batch count message for pause
                m_msgToOrchestrator->mb_reset();

                //Set the start time 
                m_msgToOrchestrator->mb_setStartTime();

                m_msgToOrchestrator->mb_setMainBatch(l_batchId);
                m_msgToOrchestrator->mb_setSubBatch(l_subBatchId);
                m_msgToOrchestrator->mb_setBillType(l_billType);
                m_msgToOrchestrator->mb_setRefNumber(l_mock_req_num);
                m_msgToOrchestrator->mb_setCycleStartDate(l_startDate);
                m_msgToOrchestrator->mb_setCycleEndDate(l_endDate);
                m_msgToOrchestrator->mb_setSubBatchCount(m_execName+ORCH_FULLY_COMPLETE);
                m_msgToOrchestrator->mb_setFullBillCycleCode(l_billCycleCode);
                // Set the timestamp
                m_msgToOrchestrator->mb_setTimeStamp();
                m_msgToOrchestrator->mb_setSequenceNumber(l_seqNo);

                m_msgToOrchestrator->mb_print(m_ABL_ServicePtr);

                *m_ABL_ServicePtr << INFO << "Sending message count to Status updater for processed"<< Endl;
                //Sending the message
                m_orchMessengerPtr->mb_writeMessage(m_batchCountMessageFormat, m_msgToOrchestrator);

                //updating the request as processed in CB_PRE_POST_BILL_CHECK_REQUEST

                /// update based on rowid
                l_updateStatement.mb_setString(1,l_rowId);

                //Execute the prepared statement
                l_updateStatement.mb_executeUpdate();

                *m_ABL_ServicePtr << INFO << "Updated status to C in CB_PRE_POST_BILL_CHECK_REQUEST for batchId:" << l_batchId<< Endl;
            }

            if (l_irowsFetched > 0)
            {
                //Comitting after processing all the requests taken and as we are using select for update
                m_billDbConnectionPtr->mb_commit();
                *m_ABL_ServicePtr << INFO << "Commit"<<Endl;
            }
            else
            {
                *m_ABL_ServicePtr << INFO << "No completed requests in CB_PRE_POST_BILL_CHECK_REQUEST" << Endl;
            }

        }
        catch(ABL_Exception &e)
        {
            std::cerr<<"Exception"<<e.mb_getMessage().c_str()<<std::endl;
            *m_ABL_ServicePtr <<"ERROR"<< e.mb_getMessage() << Endl;
            m_billDbConnectionPtr->mb_rollback(); // to release locks in case of any error
        }
    }
}

//////////////////////////////////////////////////////////////////////
//
//      Process the confirm the requests.
//
//////////////////////////////////////////////////////////////////////
//
//  Parameters  :   p_runMode        -> Engine running mode
//              :   p_billTypeInMsg  -> Bill type.   
//
//////////////////////////////////////////////////////////////////////
bool PreAndPostBillWrapper::mb_validRequest(ABL_String p_runMode, ABL_String p_billTypeInMsg)
{
    int l_billType = toupper(p_billTypeInMsg[0]);
    if(l_billType == 'H' && p_runMode == "N") //Received non standard request
    {
        return true;
    }
    else if(l_billType == 'S' && p_runMode == "S")
    {
        return true;
    }
    return false;
}
/*********************************************************************
#
#                 Copyright 2012 - 2013  
#                 Tecnotree Convergence Ltd
#
 *********************************************************************/
